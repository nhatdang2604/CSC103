#pragma once
#ifndef _PEN_H_
#define _PEN_H

#include "HandleDraw.h"
#include "HandleEvent.h"
#include "Figure.h"
#include "Shape.h"

#include <vector>
#include <memory>
#include <windowsx.h>


class Pen {

private:	//private attribute

	//style
	int _iStyle;
	int _cWidth;
	COLORREF _color;

	//Style for drawing
	HPEN _hPen;

	//The current shape which would be drawed
	std::shared_ptr<figure::Figure> _figure;

public:	//Getter and setter
	void setFigure(std::shared_ptr<figure::Figure>);
	void setStyle(HDC&, int, int, COLORREF);

public:	//static method

	//draw all shape in shapes
	static void drawAll(HDC&, std::vector<std::vector<std::shared_ptr<Shape>>> = HandleEvent::states);
	static void drawAll(HDC&, std::vector<std::shared_ptr<Shape>>);

public:	//normal public method

	void draw(HDC& hdc);

	/*
	//Draw all shapes in states
	void drawAll(HDC&);*/

	//Save the drawn shape into states
	void saveShape() const;

public:	//constructor and destructor
	Pen();
	Pen(HDC&, int = PS_DASHDOT, int = 3, COLORREF = HandleDraw::color());
	~Pen();
};

#endif // !_PEN_H_