#include "stdafx.h"
#include "Rectangle.h"
#include "Tokenizer.h"

#include <windowsx.h>
#include <sstream>

void drawRectangle(HDC& hdc, int left, int top, int right, int bottom) {
	Rectangle(hdc, left, top, right, bottom);
}

figure::Point figure::Rectangle::topLeft() const { return _topLeft; };
figure::Point figure::Rectangle::bottomRight() const { return _bottomRight; };

bool figure::Rectangle::isIn(const RECT& rect) const {
	int topLeftX = _topLeft.x();
	int bottomRightX = _bottomRight.x();
	if (topLeftX > bottomRightX)
		std::swap(topLeftX, bottomRightX);
	int topLeftY = _topLeft.y();
	int bottomRightY = _bottomRight.y();
	if (topLeftY > bottomRightY)
		std::swap(topLeftY, bottomRightY);

	if (topLeftX >= rect.left &&
	bottomRightX <= rect.right &&
	topLeftY >= rect.top &&
	bottomRightY <= rect.bottom) {
		return true;
	}

	return false;
	}

std::string figure::Rectangle::toString() const {
	std::stringstream builder;
	builder << "Rectangle" << ": " << _topLeft << " " << _bottomRight;
	return builder.str();
};

int figure::Rectangle::type() const {
	return TYPE_RECTANGLE;
};

void figure::Rectangle::draw(HDC& hdc) const {
	drawRectangle(hdc,
		_topLeft.x(),
		_topLeft.y(),
		_bottomRight.x(),
		_bottomRight.y()
	);
}

std::shared_ptr<figure::Figure> figure::Rectangle::duplicate() const {
	return std::make_shared<Rectangle>(*this);
}

void figure::Rectangle::move(const Point& vector)  {
	_topLeft.setX(_topLeft.x() + vector.x());
	_topLeft.setY(_topLeft.y() + vector.y());
	_bottomRight.setX(_bottomRight.x() + vector.x());
	_bottomRight.setY(_bottomRight.y() + vector.y());
}

	
std::shared_ptr<figure::Rectangle> figure::Rectangle::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	Point topLeft = Point::parse(tokens[0]);
	Point bottomRight = Point::parse(tokens[1]);
	return std::make_shared<Rectangle>(topLeft, bottomRight);
};

	
figure::Rectangle::Rectangle() {
	//do nothing
};

figure::Rectangle::Rectangle(const Point& topLeft, const Point& bottomRight) {
	_topLeft = topLeft;
	_bottomRight = bottomRight;
};

figure::Rectangle::~Rectangle() {
	//do nothing
};