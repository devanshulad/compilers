

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <iostream>
#include <vector>
#include <unordered_set>


extern int semant_debug;
extern char *curr_filename;
extern int node_lineno;

std::map <Symbol, Class_> class_list;
std::map <Symbol, std::map <Symbol, method_class*>> all_method_list;
std::map <Symbol, std::map <Symbol, attr_class*>> all_attr_list;
std::map <Symbol, std::vector<Symbol>> inheritance_map_class;
std::map <Symbol, SymbolTable<Symbol, Symbol>*> sym_table;
ClassTableP classtable;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy,
       Int,
       in_int,
       in_string,
       IO,
       isProto,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       _BOTTOM_,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void) {
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  ::copy      = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  isProto     = idtable.add_string("isProto");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
  //   _no_class is a symbol that can't be the name of any
  //   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  // _BOTTOM_ is the symbol for the bottom of the lattice of types
  _BOTTOM_    = idtable.add_string("_bottom");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

void ClassTable::exit_func() {
  if (this->errors()) {
      cerr << "Compilation halted due to static semantic errors." << endl;
      exit(1);
   }
}


ClassTable::ClassTable(Classes classes) : semant_errors(0), error_stream(cerr) {
  /* Fill this in */
  install_basic_classes();
}

void ClassTable::run(Classes classes) {
  exit_func();
  add_classes_check_duplicates (classes);
  exit_func();
  check_inheritance();
  exit_func();
  check_acyclic();
  exit_func();
  check_main();
  exit_func();
  add_methods(classes);
  create_inheritance (classes);
  check_features_inheritance ();
  exit_func();
  check_all_classes(classes);
  // for (auto& pair: all_attr_list) {
  //   for (auto& pair2: pair.second) {

  //     std::cout << "Class: " << pair.first << " Attribute: " << pair2.first << std::endl;
  //     std::cout << "Type: " << pair2.second->typeDecl() << std::endl;
  //     std::cout << "Init: " << pair2.second->getInit()->returnType() << std::endl;
  //   }
  // }

  print_sym_table();

// std::vector<Symbol> classes_reordered = reorder_classes();
//   cout << "Reordered Classes: ";
//   for (Symbol class_name: classes_reordered) {
//     std::cout << class_name << ", ";
//   }
//   std::cout << std::endl;
// }
}

void ClassTable::add_classes_check_duplicates(Classes classes) {
  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    Class_ curr_class = classes->nth(i);
    Symbol curr_name = curr_class->getName();
    if (class_list.count(curr_name) > 0) {
      // double check this error msg
      semant_error(curr_class);
      error_stream << "Class " << curr_name << " was previously defined." << endl;
    }
    class_list[curr_name] = curr_class;
  }
}

void ClassTable::check_inheritance() {
  for (auto& name_class: class_list) {
    Symbol curr_name = name_class.first;
    Class_ curr_class = name_class.second;

    if (curr_class->getParent() != No_class && class_list.count(curr_class->getParent()) == 0)
    {
      semant_error(curr_class);
      error_stream << "Class " << curr_name << " inherits from an undefined class " << curr_class->getParent() << "." << endl;
    }
  }
}

void ClassTable::check_acyclic() {
  for (auto& name_class: class_list) {
    std::set<Symbol> visited;
    check_cycle_for_class(name_class.second, visited);
  }
}

void ClassTable::check_cycle_for_class(Class_& curr_class, std::set<Symbol>& visited) {
  if (visited.find(curr_class->getParent()) != visited.end()) {
    semant_error(curr_class);
    error_stream << "Class " << curr_class->getName() << ", or an ancestor of " << curr_class->getName() << ", is involved in an inheritance cycle." << endl;
    return;
  }
  visited.insert(curr_class->getName());

  // following if should always be true
  if (class_list.count(curr_class->getParent()) != 0)
    check_cycle_for_class (class_list[curr_class->getParent()], visited);
}

void ClassTable::check_main() {
  // Check if class Main is defined
  if (class_list.count(Main) == 0) {
    semant_error();
    error_stream << "Class Main is not defined. " << endl;
    return;
  }
  // check if main function is defined in class Main 
  auto main_class = class_list[Main];
  auto feature_list = main_class->getFeatures();
  for (int i = feature_list->first(); feature_list->more(i); i = feature_list->next(i)) {
    //cout << "name is " << (static_cast<method_class*>(feature_list->nth(i)))->getName() << endl;
    if ( (static_cast<method_class*>(feature_list->nth(i)))->getName() == main_meth){
      return;
    }
  }
  semant_error(main_class);  
  error_stream << "No 'main' method in class Main." << endl;
}

void ClassTable::add_methods(Classes classes) {

  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    Class_ curr_class = classes->nth(i);
    Symbol class_name = curr_class->getName();
    std::map <Symbol, method_class*> curr_class_method_list;
    std::map <Symbol, attr_class*> curr_class_attr_list;
    auto method_list = curr_class->getFeatures();
    for (int i = method_list->first(); method_list->more(i); i = method_list->next(i)) {
      // auto curr_method = (static_cast<method_class*>(method_list->nth(i)));
      // auto method_name = curr_method->getName();
      if ((method_list->nth(i))->isMethod()) {
        method_class* curr_method = (static_cast<method_class*>(method_list->nth(i)));
        Symbol method_name = curr_method->getName();
        if (curr_class_method_list.count(method_name) != 0) {
          semant_error(curr_class, curr_method->get_line_number());
          error_stream << "Method " << method_name << " is multiply defined."<< endl;
        }
        else {
          curr_class_method_list[method_name] = curr_method;
          std::unordered_set<Symbol> formal_check;
          Formals formal_list = curr_method->getFormals();
          for (int j = formal_list->first(); formal_list->more(j); j = formal_list->next(j)) {
            auto curr_formal = formal_list->nth(j);
            Symbol formal_name = curr_formal->getName();
            if (formal_check.find(formal_name) != formal_check.end()) {
              semant_error(curr_class, curr_formal->get_line_number());
              error_stream << "Formal parameter " << formal_name << " is multiply defined."<< endl;
            }
            else {
              formal_check.insert(formal_name);
            }
          }
        }
      }
      else {
        attr_class* curr_attr = (static_cast<attr_class*>(method_list->nth(i)));
        Symbol attr_name = curr_attr->getName();
        if (curr_class_attr_list.count(attr_name) != 0) {
          semant_error(curr_class, curr_attr->get_line_number());
          error_stream << "Attribute " << attr_name << " is multiply defined in class."<< endl;
        }
        else {
          curr_class_attr_list[attr_name] = curr_attr;
        }
      }
    }
    all_method_list[class_name] = curr_class_method_list;
    all_attr_list[class_name] = curr_class_attr_list;
  }
}

void ClassTable::create_inheritance (Classes classes) {
  // does not add no_class as a parent
  for (auto& name_class: class_list) {
    Class_ curr_class = name_class.second;
    Symbol curr_name = name_class.first;
    Symbol curr_parent = curr_class->getParent();

    std::vector<Symbol> val;

    if (curr_parent != No_class)
      val.push_back(curr_parent);

    while (curr_parent != No_class) {
      Class_ parent = class_list[curr_parent];

      Symbol new_parent = parent->getParent();
      if (new_parent != No_class) {
        val.push_back(new_parent);
      }
      curr_parent = new_parent;
    }

    inheritance_map_class[curr_class->getName()] = val;
    // code to print out inheritance map
    // std::cout << "Current class is " << curr_name << ": ";
    // for (Symbol element : val) {
    //     std::cout << element << ", ";
    // }
    // std::cout << std::endl;
  }
}

// function to make sure methods and attributes are okay with inheritance
void ClassTable::check_features_inheritance() {
  // loop through all of the "class: list of parent class" entries
  for (auto& name_parents_list: inheritance_map_class) {
    std::vector<Symbol> parents_list = name_parents_list.second;
    Symbol curr_name = name_parents_list.first;
    Class_ curr_class = class_list[curr_name];
    auto my_features = curr_class->getFeatures();

    // loop through each inherited parent class
    for (Symbol parent: parents_list) {
      Class_ parent_class = class_list[parent];
      auto parent_features = parent_class->getFeatures();

      //loop through all features in the inherited class
      for (int i = parent_features->first(); parent_features->more(i); i = parent_features->next(i)) {
        
        //loop through all features in this class
        for (int j = my_features->first(); my_features->more(j); j = my_features->next(j)) {
          // check if we are looking at an attribute or a method
          // if looking at attribute then only have to compare name and types?
          // if looking at method have to compare name, parameter number, parameter types, and return types
          // shld offload method comparison to different function imo
          // if equal Semant error 
          if ((my_features->nth(j))->isMethod()) {
            if ((parent_features->nth(i))->isMethod()) {
              method_class* curr_method = (static_cast<method_class*>(my_features->nth(j)));
              method_class* par_method = (static_cast<method_class*>(parent_features->nth(i)));
              if (curr_method->getName() == par_method->getName()) {
                ancestor_method_check(curr_class, curr_method, par_method);
              }
            } 
          } else {
            if (!((parent_features->nth(i))->isMethod())) {
              attr_class* curr_attr = (static_cast<attr_class*>(my_features->nth(j)));
              attr_class* par_attr = (static_cast<attr_class*>(parent_features->nth(i)));
              if (curr_attr->getName() == par_attr->getName()) {
                semant_error(curr_class, curr_attr->get_line_number());
                error_stream << "Attribute " << curr_attr->getName() << " is an attribute of an inherited class."<< endl;
              }
            }
          }

        }
      }

    }
  }
}

void ClassTable::ancestor_method_check (Class_ curr_class, method_class* curr, method_class* parent) {
  if (curr->getReturnType() != parent->getReturnType()) {
      semant_error(curr_class, curr->get_line_number());
      error_stream << "In redefined method " << curr->getName() << ", return type " << curr->getReturnType() 
                   << " is different from original return type " << parent->getReturnType() << "." << endl;
  }
  
  std::vector<Symbol> child_list; 
  std::vector<Symbol> parent_list; 
  
  Formals child_formals = curr->getFormals();
  for (int i = child_formals->first(); child_formals->more(i); i = child_formals->next(i)) {
    Symbol curr_type = child_formals->nth(i)->typeDecl();
    child_list.push_back(curr_type);
  }
  Formals parent_formals = parent->getFormals();
  for (int i = parent_formals->first(); parent_formals->more(i); i = parent_formals->next(i)) {
    Symbol curr_type = parent_formals->nth(i)->typeDecl();
    parent_list.push_back(curr_type);
  }

  if (child_list.size() != parent_list.size()) {
    semant_error(curr_class, curr->get_line_number());
    error_stream << "Incompatible number of formal parameters in redefined method " << curr->getName() << endl;
    return;
  }

  if (child_list != parent_list) {
    for (int j = 0; j < int(child_list.size()); j++) {
      if (child_list[j] != parent_list[j]) {
        semant_error(curr_class, curr->get_line_number());
        error_stream << "In redefined method " << curr->getName() << ", parameter type " 
                     << child_list[j] << " is different from original type " << parent_list[j] << endl;
        return;
      }
    }
  }
}

// std::vector<Symbol> ClassTable::reorder_classes() {
//   std::vector<Symbol> known_classes;
//   while (known_classes.size() != inheritance_map_class.size()) {
//     Symbol next_class = No_class;
//     for (auto& class_parents: inheritance_map_class) {
//       std::vector<Symbol> parents = class_parents.second;
//       bool foundMatch = true;
//       for (Symbol parent: parents) {
//         if (known_classes.find(parent) == known_classes.end()) {
//           foundMatch = false;
//         }
//       }
//       if (foundMatch) {
//         next_class = class_parents.first;
//         break;
//       }
//     }
//     known_classes.push_back(next_class);
//   }
//   return known_classes;
// }


void ClassTable::install_basic_classes() {
  // The tree package uses these globals to annotate the classes built below.
  node_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

  // The following demonstrates how to create dummy parse trees to
  // refer to basic Cool classes.  There's no need for method
  // bodies -- these are already built into the runtime system.

  // IMPORTANT: The results of the following expressions are
  // stored in local variables.  You will want to do something
  // with those variables at the end of this method to make this
  // code meaningful.


  //
  // The Object class has no parent class. Its methods are
  //        cool_abort() : Object    aborts the program
  //        type_name() : Str        returns a string representation of class name
  //        copy() : SELF_TYPE       returns a copy of the object
  //
  // There is no need for method bodies in the basic classes---these
  // are already built in to the runtime system.
  //

  Class_ Object_class =
    class_(Object,
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(::copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename);
    class_list[Object] = Object_class;

  //
  // The IO class inherits from Object. Its methods are
  //        out_string(Str) : SELF_TYPE          writes a string to the output
  //        out_int(Int) : SELF_TYPE               "    an int    "  "     "
  //        in_string() : Str                    reads a string from the input
  //        in_int() : Int                         "   an int     "  "     "
  //

  Class_ IO_class =
     class_(IO,
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	    filename);
    class_list[IO] = IO_class;

  //
  // The Int class has no methods and only a single attribute, the
  // "val" for the integer.
  //

  Class_ Int_class =
      class_(Int,
	     Object,
	     single_Features(attr(val, prim_slot, no_expr())),
	     filename);
      class_list[Int] = Int_class;


  //
  // Bool also has only the "val" slot.
  //

  Class_ Bool_class =
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);
      class_list[Bool] = Bool_class;

  //
  // The class Str has a number of slots and operations:
  //       val                                  the length of the string
  //       str_field                            the string itself
  //       length() : Int                       returns length of the string
  //       concat(arg: Str) : Str               performs string concatenation
  //       substr(arg: Int, arg2: Int): Str     substring selection
  //

  Class_ Str_class =
      class_(Str,
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat,
				   single_Formals(formal(arg, Str)),
				   Str,
				   no_expr()))),
	    single_Features(method(substr,
				   append_Formals(single_Formals(formal(arg, Int)),
						  single_Formals(formal(arg2, Int))),
				   Str,
				   no_expr()))),
	     filename);
      class_list[Str] = Str_class;
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
//       print a line number and filename
//       (line number is extracted from tree_node)
//
///////////////////////////////////////////////////////////////////
ostream& ClassTable::semant_error(Class_ c)
{
  return semant_error(c->get_filename(),c);
}

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
  error_stream << filename << ":" << t->get_line_number() << ": ";
  return semant_error();
}

ostream& ClassTable::semant_error()
{
    semant_errors++;
    return error_stream;
}

ostream& ClassTable::semant_error (Class_ c, int line_no) {
  error_stream <<c->get_filename()  << ":" << line_no << ": ";
  return semant_error();
}

/*
 * This is the entry point to the semantic checker.
 *
 * Your checker should do the following two things:
 *
 *   1) Check that the program is semantically correct
 *   2) Decorate the abstract syntax tree with type information
 *      by setting the `type' field in each Expression node.
 *      (see `tree.h')
 *
 *   You are free to first do 1), make sure you catch all semantic
 *   errors. Part 2) can be done in a second stage, when you want
 *   to build mycoolc.
 */

Symbol find_common_ancestor(Symbol class1, Symbol class2) {
  std::vector<Symbol> class1_ancestors = inheritance_map_class[class1];
  class1_ancestors.insert(class1_ancestors.begin(), class1);
  std::vector<Symbol> class2_ancestors = inheritance_map_class[class2];
  class2_ancestors.insert(class2_ancestors.begin(), class2);
  for (Symbol ancestor1: class1_ancestors) {
    for (Symbol ancestor2: class2_ancestors) {
      if (ancestor1 == ancestor2) {
        return ancestor1;
      }
    }
  }
  return Object;
}

void ClassTable::make_sym_table_class_helper(Class_ c, SymbolTable<Symbol, Symbol>*& curr_sym_table) {
  Symbol class_name = c->getName();
  //curr_sym_table.addid(self, &class_name); // check this
  Features features = c->getFeatures();
  int j = 0;
  Symbol d = NULL;
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    if (features->nth(i)->isMethod()) {
      //method_class* curr_method = (static_cast<method_class*>(features->nth(i)));
      //curr_sym_table.addid(curr_method->getName(), &curr_method->getReturnType());
    }
    else {
      if (j == 0) {
        curr_sym_table->enterscope();
      }
      attr_class* curr_attr = (static_cast<attr_class*>(features->nth(i)));
      Symbol entry = static_cast<Symbol>(curr_attr->typeDecl());
      if (j == 0) {
        d =  curr_attr->getName();
        // cout << "trying   " << d << endl;
      }
      j+=1;
      // cout << "line 605 " << curr_attr->getName() << "\t" << curr_attr->typeDecl() << endl;
      curr_sym_table->addid(curr_attr->getName(), new Symbol(curr_attr->typeDecl()));
      // cout << "line 611 " << curr_attr->getName() << "\t" << entry << endl;
      // cout << "byeee " << *curr_sym_table->lookup(d) << endl;
    }
  }
  if (d != NULL) {
    // cout << "BLAHH  " << *curr_sym_table->lookup(d) << endl;
  }
  sym_table[c->getName()] = curr_sym_table;
  // curr_sym_table->dump();
  
  for (int k = features->first(); features->more(k); k = features->next(k)) {
    if (features->nth(k)->isMethod()) {
      //method_class* curr_method = (static_cast<method_class*>(features->nth(i)));
      //curr_sym_table.addid(curr_method->getName(), &curr_method->getReturnType());
    }
    else {
      // if (j == 0) {
      //   curr_sym_table->enterscope();
      // }
      // j+=1;
      attr_class* curr_attr = (static_cast<attr_class*>(features->nth(k)));
      Symbol entry = static_cast<Symbol>(curr_attr->typeDecl());
      // cout << "before coming to c" << endl;
      // cout << "line 632 " << curr_attr->getName() << "\t" << curr_attr->typeDecl() << endl;
      // cout << "HII  " << *curr_sym_table->lookup(d) << endl;
      Symbol type = curr_attr->getInit()->returnType(c);
      // cout << "symbol type = " << type << endl;

      if (find_common_ancestor(curr_attr->typeDecl(), type) != curr_attr->typeDecl()) {
        //cout << "ERROR HERE    " << find_common_ancestor(curr_attr->typeDecl(), type)  << endl;
         semant_error(c, c->get_line_number());
         error_stream << "Inferred type " << type << " of initialization of attribute " << curr_attr-> getName() 
         << " does nor conform to declared type " << curr_attr->typeDecl() << "." <<endl;

        // print error here
      } //else {
        // cout << "ADDING TO SYM TABLE      " << curr_attr->getName() << entry <<endl;
        //curr_sym_table->addid(curr_attr->getName(), &entry);
        // cout << "ADDED" << endl;
        // curr_sym_table->dump();
      //}
    }
  }
  // curr_sym_table->dump();
}

void ClassTable::print_sym_table() {
  for (auto& entry : sym_table) {
    Symbol class_name = entry.first;
    SymbolTable<Symbol, Symbol>* sym_table_class = entry.second;

    // std::cout << "Class: " << class_name;

    // sym_table_class.dump();

    // std::cout << std::endl;
  }
}

void ClassTable::make_sym_table_class(Symbol c) {
  std::vector<Symbol> ancestors = inheritance_map_class[c];
  SymbolTable<Symbol, Symbol>* curr_sym_table = new SymbolTable<Symbol, Symbol>();
  
  for (int i = ancestors.size() - 1; i >= 0; i--) {
    // cout << "  * making inheritance for class " << ancestors[i] << endl;
    make_sym_table_class_helper(class_list[ancestors[i]], curr_sym_table);
    // sym_table[c] = curr_sym_table;
  }
  // cout << "  * making symtable for current class " << c << endl;
  // curr_sym_table->dump();
  make_sym_table_class_helper(class_list[c], curr_sym_table);
  // sym_table[c] = curr_sym_table;

  
}

void ClassTable::check_all_classes(Classes classes) {
  // for (auto& name_class: classes) {
  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    Class_ curr_class = classes->nth(i);
    // cout << "  *** start for class " << curr_class->getName() << endl;
    make_sym_table_class(curr_class->getName());
    // cout << "  *** done for class " << curr_class->getName() << endl;
  }
}


void program_class::semant() {
   initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
   classtable = new ClassTable(classes);
   classtable->run(classes);

   if (classtable->errors()) {
      cerr << "Compilation halted due to static semantic errors." << endl;
      exit(1);
   }
}

Symbol assign_class::returnType(Class_ C) {
  Symbol type = *sym_table[C->getName()]->lookup(name);
  if (type == NULL) {
    // print error here
    // C->semant_error(C);
  }

  // one of expr's parent has to be type
  Symbol expr_type = expr->returnType(C);
  // cout << "REACHING expr->returnType(C)" << endl;

  if (find_common_ancestor(expr_type, type) != type) {
    // print error here
    // C-semant_error(C);
  }

  // check if name is in symbol table
  // evaluate type of expr
  // see if type of expr is a subclass of type of name
  // if so return type of expr, else error.
  set_type(expr_type);
  return expr_type;
}

Symbol static_dispatch_class::returnType(Class_ C) {
  return Int;
}

Symbol dispatch_class::returnType(Class_ C) {
  return Int;
}

Symbol cond_class::returnType(Class_ C) {
  Symbol first_type = pred->returnType(C);
  Symbol second_type = then_exp->returnType(C);
  Symbol third_type = else_exp->returnType(C);
  if (first_type != Bool) {
    // print error here
  }
  // need to find common ancestor and return that, need to also see how we will handel none_type because else can be none type
  return Bool;
}

Symbol loop_class::returnType(Class_ C) {
  if (pred->returnType(C) != Bool) {
    // print error here
  }
  set_type(Object);
  return Object;
}

Symbol typcase_class::returnType(Class_ C) {
  return Int;
}

Symbol block_class::returnType(Class_ C) {
  return Int;
}

Symbol let_class::returnType(Class_ C) {
  //need to complete 
  return Int;
}

Symbol plus_class::returnType(Class_ C) {
  // cout << "plus " << e2->name << endl;
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  // cout<< "first_type  " << first_type << "second type  " << second_type << endl;
  
  if (first_type != Int || second_type != Int) {
    classtable->semant_error(C, this->get_line_number());
    classtable->error_stream << "non-Int arguments: " << first_type << " + " << second_type << endl;
    set_type(Int);
    return Int;
  }
  set_type(Int);
  return Int;
}

Symbol sub_class::returnType(Class_ C) {
  //return Int;
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  if (first_type != Int || second_type != Int) {
    //need to print error here, maybe need to get curr class ?
    set_type(Object);
    return Object;
  }
  set_type(Int);
  return Int;
}

Symbol mul_class::returnType(Class_ C) {
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  if (first_type != Int || second_type != Int) {
    //need to print error here, maybe need to get curr class ?
    set_type(Object);
    return Object;
  }
  set_type(Int);
  return Int;
} 

Symbol divide_class::returnType(Class_ C) {
  // return Int;
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  if (first_type != Int || second_type != Int) {
    //need to print error here, maybe need to get curr class ?
    set_type(Object);
    return Object;
  }
  set_type(Int);
  return Int;
} 

Symbol neg_class::returnType(Class_ C) {
  return Int;
  Symbol first_type = e1->returnType(C);
  if (first_type != Int) {
    //need to print error here, maybe need to get curr class ?
    set_type(Object);
    return Object;
  }
  set_type(Int);
  return Int;
}

Symbol lt_class::returnType(Class_ C) {
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  if (first_type != Int || second_type != Int) {
    //need to print error here, maybe need to get curr class ?
    set_type(Object);
    return Object;
  }
  set_type(Int);
  return Int;
}

Symbol eq_class::returnType(Class_ C) {
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  if (first_type == Int || first_type == Str || first_type == Bool || second_type == Int || second_type == Str || second_type == Bool) {
      if (first_type != second_type) {
        // write error here
        set_type(Object);
        return Object;
      }
  }
  set_type(Bool);
  return Bool;
}

Symbol leq_class::returnType(Class_ C) {
  Symbol first_type = e1->returnType(C);
  Symbol second_type = e2->returnType(C);
  if (first_type != Int || second_type != Int) {
    //need to print error here, maybe need to get curr class ?
    set_type(Object);
    return Object;
  }
  set_type(Int);
  return Int;
} 

Symbol comp_class::returnType(Class_ C) {
  set_type(Bool);
  return Bool;
}

Symbol int_const_class::returnType(Class_ C) {
  // cout << "Reached INT CONST CLASS " << endl;
  set_type(Int);
  return Int;
}

Symbol bool_const_class::returnType(Class_ C) {
  set_type(Bool);
  return Bool;
}

Symbol string_const_class::returnType(Class_ C) {
  set_type(Str);
  return Str;
}

Symbol new__class::returnType(Class_ C) {
  if (type_name == SELF_TYPE) {
    set_type(SELF_TYPE);
    return SELF_TYPE;
  }
  // might need to check if type_name is defined in symbol table or not
  // i think ?
  set_type(type_name);
  return type_name;
}

Symbol isvoid_class::returnType(Class_ C) {
  set_type(Bool);
  return Bool;
  // i think this is correct
}

Symbol no_expr_class::returnType(Class_ C) {
  set_type(No_type);
  return No_type;
  // should also be correct ?
}

Symbol object_class::returnType(Class_ C) {
  Symbol *type = sym_table[C->getName()]->lookup(name);
  // cout << "reached " << *type << name << endl;

  if (type == NULL) {
    classtable->semant_error(C, this->get_line_number());
    classtable->error_stream << "Undeclared identifier " << name << "." <<endl;
    set_type(Object);
    return Object;
  }
  set_type(*type);
  return *type;
}
