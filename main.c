#include <stdio.h>
#include "9cc.h"

char *user_input;
Token *token;

int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "引数の個数が正しくありません");
    return 1;
  }

  user_input = argv[1];
  token = tokenize();
  Node *node = expr();

  printf(".global main\n");
  printf("main:\n\t");

  gen(node);

  printf("pop %%rax\n\t");
  printf("ret\n");
  return 0;
}
