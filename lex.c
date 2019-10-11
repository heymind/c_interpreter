#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lex.h"

#ifdef __cplusplus
extern "C" {
#endif

struct lexer_s {
    char *cur;
    Tok tok;
    int line_num;
};

Lexer lex_init(const char *code) {
    struct lexer_s *l = calloc(sizeof(struct lexer_s), 1);
    l->cur = code;
    return l;
}

static bool is_space(int c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t' || c == '\f' ||
           c == '\v';
}

static bool is_alpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_digit(int c) {
    return c >= '0' && c <= '9';
}

static int get_identifier(struct lexer_s *l) {
    l->cur++;
    while (l->cur[0] != '\0' && (is_alpha(*l->cur) || is_digit(*l->cur) || l->cur[0] == '_')) l->cur++;
    l->tok.len = l->cur - l->tok.ptr;
    l->cur--;
    return TOK_IDENTIFIER;
}

static int get_num(struct lexer_s *l) {
    strtod(l->cur, &l->cur);
    if (strchr("fFlLuU", *l->cur) != NULL) l->cur++;
    l->tok.len = l->cur - (char *) l->tok.ptr;
    l->cur--;
    return TOK_CONSTANT;
}

static int get_char(struct lexer_s *l) {
    l->cur++;
    if (l->cur[0] == '\\') l->cur++; // skip \0 ...
    if (l->cur[0] != '\0') l->cur++;
    if (l->cur[0] == '\'') {
        l->tok.len = l->cur - l->tok.ptr + 1;
        return TOK_CONSTANT;
    }
    return TOK_INVALID;
}

static int get_string_literal(struct lexer_s *l) {
    l->cur++;
    while (l->cur[0] != '\0' && l->cur[0] != '\"') {
        if (l->cur[0] == '\\' && l->cur[1] != '\0' &&
            (l->cur[1] == '\"' || strchr("bfnrtv\\", l->cur[1]) != NULL)) {
            l->cur += 2;
        } else {
            l->cur++;
        }
    }
    l->tok.len = l->cur - l->tok.ptr + 1;
    return TOK_STRING_LITERAL;
}

static int is_keyword(const char *s, int len) {
    const char *reserved[] =
            {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
             "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
             "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
             "void", "volatile", "while", NULL};
    if (!is_alpha(s[0])) return 0;
    for (int i = 0; reserved[i] != NULL; i++) {
        if (len == (int) strlen(reserved[i]) && strncmp(s, reserved[i], len) == 0)
            return i + TOK_AUTO;
    }
    return 0;
}

static void skip_spaces_and_comments(struct lexer_s *l) {
    const char *pos;
    do {
        pos = l->cur;
        while (is_space(l->cur[0])) {
            if (l->cur[0] == '\n') l->line_num++;
            l->cur++;
        }
        if (l->cur[0] == '/' && l->cur[1] == '/') {
            while (l->cur[0] != '\0' && l->cur[0] != '\n') l->cur++;
        }
        if (l->cur[0] == '/' && l->cur[1] == '*') {
            l->cur += 2;
            while (l->cur[0] != '\0') {
                if (l->cur[0] == '\n') l->line_num++;
                if (l->cur[0] == '*' && l->cur[1] == '/') {
                    l->cur += 2;
                    break;
                }
                l->cur++;
            }
        }
    } while (pos < l->cur);
}

static int get_symbol(struct lexer_s *l) {
    char *symbols[] = {"...", ">>=", "<<=", "+=", "-=", "*=", "/=", "%=", "&=", "^=", "|=", ">>", "<<", "++", "--",
                       "->", "&&", "||", "<=", ">=", "==", "!=", NULL};
    for (int i = 0; symbols[i] != NULL; i++) {
        int len = strlen(symbols[i]);
        if (strncmp(l->cur, symbols[i], len) == 0) {
            l->cur += len - 1;
            return i + TOK_ELLIPSIS;
        }
    }
    return 0;
}

static Tok *next(struct lexer_s *l) {
    int tok = TOK_INVALID, _tok;

    skip_spaces_and_comments(l);
    l->tok.ptr = l->cur;
    l->tok.line_num = l->line_num;

    if (*l->cur == '\0') {
        tok = TOK_EOF;
    } else if (is_digit(*l->cur)) {
        tok = get_num(l);
    } else if (*l->cur == '\'') {
        tok = get_char(l);
    } else if (*l->cur == '\"') {
        tok = get_string_literal(l);
    } else if (is_alpha(*l->cur) || l->cur[0] == '_') {
        tok = get_identifier(l);
        if ((_tok = is_keyword(l->tok.ptr, l->tok.len)) != 0) tok = _tok;
    } else if (strchr(";{},:=()[].&!~-+*/%<>^|?", *l->cur) != NULL) {
        tok = *l->cur;
        if ((_tok = get_symbol(l)) != 0) tok = _tok;
        l->tok.len = l->cur - l->tok.ptr + 1;
    }
    if (l->cur != '\0') l->cur++;
    l->tok.tok = tok;
    return &l->tok;

}

Tok *lex_next(Lexer l) {
    return next((struct lexer_s *) l);
}

#ifdef __cplusplus
}
#endif