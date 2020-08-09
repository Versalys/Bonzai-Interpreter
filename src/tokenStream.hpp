#pragma once
#include <vector>
#include "lexer.hpp"


class TokenStream{
  std::vector<token> token_list;
  int pos=0;
public:
  token next(){return token_list.at(pos++);}
  token peek(int n=0){return token_list.at(pos+n);}
  TokenStream()=default;

  TokenStream(std::vector<token> v){token_list = v;}
  
  using iterator = std::vector<token>::iterator;
  iterator begin(){return token_list.begin();}
  iterator end(){return token_list.end();}
};
