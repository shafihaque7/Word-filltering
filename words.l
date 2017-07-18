%top{
# include <stdio.h>
}

LET [A-Za-z_]
DIG [0-9]
WS  [ \t\r\n]
PUNC    [~‘!@#$ %^&*()_+-={}|\\\[\]:;\"\’<>,./?]

%%

{LET}+              { return  0; }
{LET}+"’"({LET}|{LET}{LET}) { return  0; }
{LET}+{DIG}+            { return  0; }
{LET}+{DIG}+{LET}   {   return 0;}
{PUNC}+             {}
{WS}+               {}
<<EOF>>             { return -1; }
.               {}

%%

int freeScanner(void) { yylex_destroy(); yyterminate(); }
// int main ( void ) { int res = yylex (); printf ("% d - %s", res , yytext );
