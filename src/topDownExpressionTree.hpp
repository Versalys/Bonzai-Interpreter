#pragma once

#include "abstractSyntaxTree.hpp"
#include "tokenStream.hpp"
#include <vector>
#include <stdexcept>

Node parse_expression(TokenStream::iterator, TokenStream::iterator);


