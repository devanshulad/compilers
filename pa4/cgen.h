#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include "cool-tree.h"
#include "emit.h"
#include "symtab.h"
#include <map>
#include <vector>

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;


class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
  std::list<CgenNodeP> nds;
  std::ostream& str;
  // SymbolTable<Symbol,int> class_to_tag_table;

  // The following methods emit code for constants and global declarations.
  void code_global_data();
  void code_global_text();
  void code_bools();
  void code_select_gc();
  void code_constants();

  // The following creates an inheritance graph from a list of classes. The
  // graph is implemented as  a tree of `CgenNode', and class names are placed
  // in the base class symbol table.
  void install_basic_classes();
  void install_class(CgenNodeP nd);
  void install_classes(Classes cs);
  void build_inheritance_tree();
  void set_relations(CgenNodeP nd);
  void build_parent_map();

  // we added 
  void all_object_inits();
  void recursive_gen_code();
  void all_funcs();
  void make_dispatch_tables(ostream& s);
  void assign_tags();
  void make_protos();
  void make_attr_tables();
  void class_obj_table();
  void class_name_tab();
  void class_parent_tab();
  void reverse_nds_list();

  
public:
  CgenClassTable(Classes, std::ostream& str);
  void code();
  CgenNodeP root();
};

class CgenNode : public class__class {
private:

  CgenNodeP parentnd;
  std::list<CgenNodeP> children;
  Basicness basic_status;

public:
  CgenNode(Class_ c,
	   Basicness bstatus,
	   CgenClassTableP class_table);

  void add_child(CgenNodeP child);
  std::list<CgenNodeP>& get_children() { return children; }
  void set_parentnd(CgenNodeP p);
  CgenNodeP get_parentnd();
  int basic() { return (basic_status == Basic); }
  void class_init_func(ostream& s);
  int get_num_parents_attr();
  void make_dispatch(ostream& s, Symbol class_name);
  void rec_proto(ostream& s);
  void make_prototype(ostream& s);
  void make_attr(ostream& s, bool isFirst);
  void create_func_to_param(ostream& s, Symbol class_name);
  void rec_add_params(std::map<Symbol, std::vector<Symbol>>& func_to_param);
  void create_class_to_attribute(ostream& s, Symbol class_name);
  void rec_add_attributes(std::vector<Symbol>& attributes);
};

class BoolConst {
 private:
  int val;
 public:
  BoolConst(int);
  void code_def(std::ostream&, int boolclasstag);
  void code_ref(std::ostream&) const;
};

