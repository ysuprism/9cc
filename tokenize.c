#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include "9cc.h"

extern char *user_input;
extern Token *token;

void error(char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void error_at(char *loc, char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

static Token *new_token(TokenKind kind, Token *cur, char *str){
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

Token *tokenize(){
  char *p = user_input;
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while(*p){
    if(isspace(*p)){
      p++;
      continue;
    }

    if(strchr("+-*/()", *p)){
      cur = new_token(TK_RESERVED, cur, p++);
      cur->len = 1;
      continue;
    }

    if(isdigit(*p)){
      char *tmp = p;
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      cur->len = p - tmp;
      continue;
    }

    if((*p == '=' && *(p+1) == '=') ||
       (*p == '!' && *(p+1) == '=') ||
       (*p == '<' && *(p+1) == '=') ||
       (*p == '>' && *(p+1) == '=')){
      cur = new_token(TK_RESERVED, cur, p);
      cur->len = 2;
      p += 2;
      continue;
    }

    if(*p == '<' || *p == '>'){
      cur = new_token(TK_RESERVED, cur, p++);
      cur->len = 1;
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}
