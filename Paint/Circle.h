#pragma once
#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "Ellipse.h"

namespace figure {
	class Circle : public Ellipse {
	
	/*	Parent class already had public getters setters
	public:	//getter and setter
		Point topLeft() const { return _topLeft; };
		Point bottomRight() const { return _bottomRight; };
	*/

	public:	//override method

		std::string toString() const override;
		int type() const override;
		std::shared_ptr<Figure> duplicate() const override;

		//No need, parent class already have
		//void draw(HDC& hdc)
		//bool isIn(const Rect& rect)
		//void move(const Point&)

	public: //static method
		static std::shared_ptr<Circle> parse(std::string&, std::string = " ");

	public:	//constructor and destructor
		Circle();
		Circle(const Point&, const Point&);
		~Circle() override;
	};
}

#endif // !_CIRCLE_H_

