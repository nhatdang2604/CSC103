#pragma once
#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "Rectangle.h"

namespace figure {
	class Square : public Rectangle {

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
		static std::shared_ptr<Square> parse(std::string&, std::string = " ");

	public:	//constructor and destructor
		Square();
		Square(const Point&, const Point&);
		~Square() override;
	};
}
#endif // !



