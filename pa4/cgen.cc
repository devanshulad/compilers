
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_supp.h"
#include "handle_flags.h"
#include <set>
#include <vector>
#include <iostream>
#include <map>
#include <set>

//
// Two symbols from the semantic analyzer (semant.cc) are used.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
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
       length,
       Main,
       main_meth,
       No_class,
       No_type,
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
static void initialize_constants(void)
{
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
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
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

static const char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static const char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };

//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

std::map<Symbol, std::vector<Symbol>> parent_map;
std::map<Symbol, int> class_to_tag;
int tag_counter = 5;
//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//
// Note that Spim wants comments to start with '#'. For example:
// os << "# start of generated code\n";
// os << "\n# end of generated code\n";
//
//*********************************************************
void program_class::cgen(ostream &os) {
   initialize_constants();
   CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);
}

//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

// Added for consistent label support.
static std::string get_label_ref(int l)
{ std::stringstream ss;
  ss << l;
  std::string lbl = "label" + ss.str();
  return lbl;
}

static void emit_load(const char *dest_reg, int offset, const char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")"
    << std::endl;
}

static void emit_store(const char *source_reg, int offset, const char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << std::endl;
}

static void emit_load_imm(const char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << std::endl; }

static void emit_load_address(const char *dest_reg, const char *address, ostream& s)
{ s << LA << dest_reg << " " << address << std::endl; }

static void emit_partial_load_address(const char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(const char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << std::endl;
}

static void emit_load_string(const char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << std::endl;
}

static void emit_load_int(const char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << std::endl;
}

static void emit_move(const char *dest_reg, const char *source_reg, ostream& s)
{
  s << MOVE << dest_reg << " " << source_reg << std::endl;
}

static void emit_neg(const char *dest, const char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << std::endl; }

static void emit_add(const char *dest, const char *src1, const char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << std::endl; }

static void emit_addu(const char *dest, const char *src1, const char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << std::endl; }

static void emit_addiu(const char *dest, const char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << std::endl; }

static void emit_div(const char *dest, const char *src1, const char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << std::endl; }

static void emit_mul(const char *dest, const char *src1, const char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << std::endl; }

static void emit_sub(const char *dest, const char *src1, const char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << std::endl; }

static void emit_sll(const char *dest, const char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << std::endl; }

static void emit_jalr(const char *dest, ostream& s)
{ s << JALR << "\t" << dest << std::endl; }

static void emit_jal(char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << std::endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << std::endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << get_label_ref(l); }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << ":" << std::endl;
}

static void emit_beqz(const char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_beq(const char *src1, const char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_bne(const char *src1, const char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_bleq(const char *src1, const char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_blt(const char *src1, const char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_blti(const char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_bgti(const char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << std::endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << std::endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(const char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

static void emit_pop(const char *reg, ostream& str)
{
  emit_addiu(SP,SP,4,str);
  emit_load(reg,0,SP,str);
} 
//
// Fetch the integer value in an Int object. Emits code to fetch the integer
// value of the Integer object pointed to by register source into the register dest
//
static void emit_fetch_int(const char *dest, const char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(const char *source, const char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }

static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(const char *source, ostream &s)
{
  if (strcmp(source, A1)) emit_move(A1, source, s);
  s << JAL << "_gc_check" << std::endl;
}


static void emit_func_start(ostream &str)
{
  // adds to stack
  emit_addiu(SP, SP, -12, str);
  emit_store(FP, 3, SP, str);
  emit_store(SELF, 2, SP, str);
  emit_store(RA, 1, SP, str);  
  
  // change fp and store prev return value in s0
  emit_addiu(FP, SP, 16, str);
  emit_move(SELF, ACC, str);
}

static void emit_func_end(ostream &str)
{
  // emit_move(ACC, SELF, str);
  // retrieve from stack
  emit_load(FP, 3, SP, str);
  emit_load(SELF, 2, SP, str);
  emit_load(RA, 1, SP, str);
  emit_addiu(SP, SP, 12, str);
  //jump return 
  str << RET << endl;
}


///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);
  
  // Add -1 eye catcher
  s << WORD << "-1" << std::endl;

  code_ref(s);
  s  << LABEL                                                               // label
     << WORD << stringclasstag << std::endl                                 // tag
     << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << std::endl // size
     << WORD;

  /***** Add dispatch information for class String ******/
  s << "String_dispTab";

  s << std::endl;                                              // dispatch table
  s << WORD;  lensym->code_ref(s);  s << std::endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag) {
  for (auto entry : tbl) {
    entry.code_def(s, stringclasstag);
  }
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << std::endl;

  code_ref(s);
  s << LABEL                                // label
    << WORD << intclasstag << std::endl                      // class tag
    << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << std::endl  // object size
    << WORD;

  /***** Add dispatch information for class Int ******/
  s << "Int_dispTab";
  s << std::endl;                                          // dispatch table
  s << WORD << str << std::endl;                           // integer value
}

//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag) {
  for (auto entry : tbl) {
    entry.code_def(s,intclasstag);
  }
}

//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}

//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << std::endl;

  code_ref(s);
  s << LABEL                                  // label
    << WORD << boolclasstag << std::endl                       // class tag
    << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << std::endl   // object size
    << WORD;
  
  s << "Bool_dispTab";
  /***** Add dispatch information for class Bool ******/

  s << std::endl;                                            // dispatch table
  s << WORD << val << std::endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//
// Define global names for some of basic classes and their tags.
//
void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << std::endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << std::endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << std::endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << std::endl;
  str << GLOBAL; falsebool.code_ref(str);  str << std::endl;
  str << GLOBAL; truebool.code_ref(str);   str << std::endl;
  str << GLOBAL << INTTAG << std::endl;
  str << GLOBAL << BOOLTAG << std::endl;
  str << GLOBAL << STRINGTAG << std::endl;


  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //

  str << INTTAG << LABEL
      << WORD << class_to_tag[Int] << std::endl;
  str << BOOLTAG << LABEL
      << WORD << class_to_tag[Bool] << std::endl;
  str << STRINGTAG << LABEL
      << WORD <<  class_to_tag[Str] << std::endl;
  // str << LABEL << " HI " << LABEL  << " HI "<< std::endl;
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << std::endl
      << HEAP_START << LABEL
      << WORD << 0 << std::endl
      << "\t.text" << std::endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << std::endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << std::endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << std::endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << std::endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << std::endl;
}

void CgenClassTable::code_bools()
{
  int boolclasstag = class_to_tag[Bool];
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

//
// Generate GC choice constants (pointers to GC functions)
//
void CgenClassTable::code_select_gc()
{
  str << GLOBAL << "_MemMgr_INITIALIZER" << std::endl;
  str << "_MemMgr_INITIALIZER:" << std::endl;
  str << WORD << gc_init_names[cgen_Memmgr] << std::endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << std::endl;
  str << "_MemMgr_COLLECTOR:" << std::endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << std::endl;
  str << GLOBAL << "_MemMgr_TEST" << std::endl;
  str << "_MemMgr_TEST:" << std::endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << std::endl;
}

//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//********************************************************
void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  // str << "Start of code_constants" << std::endl;
  stringtable.add_string("");
  inttable.add_string("0");
  // str << "before class tag" << std::endl;
  int stringclasstag = class_to_tag[Str];
  // str << "after class tag" << std::endl;
  int intclasstag = class_to_tag[Int];

  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools();
}

CgenClassTable::CgenClassTable(Classes classes, ostream& s) : str(s) {

  // make sure the various tables have a scope
  // class_to_tag_table.enterscope();

  enterscope();
  if (cgen_debug) std::cerr << "Building CgenClassTable" << std::endl;
  install_basic_classes();
  install_classes(classes);
  build_inheritance_tree();

  code();
  exitscope();
}


void CgenClassTable::install_basic_classes() {
  Symbol filename = stringtable.add_string("<basic class>");

  //
  // A few special class names are installed in the lookup table but not
  // the class list.  Thus, these classes exist, but are not part of the
  // inheritance hierarchy.
  // No_class serves as the parent of Object and the other special classes.
  // SELF_TYPE is the self class; it cannot be redefined or inherited.
  // prim_slot is a class known to the code generator.
  //
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
		     Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
		     Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
		     Basic,this));

  //
  // The Object class has no parent class. Its methods are
  //        cool_abort() : Object    aborts the program
  //        type_name() : Str        returns a string representation of class name
  //        copy() : SELF_TYPE       returns a copy of the object
  //
  // There is no need for method bodies in the basic classes---these
  // are already built in to the runtime system.
  //
  install_class(
   new CgenNode(
    class_(Object,
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(::copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

//
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
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
	   filename),
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer.
//
   install_class(
    new CgenNode(
     class_(Int,
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  the string's length
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//
   install_class(
    new CgenNode(
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
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
// The following possible errors are checked:
//       - a class called SELF_TYPE
//       - redefinition of a basic class
//       - redefinition of another previously defined class
//
void CgenClassTable::install_class(CgenNodeP nd) {
  Symbol name = nd->get_name();

  if (probe(name)) {
    return;
  }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds.push_front(nd);
  addid(name, nd);
}

void CgenClassTable::install_classes(Classes cs) {
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i), NotBasic, this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree() {
  for (auto nd : nds) {
    set_relations(nd);
  }
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd) {
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n) {
  children.push_front(n);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

CgenNodeP CgenNode::get_parentnd()
{
  return parentnd;
}

// void CgenClassTable::add_class_nameTab()

// void CgenClassTable::build_parent_map() {
//   parent_map = std::map<Symbol, std::vector<Symbol>>();
//   for (auto nd : nds) {
//     std::vector<Symbol> parent_of_nd;
//     CgenNodeP parent = nd->get_parentnd(nd);
//     while (parent != NULL) {
//       parent_of_nd.push_back(parent);
//       parent = parent->get_parentnd();
//     }
    
//     if (nd->get_name() != Object) {
//       inheritance_graph.push_back(nd);
//     }
//   }
// }

void CgenClassTable::assign_tags() {
  std::set<Symbol> already_assigned = {IO, Str, Int, Bool, Object};
  class_to_tag[Object] = 0; 
  class_to_tag[IO] = 1;
  class_to_tag[Int] = 2;
  class_to_tag[Bool] = 3;
  class_to_tag[Str] = 4;
  
  for (auto nd : nds) {
    Symbol name = nd->get_name();
    if (already_assigned.find(nd->get_name()) == already_assigned.end()) {
      class_to_tag[nd->get_name()] = tag_counter;
      tag_counter++;
    }
  }
}


void CgenClassTable::code()
{

    if (cgen_debug) std::cerr << "assigning the tags" << std::endl;
    assign_tags();

    if (cgen_debug) std::cerr << "coding global data" << std::endl;
    code_global_data();

    if (cgen_debug) std::cerr << "choosing gc" << std::endl;
    code_select_gc();

    if (cgen_debug) std::cerr << "coding constants" << std::endl;
    code_constants();

    //                 Add your code to emit
    //                   - prototype objects
    //                   - class_nameTab
    //                   - dispatch tables
    //
    // if (cgen_debug) std::cerr << "adding classes to table" << std::endl;
    // add_class_nameTab();
    // build_parent_map(); 
    // inheritance table represented as a list of nd, called nds. 
    // each nd in the list is a CLASS. each class has a parent and maybe children.
    // can use the parent and children to traverse up or down the inheritance class.
    if (cgen_debug) std::cerr << "making dispatch tables" << std::endl;
    make_dispatch_tables(str);

    if (cgen_debug) std::cerr << "making proto tables" << std::endl;
    make_protos();


    if (cgen_debug) std::cerr << "coding global text" << std::endl;
    code_global_text();

    //                 Add your code to emit
    //                   - object initializer
    //                   - the class methods
    //                   - etc...
    all_object_inits();
    all_funcs();

    // recursive_gen_code();
}

/*
Object_init:
  addiu	$sp $sp -12
	sw	$fp 12($sp)
	sw	$s0 8($sp)
	sw	$ra 4($sp)
	addiu	$fp $sp 16
	move	$s0 $a0
	move	$a0 $s0
	lw	$fp 12($sp)
	lw	$s0 8($sp)
	lw	$ra 4($sp)
	addiu	$sp $sp 12
	jr	$ra	
*/
int CgenNode::get_num_parents_attr() {
  int num_parents_attr = 0;
  Symbol curr = name;
  // cerr << curr << endl;
  CgenNode* curr_node = this;
  while (curr != Object && curr != No_class) {
    // num_parents_attr += class_->get_features()->len();
    for (int i = curr_node->features->first(); curr_node->features->more(i); i = curr_node->features->next(i)) {
      if (!curr_node->features->nth(i)->isMethod()) {
        num_parents_attr++;
      }
    }
    curr_node = curr_node->get_parentnd();
    curr = curr_node->name;
    // cerr << curr << endl;
  }
  return num_parents_attr;
}

void CgenNode::rec_proto(ostream& s) {
  if (name != Object) {
    parentnd->rec_proto(s);
  }
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    if (!features->nth(i)->isMethod()) {
      s << WORD;
      if (features->nth(i)->get_type_decl() == Int) {
        inttable.lookup_string("0")->code_ref(s);
      } else if (features->nth(i)->get_type_decl() == Str) {
        stringtable.lookup_string("")->code_ref(s);
      } else if (features->nth(i)->get_type_decl() == Bool) {
        falsebool.code_ref(s);
      } else {
        s << "0";
      }
      s << endl;
    }
  }
}

void CgenNode::make_prototype(ostream& s) {
  s << WORD << "-1" << endl;
  s << get_name() << "_protObj" << LABEL;
  s << WORD << class_to_tag[get_name()] << endl;
  int num_attributes = get_num_parents_attr();
  s << WORD << (DEFAULT_OBJFIELDS + num_attributes) << endl;
  s << WORD << get_name() << "_dispTab" << endl;
  rec_proto(s);
}

void CgenClassTable::make_protos() {
  for (auto nd: nds) {
    nd->make_prototype(str);
  }
}

void CgenNode::class_init_func(ostream& str) {
  str << name << "_init" << LABEL;
  emit_func_start(str);
  
  int par_attributes = parentnd->get_num_parents_attr();
  // cerr << "parent of " << name << " have " << par_attributes << " many attributes." << endl;
  int offset = 3 + par_attributes;
  // cerr << "parent of " << name << " is " << parentnd->name << endl;
  // int offset = 3;
  if (name != Object) {
    str << JAL << parent << "_init" << endl;
    for (int i = features->first(); features->more(i); i = features->next(i)) {
      if (!features->nth(i)->isMethod() && name != Str && name != Int && name != Bool && name != IO) {
        features->nth(i)->code(str);
        emit_store(ACC, offset, SELF, str);
        offset++;
      }
    }
  }
  
  emit_move(ACC, SELF, str);
  emit_func_end(str);
}

void CgenClassTable::all_object_inits() {
  for (auto nd: nds) {
    nd->class_init_func(str);
  }
}

void CgenClassTable::all_funcs() {
  for (auto nd: nds) {
    if (nd->getName() != Object && nd->getName() != Int && nd->getName() != Str && nd->getName() != Bool && nd->getName() != IO){
      // emit code for each method in the class (not inherited methods..
      for (int i = nd->get_features()->first(); nd->get_features()->more(i); i = nd->get_features()->next(i)) {
        if (nd->get_features()->nth(i)->isMethod()) {
          // emit code for method 
          str << nd->getName() << "." << nd->get_features()->nth(i)->getName() << LABEL;
          nd->get_features()->nth(i)->code(str);
          // str << "\t--------" << endl;
        }
      }
    }
  }
}

void CgenClassTable::recursive_gen_code() {
  for (auto nd: nds) {
    static_cast<Class_>(nd)->code(str);
  }
}  

void CgenNode::make_dispatch(ostream& s) {
  // recursion
  if (name != Object) {
    parentnd->make_dispatch(s);
  }
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    if (features->nth(i)->isMethod()) {
      s << WORD << name << "." << features->nth(i)->getName() << endl;
    }
  }
}

void CgenClassTable::make_dispatch_tables(ostream& s) {
  for (auto nd: nds) {
    s << nd->get_name() << "_dispTab" << LABEL;
    nd->make_dispatch(s);
  }
}


CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd,Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   basic_status(bstatus)
{
  // std::set<Symbol> bad_classes = {No_class, prim_slot, SELF_TYPE};
  if (strcmp(name->get_string(), "_no_class") && strcmp(name->get_string(), "_prim_slot") && strcmp(name->get_string(), "SELF_TYPE")) {
    // std::cerr<< "name: " << name->get_string() << std::endl;
    // std::cerr << Symbol(name->get_string()) << std::endl;
    stringtable.add_string(name->get_string());          // Add class name to string table
  }
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

void class__class::code(ostream &s) {
  // s << "reached a class " << name << endl;
  // cerr << "reached a class" << endl;
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    features->nth(i)->code(s);
  }
  
}

void method_class::code(ostream &s) {
  // s << "reached a method " << name << endl;
  
  emit_func_start(s);
  expr->code(s);
  emit_func_end(s);
}

void attr_class::code (ostream &s) {
  // s << "reached an attribute " << name << endl;
  init->code(s);
}

void branch_class::code(ostream &s) {
}

void assign_class::code(ostream &s) {
}

void static_dispatch_class::code(ostream &s) {
}

void dispatch_class::code(ostream &s) {
}

void cond_class::code(ostream &s) {
}

void loop_class::code(ostream &s) {
}

void typcase_class::code(ostream &s) {
}

void block_class::code(ostream &s) {
}

void let_class::code(ostream &s) {
}

void plus_class::code(ostream &s) {
  e1->code(s);
  emit_push(ACC, s);
  e2->code(s);
  emit_pop(T1, s);
  emit_fetch_int(T1, T1, s);
  emit_fetch_int(ACC, ACC, s);
  emit_add(ACC, ACC, T1, s);
  emit_store_int(ACC, ACC, s);
}

void sub_class::code(ostream &s) {
}

void mul_class::code(ostream &s) {
}

void divide_class::code(ostream &s) {
}

void neg_class::code(ostream &s) {
}

void lt_class::code(ostream &s) {
}

void eq_class::code(ostream &s) {
}

void leq_class::code(ostream &s) {
}

void comp_class::code(ostream &s) {
}

void int_const_class::code(ostream& s)
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(ostream& s)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s) {
}

void isvoid_class::code(ostream &s) {
}

void no_expr_class::code(ostream &s) {
}

void object_class::code(ostream &s) {
}

