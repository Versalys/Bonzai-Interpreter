/* 
Lexer for Bonzai language interpreter
Written by a cool with a cool attitude

Edit exceptions to add line and col numbers
*/


#include <string>
#include <vector>
#include <utility>
#include <set>
#include <cctype>
#include <algorithm>
#include <map> // idiot
#include <iostream>
#include <stdexcept>

#include "lexer.hpp"

using std::string;
using std::pair;
using std::vector;
using std::set;


// fix to make const
std::map<string,set<string>> alpha_symbol_map = { // maps statements to token catagories
  {"KEYWORDS", {"if", "else", "for", "while", "print", "fun"}},
  {"LITERALS", {"true", "false"}}
};

// only first char to capture
set<char> operator_set =  {'+', '=', '-', '*', '/', '%','!', '&', '|'};
set<char> eq_operator_set = {'=', '+', '-', '*', '%', '/','!'}; // for operators with '=' suffix
set<char> separator_set = {'\n', '{', '}', '(', ')', ';', ','};


token read_in_operator(const string &text, int &pos){
  /* Reads in operator denoted by operator_set */
  string r_str;
  if(text[pos] != '=' && text[pos+1] == '='){
    r_str += text.substr(pos, 2);
    pos++;
    return std::make_pair("OPERATOR",r_str);
  }
  if(text[pos+1] == '=' && eq_operator_set.find(text[pos]) != eq_operator_set.end()){
    r_str += text.substr(pos, 2);
    pos++;
    return std::make_pair("OPERATOR",r_str);
  }
  // throw if illegal operator
  if(operator_set.find(text[pos+1])==operator_set.end()) return std::make_pair("OPERATOR",string(1,text[pos]));

  throw std::logic_error(string("NO SUCH OPERATOR '")+text.substr(pos,2)+"'");
}


token find_alpha_token_value(string sequence){ 
  /* finds alpha token value */
    if(alpha_symbol_map["KEYWORDS"].find(sequence) != alpha_symbol_map["KEYWORDS"].end())
      return std::make_pair("KEYWORDS", sequence);
	if(alpha_symbol_map["LITERALS"].find(sequence) != alpha_symbol_map["LITERALS"].end())
	  return std::make_pair("BOOL_LITERAL", sequence);
	return std::make_pair("IDENTIFIER", sequence);
}


string read_in_word(const string &text, int &pos){
  /* read in word as long as isalphanum is true */
  string r_str;
  for(; pos<text.size(); pos++){
    if(isalnum(text[pos]) || text[pos]=='_'){
      r_str += text[pos];
    } else{
      pos--;
      break;
    }
  }
  return r_str;
}


token read_in_num_literal(const string &text, int &pos){
  /* Read in number denoted bu isdigit() until isdigit returns false (excluding '.') */
  string r_str;
  
  for(; pos<text.size(); pos++){
    if(isdigit(text[pos]) || text[pos] == '.'){
      r_str += text[pos];
    } else{
      pos--;
      break;
    }
  }
  return std::make_pair("NUM_LITERAL", r_str);
}


token read_in_string_literal(const string &text, int &pos){
  /* Read in string denoted by " as a literal until closing " is reached */
  string r_str = "";
  pos++; // skip first quote mark
  for(; pos<text.size(); pos++){
    if(text[pos] == '"'){
      if(text[pos-1] != '\\') return std::make_pair("STR_LITERAL", r_str);
    }
    r_str += text[pos];
  }
  throw std::logic_error("NO MATCHING \"");
}


vector<token> lex_text(string text){
  /* lexes text and returns a token map */
  vector<token> token_list;
  string tmp;
  for(int pos=0; pos<text.size(); pos++){
    token single_token;
    if (text[pos] == ' ' || text[pos] == '\t'){  // ignore whitespace
      continue;
    } else if(isalpha(text[pos])){ // alpha literals, keywords, identifiers
      tmp = read_in_word(text, pos);
      single_token = find_alpha_token_value(tmp);
    } else if(isdigit(text[pos])){  // numerical literals
      single_token = read_in_num_literal(text, pos);
    } else if(text[pos] == '"'){  // string literals
      single_token = read_in_string_literal(text, pos);
    } else if (operator_set.find(text[pos])!=operator_set.end()){
      single_token = read_in_operator(text, pos);
    } else if (separator_set.find(text[pos])!=separator_set.end()){
      single_token = std::make_pair("SEPARATORS", string(1, text[pos]));
      // pos++; // find better way
    } else {  // illegal catch all
      throw std::logic_error(string("UNEXPECTED CHAR '")+text.substr(pos,1)+"'");
    }
    // pos--;
    // std::cout << single_token.first << ':' << single_token.second << std::endl;
    token_list.push_back(single_token);
  }
  return token_list;
}
