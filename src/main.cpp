/*
My interpreter

Written by a cool dude with a cool attitude.

Lexer                   [X]
parser
AST
Bytecode-compiler
bytecode interpreter
runtime
*/

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "lexer.hpp"
#include "topDownExpressionTree.hpp"

void print_tree(Node &node, int offset=0){
  for(int i=0; i<offset; i++) std::cout << '\t';
  std::cout << '{' << node.value << ':' << node.type  << "}" << std::endl;
  
  for(auto &i : node.branches){
    print_tree(i, offset+1);
    std::cout << std::endl;
  }
  
  // for(int i=0; i<offset;i++) std::cout << '\t';
  // std::cout << '}' << std::endl;
}

int main(){
  std::ifstream fs("testcode.txt");
  std::ostringstream raw_oss;
  raw_oss << fs.rdbuf();
  TokenStream t_s = lex_text(raw_oss.str());

  Node head;
  
  head = parse_expression(t_s.begin(), t_s.end()-1);

  print_tree(head);
  // std::cout << OPERATOR;
}
