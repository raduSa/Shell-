#ifndef _yy_defines_h_
#define _yy_defines_h_

#define WORD 257
#define GREAT 258
#define LESS 259
#define GREATGREAT 260
#define GREATAMPERSAND 261
#define GREATGREATAMPERSAND 262
#define PIPE 263
#define AMPERSAND 264
#define NEWLINE 265
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
    char *string_val;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
