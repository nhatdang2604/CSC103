#pragma once
#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Figure.h"

namespace figure {
	class Rectangle : public Figure {

	protected://protected attribute
		Point _topLeft;
		Point _bottomRight;

	public:	//getter and setter
		Point topLeft() const;
		Point bottomRight() const;

	public:	//override method
		bool isIn(const RECT&) const override;
		std::string toString() const override;
		int type() const override;
		void draw(HDC&) const override;
		std::shared_ptr<Figure> duplicate() const override;
		void move(const Point&) override;

	public: //static method
		static std::shared_ptr<Rectangle> parse(std::string&, std::string = " ");

	public:	//constructor and destructor
		Rectangle();
		Rectangle(const Point&, const Point&);
		~Rectangle() override;
	};
}


#endif // !_RECTANGLE_H_


