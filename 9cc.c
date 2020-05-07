#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  if(argc != 2){
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  char *p = argv[1];

  printf(".global main\n");
  printf("main:\n\t");
  printf("mov $%ld, %%rax\n\t", strtol(p, &p, 10));

  while(*p){
    if (*p == '+'){
      p++;
      printf("add $%ld, %%rax\n\t", strtol(p, &p, 10));
      continue;
    }

    if(*p == '-'){
      p++;
      printf("sub $%ld, %%rax\n\t", strtol(p, &p, 10));
      continue;
    }

    fprintf(stderr, "予期しない文字です: '%c'\n", *p);
    return 1;
  }

  printf("ret\n");
  return 0;
}
