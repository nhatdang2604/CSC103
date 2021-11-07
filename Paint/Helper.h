#pragma once
#ifndef _HELPER_H_
#define _HELPER_H_

#include "Shape.h"
#include <vector>
#include <memory>

class Helper {
public:
	//Swap top, left, right, down of a rect in correct position
	static void standardlizeRect(RECT&);

	//return true if all shapes in b are the same position compare to all the shapes in a
	static bool Helper::isNotMove(std::vector<std::shared_ptr<Shape>>&, std::vector<std::shared_ptr<Shape>>&);
};


#endif // !_HELPER_H_


