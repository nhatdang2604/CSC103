#include "stdafx.h"
#include "Pen.h"

void Pen::setFigure(std::shared_ptr<figure::Figure> figure) {
	_figure = figure;
}

void Pen::setStyle(HDC& hdc, int iStyle, int cWidth, COLORREF color) {
	if (_hPen && (_iStyle != iStyle || _cWidth != cWidth || _color != color)) {
		DeleteObject(_hPen);
		_hPen = nullptr;
		_iStyle = iStyle;
		_cWidth = cWidth;
		_color = color;
		_hPen = CreatePen(iStyle, cWidth, color);
		SelectObject(hdc, _hPen);
	}
};

//draw all shape in shapes
void Pen::drawAll(HDC& hdc, std::vector<std::shared_ptr<Shape>> shapes) {
	std::vector<Pen> pens(shapes.size());
	for (unsigned int i = 0; i < shapes.size(); ++i) {
		pens[i]._figure = shapes[i]->figure;
		pens[i].setStyle(hdc, shapes[i]->iStyle, shapes[i]->cWidth, shapes[i]->color);
		if (shapes[i]->isValid) pens[i]._figure->draw(hdc);
	}
}

void Pen::drawAll(HDC& hdc, std::vector<std::vector<std::shared_ptr<Shape>>> states) {
	//calculate the true size
	int size = 0;
	for (unsigned int i = 0; i < states.size(); ++i) {
		size += states[i].size();
	}
	int index = -1;
	std::vector<Pen> pens(size);
	for (unsigned int i = 0; i < states.size(); ++i) {
		for (unsigned int j = 0; j < states[i].size(); ++j) {
			++index;
			pens[index]._figure = states[i][j]->figure;
			pens[index].setStyle(hdc, states[i][j]->iStyle, states[i][j]->cWidth, states[i][j]->color);
			if (states[i][j]->isValid) pens[index]._figure->draw(hdc);
		}
	}
}

void Pen::draw(HDC& hdc) {
	if (!_figure) return;
	_figure->draw(hdc);	
}

	/*
	//Draw all shapes in states
	void Pen::drawAll(HDC& hdc) {

	//Save current state
	int savedIStyle = _iStyle;
	int savedCWidth = _cWidth;
	COLORREF savedColor = _color;
	std::shared_ptr<figure::Figure> savedFigure = _figure;

	for (Shape i : states) {
	_figure = i.figure;
	setStyle(hdc, i.iStyle, i.cWidth, i.color);
	if (i.isValid)_figure->draw(hdc);
	};

	//Restore to current state
	_figure = savedFigure;
	setStyle(hdc, savedIStyle, savedCWidth, savedColor);
	}*/

	//Save the drawn shape into states
void Pen::saveShape() const {
	if (!_figure) return;
	Shape shape;
	shape.executedFrom = nullptr;
	shape.iStyle = _iStyle;
	shape.cWidth = _cWidth;
	shape.color = _color;
	shape.figure = _figure;
	shape.isValid = true;

	std::vector<std::shared_ptr<Shape>> state;
	state.push_back(std::make_shared<Shape>(shape));
	HandleEvent::states.push_back(state);
}

Pen::Pen() {
	//do nothing
}

Pen::Pen(HDC& hdc,
	int iStyle,
	int cWidth,
	COLORREF color) {

	_iStyle = iStyle;
	_cWidth = cWidth;
	_color = color;
	_figure = nullptr;
	_hPen = CreatePen(iStyle, cWidth, color);
	SelectObject(hdc, _hPen);
}


Pen::~Pen() {
	if (_hPen) DeleteObject(_hPen);
	_hPen = nullptr;	
}