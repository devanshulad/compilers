/*
 * The scanner definition for COOL.
 */
%option noyywrap
/*
 * Stuff enclosed in %{ %} in the first section is copied verbatim to the
 * output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include "cool-parse.h"

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;
static int num_comm_open = 0;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

%}

/*
 * Define names for regular expressions here.
 */

DARROW =>
DIGIT [0-9]
 /*IDENTIFIER_REST [a-zA-Z0-9_]* */
OBJID [a-z][a-zA-Z0-9_]*
 /* TYID  [A-Z][a-zA-Z0-9_]* */
%x COMMENT
%x INLINE_COMMENT
%x STRING

%%

<STRING><<EOF>> {
  cool_yylval.error_msg = "EOF in string constant";
  BEGIN 0;
  return ERROR;
}
 /*
<STRING>\\n {
  cool_yylval.error_msg = "Unmatched new line in string constant";
  BEGIN 0;
  return ERROR;
}
 */

<STRING>"\"" {
  /* strcpy(string_buf_ptr, "\0"); */
  cool_yylval.symbol = stringtable.add_string(string_buf);
  string_buf_ptr = string_buf;
  BEGIN 0;
  return STR_CONST;
}

"\"" { 
  BEGIN (STRING); 
  string_buf_ptr = string_buf;
}

<STRING>\[^nbtf.]

<STRING>[^"\""<<EOF>>"\n"] { 
  strcpy(string_buf_ptr, yytext); 
  *string_buf_ptr++; 
}
 /*
<STRING>[^"\""<<EOF>>]* {
  int len_string = strlen(yytext);
  strcpy(string_buf_ptr, yytext);
  *string_buf_ptr += len_string;
}
 */
 /*
  * "\""([^"\""]*)"\"" {
  *  cool_yylval.symbol = stringtable.add_string(yytext);
  *  return STR_CONST;
  * }
  */

"--" {BEGIN (INLINE_COMMENT);}
<INLINE_COMMENT>[\n] {
  curr_lineno ++;
  BEGIN 0;
  }
<INLINE_COMMENT><<EOF>> {BEGIN 0;}
<INLINE_COMMENT>. {}

"(*" {
  BEGIN(COMMENT);
  num_comm_open ++;
}
<COMMENT>[\n] {curr_lineno ++;}
<COMMENT>"*)" {
  num_comm_open --;
  if (num_comm_open == 0) {
    BEGIN 0;
  }
}
<COMMENT>"(*" {
  num_comm_open ++;
}
<COMMENT><<EOF>> {
    cool_yylval.error_msg = "EOF in comment";
    BEGIN 0;
    return ERROR;
}
<COMMENT>. {} 

"*)" {
  cool_yylval.error_msg = "Unmatched *)";
  BEGIN 0;
  return ERROR;
}

 /*
  * Normal comments
  */
 /* '--'.* */

 /*
  *  Nested comments
  */

 /* 
  * Object Identifiers.
  */
 /*{OBJID} { 
  cool_yylval.symbol = idtable.add_string(yytext);
  return (OBJECTID);
 */

 
 
 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  * Keyword Done List: Class, else, if, fi, in, inherits, isvoid, let,
  * loop, pool, new, of, not, then, while, case
  */
(?i:class)   { return (CLASS); }
(?i:else)    { return (ELSE); }
(?i:if)      { return (IF); }  
(?i:fi)      { return (FI); }  
(?i:in)      { return (IN); }  
(?i:inherits)      { return (INHERITS); }  
(?i:isvoid)      { return (ISVOID); }  
(?i:let)      { return (LET); }  
(?i:loop)      { return (LOOP); }  
(?i:pool)      { return (POOL); }  
(?i:then)      { return (THEN); }  
(?i:while)      { return (WHILE); }  
(?i:case)      { return (CASE); }  
(?i:esac)      { return (ESAC); }  
(?i:new)      { return (NEW); }  
(?i:of)      { return (OF); }  
(?i:not)      { return (NOT); }  

t(?i:rue)     { 
  cool_yylval.boolean = true;
  return (BOOL_CONST); 
}
f(?i:alse)    { 
  cool_yylval.boolean = false; 
  return (BOOL_CONST); 
}
  /* Need to do true and false. */
 /*
  *  String constants (C syntax, taken from lexdoc(1) )
  *  Escape sequence \c is accepted for all characters c. Except for
  *  \n \t \b \f, the result is c.
  *  (but note that 'c' can't be the NUL character)
  *
  */
[a-z][A-Za-z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return OBJECTID;
}


 /*
  * Type Identifiers.
  */
[A-Z][A-Za-z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return TYPEID;
}

 /*
  *  The multiple-character operators.
  */
{DARROW}    { return (DARROW); }

"<="        { return (LE); }

"<-"        { return (ASSIGN); }
 /* Integers. */
{DIGIT}+  { 
  cool_yylval.symbol = inttable.add_string(yytext);
  return (INT_CONST); /* INT_CONST */
}

[\n]    { curr_lineno++; }

" "          |
[\f\r\t\v]   {}

  /* ( ) ~ < ; , [ ] { } <- @ . : + - * / = (<= is le and <- is assi)*/
"("     |
")"     |
"~"     |
"<"     |
"="     |
":"     |
";"     |
","     |
"/"     |
"."     |
"{"     |
"}"     |
"@"     |
"/"     |
"*"     |
"+"     |
"-"     { return (int (*yytext)); }

[^\n]   {
  cool_yylval.error_msg = yytext;
  return ERROR;
}
%%


