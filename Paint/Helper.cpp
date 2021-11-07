#include "stdafx.h"
#include "Helper.h"

#include <iostream>



void Helper::standardlizeRect(RECT& rect) {
	if (rect.left > rect.right) std::swap(rect.left, rect.right);
	if (rect.top > rect.bottom) std::swap(rect.top, rect.bottom);
}

bool Helper::isNotMove(std::vector<std::shared_ptr<Shape>>& a, std::vector<std::shared_ptr<Shape>>& b) {
	if (a.size() != b.size()) return false;
	for (unsigned int i = 0; i <a.size(); ++i) {
		if (a[i]->figure->type() != b[i]->figure->type()) return false;
		if (a[i]->color != b[i]->color) return false;
		if (a[i]->cWidth != b[i]->cWidth) return false;
		if (a[i]->iStyle != b[i]->iStyle) return false;
		if (a[i]->figure->toString() != b[i]->figure->toString()) return false;
	}
	return true;
}