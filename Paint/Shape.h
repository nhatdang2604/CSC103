#pragma once
#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "Figure.h"
#include <memory>
#include <windef.h>
#include <vector>

struct Shape {

	//If use move/cut/delete (process make the old shape changed)
	//	=> executedFrom = the old shape
	std::shared_ptr<Shape> executedFrom;

	//style
	int iStyle;
	int cWidth;
	COLORREF color;

	//figure
	std::shared_ptr<figure::Figure> figure;

	//If (!isValid)
	//	=> cannot be drawn anymore
	bool isValid;

};

#endif // !_SHAPE_H_
