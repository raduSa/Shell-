#ifndef _yy_defines_h_
#define _yy_defines_h_

#define WORD 257
#define GREAT 258
#define LESS 259
#define GREATGREAT 260
#define GREATAMPERSAND 261
#define PIPE 262
#define AMPERSAND 263
#define NEWLINE 264
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union YYSTYPE {
    char *string_val; /* For WORD tokens (filenames, arguments, commands) */
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;

#endif /* _yy_defines_h_ */
