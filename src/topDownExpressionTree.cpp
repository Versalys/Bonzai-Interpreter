/*
Expression tree for AST. May be integrated into main ast src. Maybe not. Please fucking kill me.
Written by a cool dude with a cool attitude.
*/

// use different functions to represent different rules. AKA one for statements, expressions, etc. to flesh out symantic rules.
// may make node structure a struct instead of a class because so many functs need access to members

// #include "tokenStream.hpp"
#include "abstractSyntaxTree.hpp"
#include "topDownExpressionTree.hpp"
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>



using std::map;
using std::vector;

map<string, int> operator_precedence = { // operator precedence table
  {"++", 1}, {"--", 1}, {".", 1}, {"CALL", 1},
  {"!", 2},
  {"*", 3}, {"/", 3}, {"%", 3},
  {"+", 4}, {"-", 4},
  {"<=", 5}, {"<", 5}, {">", 5}, {">=", 5},
  {"==", 6}, {"!=", 6},
  {"&", 7},
  {"|", 8}, 
  {"=", 9}, {"+=", 9}, {"-=", 9}, {"*=", 9}, {"/=", 9}, {"%=", 9}
};  // from first to last on evaluation

// enum eval {OPERATOR, LITERAL, IDENTIFIER};


int get_precedence(token &t){
  try{
    return operator_precedence.at(t.second);
  } catch(std::exception &e){
    std::cout << "Exception... " << e.what() << ':' << t.first << std::endl;
    exit(1);
  }
}

void print_out_stream(TokenStream::iterator begin, TokenStream::iterator end){
  std::cout << "=================================" << std::endl;
  for(auto i = begin; i!=end; i++) std::cout << ":: " << i->first << ':' << i->second << std::endl;
  std::cout << "---------------" << std::endl << std::endl;
}

Node parse_expression(TokenStream::iterator begin, TokenStream::iterator end){
  /* 
     bottom up eval on token stream. Eval lower operation them recursively go up the tree
     func calls not yet programmed. just see if identifier followed by '(' token
  */
  print_out_stream(begin, end);
  
  int paran=0;  // num paranthesis
  TokenStream::iterator t_comp = end;  // token to point to next highest operator
  for(auto t = begin; t != end; t++){
    if(t->second == "(") paran++;  // only eval if not in parenthesis because it changes order op
    else if(t->second == ")") paran--;
    else if(!paran && t->first == "OPERATOR" && (t_comp==end || get_precedence(*t_comp) < get_precedence(*t))){
      t_comp = t;
    }
  }

  if(paran) throw std::logic_error("MISSMATCHED PARENTHESIS");

  Node node;
  // ==== add support for unary operators ===
  if(t_comp==end){ // base case. No new operators so must be literal, identifier, or parenthesis
    if(begin+1 == end){ // literal/identifier

      if(begin->first == "NUM_LITERAL") {
	node.type = NUM_LITERAL;
	node.value = begin->second;
	return node;

      } else if(begin->first == "STR_LITERAL") {
	node.type = STR_LITERAL;
	node.value = begin->second;
	return node;

      } else if(begin->first == "IDENTIFIER"){
	node.type = IDENTIFIER;
	node.value = begin->second;
      } else throw std::logic_error("INVALID TOKEN FOR EXPRESSION TERMINATION " +
				    t_comp->first + ':' + t_comp->second);
      
    } else if(begin->second == "("){
      
      if((end-1)->second == ")"){
	return parse_expression(begin+1, end-1);
      } else throw std::logic_error("UNEXPECTED EXTRA TOKEN");
      
    } else throw std::logic_error("MISSING OPERAND");
    
  } else {
    node.value = t_comp->second;
    node.type = OPERATOR;
    node.branches.push_back(parse_expression(begin, t_comp));
    node.branches.push_back(parse_expression(t_comp+1, end));
  }
  return node;
}
