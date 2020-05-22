typedef enum{
  TK_RESERVED,
  TK_NUM,
  TK_EOF
}TokenKind;

typedef struct Token Token;
struct Token{
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

typedef enum{
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM,
  ND_EQL,
  ND_NEQL,
  ND_LT,
  ND_LE,
}NodeKind;

typedef struct Node Node;
struct Node{
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

Token *tokenize();
Node *expr();
void gen(Node *node);
