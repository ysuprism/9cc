#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "9cc.h"

void error_at(char *loc, char *fmt, ...);
extern Token *token;

static bool consume(char *op){
  if(token->kind != TK_RESERVED || 
     strlen(op) != token->len ||
     memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

static void expect(char *op){
  if(token->kind != TK_RESERVED || 
      strlen(op) != token->len ||
      memcmp(token->str, op, token->len))
    error_at(token->str, "'%c'ではありません", op);
  token = token->next;
}

static int expect_number(){
  if(token->kind != TK_NUM)
    error_at(token->str, "数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

static bool at_eof(){
  return token->kind == TK_EOF;
}

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

static Node *new_node_num(int val){
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

static Node *equality();
static Node *relational();
static Node *add();
static Node *mul();
static Node *unary();
static Node *primary();

Node *expr(){
  return equality();
}

static Node *equality(){
  Node *node = relational();

  for(;;){
    if(consume("=="))
      node = new_node(ND_EQL, node, relational());
    else if(consume("!="))
      node = new_node(ND_NEQL, node, relational());
    else
      return node;
  }
}

static Node *relational(){
  Node *node = add();

  for(;;){
    if(consume("<="))
      return new_node(ND_LE, node, add());
    else if(consume(">="))
      return new_node(ND_LE, add(), node);
    else if(consume("<"))
      return new_node(ND_LT, node, add());
    else if(consume(">"))
      return new_node(ND_LT, add(), node);
    else
      return node;
  }
}

static Node *add(){
  Node *node = mul();

  for(;;){
    if(consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if(consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

static Node *mul(){
  Node *node = unary();

  for(;;){
    if(consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if(consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

static Node *unary(){
  if(consume("+"))
    return primary();
  if(consume("-"))
    return new_node(ND_SUB, new_node_num(0), unary());
  return primary();
}

static Node *primary(){
  if(consume("(")){
    Node *node = expr();
    expect(")");
    return node;
  }

  return new_node_num(expect_number());
}
