#include "stdafx.h"
#include <fstream>
#include <iostream>

#include "Parser.h"
#include "Tokenizer.h"
#include "Line.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Square.h"
#include "Circle.h"

std::shared_ptr<figure::Figure> Parser::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	try {
		if ("Line" == tokens[0]) return figure::Line::parse(tokens[1]);
		if ("Rectangle" == tokens[0]) return figure::Rectangle::parse(tokens[1]);
		if ("Ellipse" == tokens[0]) return figure::Ellipse::parse(tokens[1]);
		if ("Square" == tokens[0]) return figure::Square::parse(tokens[1]);
		if ("Circle" == tokens[0]) return figure::Circle::parse(tokens[1]);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return nullptr;
};

std::vector<std::shared_ptr<figure::Figure>> Parser::readFigure(const std::string& input) {
	std::ifstream device(input);
	std::string buffer;
	std::vector<std::shared_ptr<figure::Figure>> result;
	try {
		while (!device.eof()) {
			getline(device, buffer);
			std::shared_ptr<figure::Figure> figure = parse(buffer);
			if (figure) {
				result.push_back(figure);
			}
		}

		device.close();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return result;
};