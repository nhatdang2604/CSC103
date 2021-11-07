#include "stdafx.h"
#include "Square.h"
#include "Figure.h"
#include "Tokenizer.h"

#include <vector>
#include <sstream>
#include <memory>

std::string figure::Square::toString() const {
	std::stringstream builder;
	builder << "Square" << ": " << _topLeft << " " << _bottomRight;
	return builder.str();
};
int figure::Square::type() const {
	return TYPE_SQUARE;
};

std::shared_ptr<figure::Figure> figure::Square::duplicate() const {
	return std::make_shared<figure::Square>(*this);
}

//No need, parent class already have
//void draw(HDC& hdc)
//bool isIn(const Rect& rect)
//void move(const Point&)


std::shared_ptr<figure::Square> figure::Square::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	Point topLeft = Point::parse(tokens[0]);
	Point bottomRight = Point::parse(tokens[1]);
	return std::make_shared<Square>(topLeft, bottomRight);
};

	
figure::Square::Square() {
		//do nothing
};

figure::Square::Square(const Point& topLeft, const Point& bottomRight) {
	_topLeft = topLeft;
	_bottomRight = bottomRight;
};

figure::Square::~Square() {
	//do nothing
};