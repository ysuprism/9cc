#include <stdio.h>
#include "9cc.h"

void gen(Node *node){
  if(node->kind == ND_NUM){
    printf("push $%d\n\t", node->val);
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("pop %%rdi\n\t");
  printf("pop %%rax\n\t");

  switch(node->kind){
  case ND_ADD:
    printf("add %%rdi, %%rax\n\t");
    break;
  case ND_SUB:
    printf("sub %%rdi, %%rax\n\t");
    break;
  case ND_MUL:
    printf("imul %%rdi, %%rax\n\t");
    break;
  case ND_DIV:
    printf("cqo\n\t");
    printf("idiv %%rdi\n\t");
    break;
  case ND_EQL:
    printf("cmp %%rdi, %%rax\n\t");
    printf("sete %%al\n\t");
    printf("movzb %%al, %%rax\n\t");
    break;
  case ND_NEQL:
    printf("cmp %%rdi, %%rax\n\t");
    printf("setne %%al\n\t");
    printf("movzb %%al, %%rax\n\t");
    break;
  case ND_LT:
    printf("cmp %%rdi, %%rax\n\t");
    printf("setl %%al\n\t");
    printf("movzb %%al, %%rax\n\t");
    break;
  case ND_LE:
    printf("cmp %%rdi, %%rax\n\t");
    printf("setle %%al\n\t");
    printf("movzb %%al, %%rax\n\t");
    break;
  }

  printf("push %%rax\n\t");
}
