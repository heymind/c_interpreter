

#ifndef C_INTERPRETER_LEX_H
#define C_INTERPRETER_LEX_H
#ifdef __cplusplus
extern "C" {
#endif
enum {
    TOK_EOF,
    TOK_INVALID,

    TOK_ELLIPSIS,
    TOK_RIGHT_ASSIGN,
    TOK_LEFT_ASSIGN,
    TOK_ADD_ASSIGN,
    TOK_SUB_ASSIGN,
    TOK_MUL_ASSIGN,
    TOK_DIV_ASSIGN,
    TOK_MOD_ASSIGN,
    TOK_AND_ASSIGN,
    TOK_XOR_ASSIGN,
    TOK_OR_ASSIGN,
    TOK_RIGHT_OP,
    TOK_LEFT_OP,
    TOK_INC_OP,
    TOK_DEC_OP,
    TOK_PTR_OP,
    TOK_AND_OP,
    TOK_OR_OP,
    TOK_LE_OP,
    TOK_GE_OP,
    TOK_EQ_OP,
    TOK_NE_OP,

    TOK_CONSTANT,
    TOK_STRING_LITERAL,


    TOK_AUTO = 200, // 为了避免和'+' , '-' 等 ascii 码片冲突
    TOK_BREAK,
    TOK_CASE,
    TOK_CHAR,
    TOK_CONST,
    TOK_CONTINUE,
    TOK_DEFAULT,
    TOK_DO,
    TOK_DOUBLE,
    TOK_ELSE,
    TOK_ENUM,
    TOK_EXTERN,
    TOK_FLOAT,
    TOK_FOR,
    TOK_GOTO,
    TOK_IF,
    TOK_INT,
    TOK_LONG,
    TOK_REGISTER,
    TOK_RETURN,
    TOK_SHORT,
    TOK_SIGNED,
    TOK_SIZEOF,
    TOK_STATIC,
    TOK_STRUCT,
    TOK_SWITCH,
    TOK_TYPEDEF,
    TOK_UNION,
    TOK_UNSIGNED,
    TOK_VOID,
    TOK_VOLATILE,
    TOK_WHILE,
    TOK_IDENTIFIER
};

typedef struct token_s {
    int tok;
    int len;
    char *ptr;
    int line_num; // 行号

} Tok;

typedef void *Lexer;

Lexer lex_init(const char *code);
Tok *lex_next(Lexer lexer);
#ifdef __cplusplus
}
#endif
#endif //C_INTERPRETER_LEX_H
