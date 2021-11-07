#include "stdafx.h"
#include "Circle.h"
#include "Tokenizer.h"

#include <sstream>
#include <vector>

std::string figure::Circle::toString() const {
	std::stringstream builder;
	builder << "Circle" << ": " << _topLeft << " " << _bottomRight;
	return builder.str();
};
int figure::Circle::type() const {
	return TYPE_CIRCLE;
};

std::shared_ptr<figure::Figure> figure::Circle::duplicate() const {
	return std::make_shared<figure::Circle>(*this);
}

//No need, parent class already have
//void draw(HDC& hdc)
//bool isIn(const Rect& rect)
//void move(const Point&)

	
std::shared_ptr<figure::Circle> figure::Circle::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	Point topLeft = Point::parse(tokens[0]);
	Point bottomRight = Point::parse(tokens[1]);
	return std::make_shared<Circle>(topLeft, bottomRight);
};

figure::Circle::Circle() {
	//do nothing
};

figure::Circle::Circle(const Point& topLeft, const Point& bottomRight) {
	_topLeft = topLeft;
	_bottomRight = bottomRight;
};

figure::Circle::~Circle() {
	//do nothing
};
