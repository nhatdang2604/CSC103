#pragma once
#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

#include "Figure.h"

namespace figure {
	class Ellipse : public Figure {

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
		static std::shared_ptr<Ellipse> parse(std::string&, std::string = " ");

	public:	//constructor and destructor
		Ellipse();
		Ellipse(const Point&, const Point&);
		~Ellipse() override;
	};
}


#endif // !_ELLIPSE_H_



