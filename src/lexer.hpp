/*
Header for lexer
Written by a cool dude with a cool attitude
*/

#pragma once

#include <string>
#include <vector>
#include <utility>

using token = std::pair<std::string, std::string>;

std::vector<token> lex_text(std::string text);

// token find_alpha_token_value(std::string sequence);

// std::string read_in_word(const std::string &text, int &pos);

