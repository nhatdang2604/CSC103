#pragma once
#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>
#include <memory>
#include "Figure.h"

class Parser {
public:
	static std::shared_ptr<figure::Figure> parse(std::string&, std::string = ": ");
	static std::vector<std::shared_ptr<figure::Figure>> readFigure(const std::string&);
};

#endif // !_PARSER_H_





