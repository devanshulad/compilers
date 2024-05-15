#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include <list>
#include <map>
#include <set>
#include <vector>


#define TRUE 1
#define FALSE 0

class InheritanceNode;
typedef InheritanceNode *InheritanceNodeP;
class ClassTable;
typedef ClassTable *ClassTableP;


// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.
class ClassTable : public SymbolTable<Symbol, InheritanceNode> {
private:
  int semant_errors;           // counts the number of semantic errors
  void install_basic_classes();
  

  void add_classes_check_duplicates(Classes classes);
  void check_inheritance ();
  void check_acyclic ();
  void check_cycle_for_class (Class_& curr_class, std::set<Symbol>& visited);
  void check_main ();
  void exit_func ();
  void add_methods(Classes classes);
  void create_inheritance (Classes classes);
  void check_features_inheritance ();
  void ancestor_method_check (Class_ curr_class, method_class* curr, method_class* parent);
  // Symbol find_common_ancestor (Symbol a, Symbol b);
  // std::vector<Symbol> reorder_classes();
  void print_sym_table();
  void make_sym_table_class(Symbol c);
  void check_all_classes(Classes );
  void make_sym_table_class_helper(Class_ c, SymbolTable<Symbol, Symbol>*& curr_sym_table);

public:
  ClassTable(Classes);
  void run(Classes);
  int errors() { return semant_errors; }
  std::ostream& semant_error();
  std::ostream& semant_error(Class_ c);
  std::ostream& semant_error(Symbol filename, tree_node *t);
  std::ostream& semant_error (Class_ c, int line_no);
  std::ostream& error_stream;
};


#endif
