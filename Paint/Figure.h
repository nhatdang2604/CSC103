#pragma once
#ifndef _FIGURE_H_
#define _FIGURE_H_

#include "Point.h"

#include <string>
#include <memory>
#include <windowsx.h>

//Type of figure which was drawn
#define TYPE_NULL 0
#define TYPE_ELLIPSE 1
#define TYPE_RECTANGLE 2
#define TYPE_LINE 3
#define TYPE_CIRCLE 4
#define TYPE_SQUARE 5

/* Interface for all figures */
namespace figure {
	class Figure {
	public:
		//Return information of a figure
		virtual std::string toString() const = 0;

		//Return type of figure
		virtual int type() const = 0;

		//Draw figure into a hdc
		virtual void draw(HDC&) const = 0;

		//return true if the figure is in the RECT
		virtual bool isIn(const RECT&) const = 0;

		//make a copy
		virtual std::shared_ptr<Figure> duplicate() const = 0;

		//The point parameter is now a vector (dx; dy)
		//	This method add dx for .x() and add dy for .y() of all points in a figure
		virtual void move(const Point&) = 0;
	public:
		virtual ~Figure() {
			//do nothing
		};
	};
}

#endif // !_FIGURE_H_

