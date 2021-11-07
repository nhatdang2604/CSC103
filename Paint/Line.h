#pragma once
#ifndef  _LINE_H_
#define _LINE_H_

#include "Figure.h"

namespace figure {
	class Line : public Figure {

	private: //private attribute
		Point _start;
		Point _end;

	public:	//getter and setter
		Point start() const;
		Point end() const;

	public:	//override method
		bool isIn(const RECT& rect) const override;
		std::string toString() const override;
		int type() const override;
		void draw(HDC& hdc) const override;
		std::shared_ptr<Figure> duplicate() const override;
		void move(const Point& vector) override;

	public: //static method
		static std::shared_ptr<Line> Line::parse(std::string&, std::string  = " ");

	public:	//Constructor and Destructor
		Line();
		Line(const Point& start, const Point& end);
		~Line() override;
	};
}

#endif // ! _LINE_H_

