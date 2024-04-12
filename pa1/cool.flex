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

%%

 /*
  *  Nested comments
  */


 /*
  *  The multiple-character operators.
  */
{DARROW}		{ return (DARROW); }

 /* Integers. */
{DIGIT}+  { 
  cool_yylval.symbol = inttable.add_string(yytext);
  return INT_CONST; /* INT_CONST */
}
 
 
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

[\n]    { curr_lineno++; }

%%

