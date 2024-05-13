

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <iostream>

extern int semant_debug;
extern char *curr_filename;
extern int node_lineno;

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
  exit_func();
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
      if ((method_list->nth(i))->diff()) {
        auto curr_method = (static_cast<method_class*>(method_list->nth(i)));
        auto method_name = curr_method->getName();
        if (curr_class_method_list.count(method_name) != 0) {
          semant_error(curr_class);
          error_stream << "Method " << method_name << " is multiply defined."<< endl;
        }
        else {
          curr_class_method_list[method_name] = curr_method;
        }
      }
      else {
        auto curr_attr = (static_cast<attr_class*>(method_list->nth(i)));
        auto attr_name = curr_attr->getName();
        if (curr_class_attr_list.count(attr_name) != 0) {
          semant_error(curr_class);
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
void program_class::semant() {
   initialize_constants();
   //cout << "HIIIIIIIIIII\n\n" << endl;
  //  cout << classes << endl;
  // for(int i = classes->first(); classes->more(i); i = classes->next(i))
  // {
  //   cout << classes->nth(i)->dump(cout)<< " hi " << endl;
  // }

    /* ClassTable constructor may do some semantic analysis */
   ClassTableP classtable = new ClassTable(classes);

   if (classtable->errors()) {
      cerr << "Compilation halted due to static semantic errors." << endl;
      exit(1);
   }
}
