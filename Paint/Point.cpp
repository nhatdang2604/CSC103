#include "stdafx.h"
#include "Point.h"
#include "Tokenizer.h"
#include <iostream>
#include <vector>
#include <string>

int figure::Point::x() const { return _x; };
int figure::Point::y() const { return _y; };
void figure::Point::setX(int x) { _x = x; };
void figure::Point::setY(int y) { _y = y; };

figure::Point figure::Point::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	int x = std::stoi(tokens[0]);
	int y = std::stoi(tokens[1]);
	return Point(x, y);
};

//Because << is friend method
namespace figure {
	std::ostream& operator <<(std::ostream& device, const figure::Point& object) {
		device << object._x << "," << object._y;
		return device;
	};
}

figure::Point::Point() {
	//do nothing
};

figure::Point::Point(int x, int y) {
	this->_x = x;
	this->_y = y;
};

figure::Point::~Point() {
	//do nothing
};
