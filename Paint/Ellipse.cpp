#include "stdafx.h"
#include "Ellipse.h"
#include "Tokenizer.h"
#include <sstream>

figure::Point figure::Ellipse::topLeft() const { return _topLeft; }
figure::Point figure::Ellipse::bottomRight() const { return _bottomRight; }

void drawEllipse(HDC& hdc, int left, int top, int right, int bottom) {
	Ellipse(hdc, left, top, right, bottom);
}

bool figure::Ellipse::isIn(const RECT& rect) const {
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

std::string figure::Ellipse::toString() const {
	std::stringstream builder;
	builder << "Ellipse" << ": " << _topLeft << " " << _bottomRight;
	return builder.str();
}

int figure::Ellipse::type() const {
	return TYPE_ELLIPSE;
}

void figure::Ellipse::draw(HDC& hdc) const {
	drawEllipse(hdc,
		_topLeft.x(),
		_topLeft.y(),
		_bottomRight.x(),
		_bottomRight.y()
	);
}

std::shared_ptr<figure::Figure> figure::Ellipse::duplicate() const {
	return std::make_shared<Ellipse>(*this);
}

void figure::Ellipse::move(const Point& vector) {
	_topLeft.setX(_topLeft.x() + vector.x());
	_topLeft.setY(_topLeft.y() + vector.y());
	_bottomRight.setX(_bottomRight.x() + vector.x());
	_bottomRight.setY(_bottomRight.y() + vector.y());
}
	
std::shared_ptr<figure::Ellipse> figure::Ellipse::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	Point topLeft = Point::parse(tokens[0]);
	Point bottomRight = Point::parse(tokens[1]);
	return std::make_shared<Ellipse>(topLeft, bottomRight);
}


figure::Ellipse::Ellipse() {
	//do nothing
}

figure::Ellipse::Ellipse(const figure::Point& topLeft, const figure::Point& bottomRight) {
	_topLeft = topLeft;
	_bottomRight = bottomRight;
}

figure::Ellipse::~Ellipse() {
	//do nothing
}
