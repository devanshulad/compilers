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
char *string_buf_ptr; /* A pointer to the last char in the current string */

extern int curr_lineno;
extern int verbose_flag;
static int num_comm_open = 0; /* keeps track of number of nested comment */
static bool is_maxed = false;

extern YYSTYPE cool_yylval;

bool test_max_string ();

%}

DARROW =>
DIGIT [0-9]
%x COMMENT
%x INLINE_COMMENT
%x STRING

%%

 /* String constant */
<STRING><<EOF>> {
  cool_yylval.error_msg = "EOF in string constant";
  BEGIN 0;
  string_buf_ptr = string_buf;
  return ERROR;
}
 /*
<STRING>\\n {
  cool_yylval.error_msg = "Unmatched new line in string constant";
  BEGIN 0;
  return ERROR;
}
 */
"\"" { 
  BEGIN (STRING); 
  is_maxed = false;
  /* printf ("i am a string \n"); */
  string_buf_ptr = string_buf;
}

<STRING>"\"" {
  /* strcpy(string_buf_ptr, "\0"); 
  
   do we need to add \0 here ?*/
  if (is_maxed) {
    is_maxed = false;
    BEGIN 0;
    string_buf_ptr = string_buf; 
    return STR_CONST;
  } else {
    if (test_max_string())
      return ERROR;

    *string_buf_ptr = '\0';
    cool_yylval.symbol = stringtable.add_string(string_buf);
    string_buf_ptr = string_buf;
    BEGIN 0;
    /*printf ("done with string \n"); */
    return STR_CONST;
  }
}

 /*
<STRING>'\n' {
  printf("REACHED new line compiler");
}
 */


<STRING>\\\n {
  if (test_max_string())
    return ERROR;
  /*printf("now text is %s", yytext);*/ 
  /* printf("in instance 1 with %s", yytext); */
  *string_buf_ptr++ = '\n';
  curr_lineno++;
}

<STRING>\n {
  /*   if (test_max_string())
    return ERROR; */
  cool_yylval.error_msg = "Unterminated string constant";
  curr_lineno++;
  string_buf_ptr = string_buf;
  BEGIN 0;
  return ERROR;
}
 /* need to check string length max in all of these maybe we can have a 
  helper fun or switch ? lowkey we can also just redo the same code block if 
  they dont grade for style. */

<STRING>"\\b" {
  if (test_max_string())
    return ERROR;
  *string_buf_ptr++ = '\b';
}

<STRING>"\\f" {
  if (test_max_string())
    return ERROR;
  *string_buf_ptr++ = '\f';
}

<STRING>"\\n" {
   /*printf("in instance 2 with %s", yytext); */
  if (test_max_string())
    return ERROR;
  *string_buf_ptr++ = '\n';
}


<STRING>"\\t" {
  if (test_max_string())
    return ERROR;
  *string_buf_ptr++ = '\t';
}

<STRING>\\[^b^f^n^t] { /* do we need \0 here ? */
   /*printf("reached line 127 which is %s", yytext); */
   /*if (strlen(yytext) > 1) { */
    /*printf("reached line 127 which is %s, %zu", yytext, strlen(yytext)); */
  if (test_max_string())
    return ERROR;
  *string_buf_ptr++ = yytext[1];
  /* } */
  /* curr_lineno++; */
}

 /* Should this be \\0 instead of \\\0 */
<STRING>"\0" {  /* I dont know how to make a test for this */
  cool_yylval.error_msg = "String contains null character";
  string_buf_ptr = string_buf;
  BEGIN 0;
  return ERROR;
}

<STRING>. { 
  if (test_max_string())
    return ERROR;
  strcpy(string_buf_ptr, yytext); 
  *string_buf_ptr++; 
} 

 /* Normal Comments */
"--" {BEGIN (INLINE_COMMENT);}
<INLINE_COMMENT>[\n] {
  curr_lineno ++;
  BEGIN 0;
  }
<INLINE_COMMENT><<EOF>> {BEGIN 0;}
<INLINE_COMMENT>. {}

 /* Nested Comments */
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
 
 /* Tokens for key words */
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

 /* Boolen operators */
t(?i:rue)     { 
  cool_yylval.boolean = true;
  return (BOOL_CONST); 
}
f(?i:alse)    { 
  cool_yylval.boolean = false; 
  return (BOOL_CONST); 
}

  /* Type Identifiers. */
[a-z][A-Za-z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return OBJECTID;
}

[A-Z][A-Za-z0-9_]* {
    cool_yylval.symbol = idtable.add_string(yytext);
    return TYPEID;
}

 /* The multiple-character operators. */
{DARROW}    { return (DARROW); }

"<="        { return (LE); }

"<-"        { return (ASSIGN); }

 /* Integers. */
{DIGIT}+  { 
  cool_yylval.symbol = inttable.add_string(yytext);
  return (INT_CONST); 
}

 /* Increment the line number for every new line */
[\n]    { curr_lineno++; }

 /* Escape tokens */
" "          |
[\f\r\t\v]   {}

 /* Special single char tokens */
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

 /* Catch can for all the errors */
[^\n]   {
  cool_yylval.error_msg = yytext;
  return ERROR;
}
%%

bool test_max_string () {
  if (string_buf_ptr >= string_buf + MAX_STR_CONST) {
    cool_yylval.error_msg = "String constant too long";
    string_buf_ptr = string_buf;
    BEGIN (STRING);
    is_maxed = true;
    return true;
  }
  return false;
}


