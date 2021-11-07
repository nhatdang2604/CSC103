#include "stdafx.h"
#include "Line.h"
#include "Tokenizer.h"

#include <sstream>

figure::Point figure::Line::start() const { return _start; };
figure::Point figure::Line::end() const { return _end; };

bool figure::Line::isIn(const RECT& rect) const {
	int topLeftX = _start.x();
	int bottomRightX = _end.x();
	if (topLeftX > bottomRightX)
		std::swap(topLeftX, bottomRightX);
	int topLeftY = _start.y();
	int bottomRightY = _end.y();
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

std::string figure::Line::toString() const {
	std::stringstream builder;
	builder << "Line" << ": " << _start << " " << _end;
	return builder.str();
};

int figure::Line::type() const {
	return TYPE_LINE;
};

void figure::Line::draw(HDC& hdc) const {
	MoveToEx(hdc, _start.x(), _start.y(), NULL);
	LineTo(hdc, _end.x(), _end.y());
}

std::shared_ptr<figure::Figure> figure::Line::duplicate() const {
	return std::make_shared<Line>(*this);
}

void figure::Line::move(const figure::Point& vector) {
	_start.setX(_start.x() + vector.x());
	_start.setY(_start.y() + vector.y());
	_end.setX(_end.x() + vector.x());
	_end.setY(_end.y() + vector.y());
}

	
std::shared_ptr<figure::Line> figure::Line::parse(std::string& buffer, std::string needle) {
	std::vector<std::string> tokens = Tokenizer::split(buffer, needle);
	Point start = Point::parse(tokens[0]);
	Point end = Point::parse(tokens[1]);
	return std::make_shared<Line>(start, end);
}

	
figure::Line::Line() {
	//do nothing
}

figure::Line::Line(const figure::Point& start, const figure::Point& end) {
	_start = start;
	_end = end;
}

figure::Line::~Line() {
	//do nothing
}
	