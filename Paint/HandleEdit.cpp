#include "stdafx.h"
#include "HandleEdit.h"
#include "HandleEvent.h"
#include "HandleDraw.h"
#include "Pen.h"
#include "Helper.h"

#include <windowsx.h>

//Init static variable
std::vector<std::vector<std::shared_ptr<Shape>>> HandleEdit::_redoStates;
std::vector<std::shared_ptr<Shape>> HandleEdit::_clipboard;
RECT HandleEdit::_rcSelectCopy;
int HandleEdit::_undoStartIndex = -1;

std::vector<std::shared_ptr<Shape>> HandleEdit::clipboard() {
	return _clipboard;
}

void HandleEdit::setUndoStartIndex(int value) {
	_undoStartIndex = value;
}

void HandleEdit::traversalDraggedShapes(void(*process)(int, std::vector<std::shared_ptr<Shape>>&), std::vector<std::shared_ptr<Shape>>& shapes) {
	for (unsigned int i = 0; i < HandleDraw::_draggedShapes.size(); ++i) {
		process(i, shapes);
	}
}

void HandleEdit::clearRedoStates() {
	if (!_redoStates.empty()) {
		_redoStates.clear();
	}
}

void HandleEdit::modeDelete(HDC& hdc, HDC& memDC, int mode) {
	if (MODE_DELETE != mode) return;

	//If a shape is delete, it would have:
	//	- A duplicate in the top of states, which have isValid = false, and is executedFrom = &original
	//a duplicate is make for undoing/redoing
	//	- set the orginal one's isValid = false
	auto deleteCheck = [](int i, int j, std::vector<std::shared_ptr<Shape>>& _draggedShapes) {
		
		_draggedShapes.push_back(std::make_shared<Shape>(*HandleEvent::states[i][j]));
		_draggedShapes.back()->executedFrom = HandleEvent::states[i][j];
		_draggedShapes.back()->figure = HandleEvent::states[i][j]->figure->duplicate();
		_draggedShapes.back()->isValid = false;

		//invalidate the selected shape (which mean this cannot be drawn)
		// We do not delete the old seletected shape because of undoing/redoing
		HandleEvent::states[i][j]->isValid = false;
	};


	//Check if there are some dragged shapes before
	if (HandleDraw::_draggedShapes.empty()) {
		HandleDraw::traversalStates(deleteCheck, HandleDraw::_draggedShapes);
		Pen::drawAll(memDC);
		Pen::drawAll(memDC, HandleDraw::_draggedShapes);
	}
	else {
		for (unsigned int i = 0; i < HandleDraw::_draggedShapes.size(); ++i) {
			HandleDraw::_draggedShapes[i]->isValid = false;
		}
		Pen::drawAll(memDC);
		Pen::drawAll(memDC, HandleDraw::_draggedShapes);
	}

	HandleEvent::states.push_back(HandleDraw::_draggedShapes);
	HandleDraw::_draggedShapes.clear();

	//Copy data from memoryDC -> hdc
	BitBlt(hdc, 0, 0,
		HandleEvent::rcHWND.right - HandleEvent::rcHWND.left,
		HandleEvent::rcHWND.bottom - HandleEvent::rcHWND.top,
		memDC, 0, 0, SRCCOPY);
}

void HandleEdit::modeCopy(HDC& hdc, HDC& memDC, int mode) {
	if (MODE_COPY != mode) return;

	//clear clipboard first
	_clipboard.clear();

	//Save the rcSelect position
	_rcSelectCopy.left = HandleEvent::rcSelect.left;
	_rcSelectCopy.top = HandleEvent::rcSelect.top;
	_rcSelectCopy.right = HandleEvent::rcSelect.right;
	_rcSelectCopy.bottom = HandleEvent::rcSelect.bottom;
	Helper::standardlizeRect(_rcSelectCopy);

	auto getOriginal_nonEmpty = [](int i, std::vector<std::shared_ptr<Shape>>& clipboard) {

		//Save the shape to _draggedShapes
		clipboard.push_back(std::make_shared<Shape>(*HandleDraw::_draggedShapes[i]));
		clipboard.back()->figure = HandleDraw::_draggedShapes[i]->figure->duplicate();
	};

	auto getOriginal_empty = [](int i, int j, std::vector<std::shared_ptr<Shape>>& clipboard) {
		//Save the shape to clipboard
		clipboard.push_back(std::make_shared<Shape>(*HandleEvent::states[i][j]));
		clipboard.back()->figure = HandleEvent::states[i][j]->figure->duplicate();
	};

	if (HandleDraw::_draggedShapes.empty()) {
		HandleDraw::traversalStates(getOriginal_empty, _clipboard);
	}
	else {
		traversalDraggedShapes(getOriginal_nonEmpty, _clipboard);
	}

	for (std::shared_ptr<Shape>i : _clipboard) {
		i->executedFrom = nullptr;
	}
}


void HandleEdit::modeCut(HDC& hdc, HDC& memDC, int mode) {
	if (MODE_CUT != mode) return;

	//clear clipboard first
	_clipboard.clear();

	//Save the rcSelect position
	_rcSelectCopy.left = HandleEvent::rcSelect.left;
	_rcSelectCopy.top = HandleEvent::rcSelect.top;
	_rcSelectCopy.right = HandleEvent::rcSelect.right;
	_rcSelectCopy.bottom = HandleEvent::rcSelect.bottom;
	Helper::standardlizeRect(_rcSelectCopy);


	////Process when _draggedShapes is not empty: we are dragging, stop and copy
	//- Get save and changed isValid = false to all shapes which are in rcSelect
	//- Note: no need to store for redoing/undoing
	auto getOriginal_nonEmpty = [](int i, std::vector<std::shared_ptr<Shape>>& clipboard) {
		clipboard.push_back(std::make_shared<Shape>(*HandleDraw::_draggedShapes[i]));
		clipboard.back()->figure = HandleDraw::_draggedShapes[i]->figure->duplicate();

		//invalidate the selected shape (which mean this cannot be drawn)
		// We do not delete the old seletected shape because of undoing/redoing
		HandleDraw::_draggedShapes[i]->isValid = false;
	};

	//Process when _draggedShapes is empty: drag-after-draw or after selecting
	//- Get save and changed isValid = false to all shapes which are in rcSelect
	auto getOriginal_empty_save = [](int i, int j, std::vector<std::shared_ptr<Shape>>& save) {
		
		//dummy gonna push back to states later
		save.push_back(std::make_shared<Shape>(*HandleEvent::states[i][j]));
		save.back()->figure = HandleEvent::states[i][j]->figure->duplicate();
		save.back()->executedFrom = HandleEvent::states[i][j];
		save.back()->isValid = false;
		
		//Cut the original
		HandleEvent::states[i][j]->isValid = false;
	};

	//Process when _draggedShapes is empty: drag-after-draw or after selecting
	//- Get data from states to clipboard
	auto getOriginal_empty_clipboard = [](int i, int j, std::vector<std::shared_ptr<Shape>>& clipboard) {

		//dummy gonna push back to states later
		clipboard.push_back(std::make_shared<Shape>(*HandleEvent::states[i][j]));
		clipboard.back()->figure = HandleEvent::states[i][j]->figure->duplicate();
	};

	

	if (HandleDraw::_draggedShapes.empty()) {
		std::vector<std::shared_ptr<Shape>> save;
		HandleDraw::traversalStates(getOriginal_empty_clipboard, _clipboard);
		HandleDraw::traversalStates(getOriginal_empty_save, save);
		HandleEvent::states.push_back(save);

	}
	else {
		
		traversalDraggedShapes(getOriginal_nonEmpty, _clipboard);
	}

	
	for (std::shared_ptr<Shape>i : _clipboard) {
		i->executedFrom = nullptr;
	}

	Pen::drawAll(memDC);

	//Copy data from memoryDC -> hdc
	BitBlt(hdc, 0, 0,
		HandleEvent::rcHWND.right - HandleEvent::rcHWND.left,
		HandleEvent::rcHWND.bottom - HandleEvent::rcHWND.top,
		memDC, 0, 0, SRCCOPY);

}

void HandleEdit::modePaste(HDC& hdc, HDC& memDC, int mode) {
	if (MODE_PASTE != mode) return;
	if (_clipboard.empty()) return;

	//Drop all the last dragged shapes
	HandleDraw::finishToDrag(HandleEvent::states);

	//push back the clipboard into states
	for (std::shared_ptr<Shape> i : _clipboard) {
		HandleDraw::_draggedShapes.push_back(std::make_shared<Shape>(*i));
		HandleDraw::_draggedShapes.back()->figure = i->figure->duplicate();
	}

	// Pen to draw rcSelected
	Pen pen(memDC);

	//Draw all
	Pen::drawAll(memDC);

	//Draw rcSelect
	pen.setStyle(memDC, PS_DASHDOT, 1, RGB(0, 0, 255));
	HandleDraw::drawIntoMemDC(memDC, pen, TYPE_RECTANGLE,
		_rcSelectCopy.left, _rcSelectCopy.top,
		_rcSelectCopy.right, _rcSelectCopy.bottom);

	//Save the rcSelect position
	HandleEvent::rcSelect.left = _rcSelectCopy.left;
	HandleEvent::rcSelect.top = _rcSelectCopy.top;
	HandleEvent::rcSelect.right = _rcSelectCopy.right;
	HandleEvent::rcSelect.bottom = _rcSelectCopy.bottom;

	//Draw everything in draggedShapes
	Pen::drawAll(memDC, HandleDraw::_draggedShapes);

	//Copy data from memoryDC -> hdc
	BitBlt(hdc, 0, 0,
		HandleEvent::rcHWND.right - HandleEvent::rcHWND.left,
		HandleEvent::rcHWND.bottom - HandleEvent::rcHWND.top,
		memDC, 0, 0, SRCCOPY);

}

void HandleEdit::modeUndo(HDC& hdc, HDC& memDC, int mode) {
	if (MODE_UNDO != mode) return;
	if (HandleEvent::states.empty()) return;

	int endPos = HandleEvent::states.size() - 1;
	if (endPos < _undoStartIndex) return;

	std::vector<std::shared_ptr<Shape>> latestShapes = HandleEvent::states.back();	//save the latest shape which are in states
	_redoStates.push_back(latestShapes);							//pop them into redo states
	HandleEvent::states.pop_back();


	//validate if there are some executed-from-shapes which are refered to latest shapes
	for (std::shared_ptr<Shape> i : latestShapes) {
		if (i->executedFrom) {
			i->executedFrom->isValid = true;
		}
	}

	//Clear everything is dragging (because it's still drawn if we were in Drag mode)
	if (!HandleDraw::_draggedShapes.empty()) {
		HandleDraw::_draggedShapes.clear();
	}

	//Draw all
	Pen::drawAll(memDC);

	//Copy data from memoryDC -> hdc
	BitBlt(hdc, 0, 0,
		HandleEvent::rcHWND.right - HandleEvent::rcHWND.left,
		HandleEvent::rcHWND.bottom - HandleEvent::rcHWND.top,
		memDC, 0, 0, SRCCOPY);
}

void HandleEdit::modeRedo(HDC& hdc, HDC& memDC, int mode) {
	if (MODE_REDO != mode) return;
	if (_redoStates.empty()) return;

	std::vector<std::shared_ptr<Shape>> latestUndoShapes = _redoStates.back();	//get the latest shape which are in _redoStates
	_redoStates.pop_back();
	HandleEvent::states.push_back(latestUndoShapes);

	//Invalidate if there are some executed-from-shapes which are refered to latest shapes
	for (std::shared_ptr<Shape> i : latestUndoShapes) {
		if (i->executedFrom) {
			i->executedFrom->isValid = false;
		}
	}

	//Clear everything is dragging (because it's still drawn if we were in Drag mode)
	if (!HandleDraw::_draggedShapes.empty()) {
		HandleDraw::_draggedShapes.clear();
	}

	//Draw all
	Pen::drawAll(memDC);

	//Copy data from memoryDC -> hdc
	BitBlt(hdc, 0, 0,
		HandleEvent::rcHWND.right - HandleEvent::rcHWND.left,
		HandleEvent::rcHWND.bottom - HandleEvent::rcHWND.top,
		memDC, 0, 0, SRCCOPY);
}

void HandleEdit::activateDelete(HWND& hwnd, int& mode) {

	//Only use while in mode drag
	if (MODE_DRAG == mode) {

		//end drag mode temporaly
		//start delete mode
		mode = MODE_DELETE;

		//Clear redo 
		HandleEdit::clearRedoStates();

		InvalidateRect(hwnd, &HandleEvent::rcHWND, FALSE);
		UpdateWindow(hwnd);

		//Update delete states
		//HandleDraw::finishToDrag(states);

		//Disable Cut/Copy
		if (HandleEvent::isCutAndCopyOn) {
			SendMessage(HandleEvent::hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(0, 0));
			SendMessage(HandleEvent::hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(0, 0));
			HandleEvent::isCutAndCopyOn = false;
		}


	}
}

void HandleEdit::activateCopy(HWND& hwnd, int& mode) {
	//Only use while in mode drag
	if (MODE_DRAG == mode) {

		//end drag mode temporaly
		//start copy mode
		mode = MODE_COPY;

		//Change color immediately
		InvalidateRect(hwnd, &HandleEvent::rcHWND, FALSE);
		UpdateWindow(hwnd);

		//restart mode drag
		mode = MODE_DRAG;
	}
}

void HandleEdit::activateCut(HWND& hwnd, int& mode) {

	//Only use while in mode drag
	if (MODE_DRAG == mode) {

		//end drag mode temporaly
		//start cut mode
		mode = MODE_CUT;

		//Clear redo 
		HandleEdit::clearRedoStates();

		//Change color immediately
		InvalidateRect(hwnd, &HandleEvent::rcHWND, FALSE);
		UpdateWindow(hwnd);

		//restart mode drag
		mode = MODE_DRAG;

		//Disable sCut/Copy
		if (HandleEvent::isCutAndCopyOn) {
			SendMessage(HandleEvent::hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(0, 0));
			SendMessage(HandleEvent::hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(0, 0));
			HandleEvent::isCutAndCopyOn = false;
		}
	}
}

void HandleEdit::activatePaste(HWND& hwnd, int& mode) {

	//Save the mode first
	int save = mode;

	//end drag mode temporaly
	//start paste mode
	mode = MODE_PASTE;

	//Change color immediately
	InvalidateRect(hwnd, &HandleEvent::rcHWND, FALSE);
	UpdateWindow(hwnd);

	//Clear redo 
	HandleEdit::clearRedoStates();

	//activate mode drag if there are something to paste, else return to the past mode
	if (!HandleEdit::clipboard().empty()) {
		mode = MODE_DRAG;

		//Unlock sCut/Copy
		if (!HandleEvent::isCutAndCopyOn) {
			SendMessage(HandleEvent::hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(1, 0));
			SendMessage(HandleEvent::hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(1, 0));
			HandleEvent::isCutAndCopyOn = true;
		}
	}
	else mode = save;

}

void HandleEdit::activateUndo(HWND& hwnd, int& mode) {
	//Save the mode first
	int save = mode;

	//start mode undo
	mode = MODE_UNDO;

	//Change color immediately
	InvalidateRect(hwnd, &HandleEvent::rcHWND, FALSE);
	UpdateWindow(hwnd);

	//Return to the last mode
	mode = save;
}

void HandleEdit::activateRedo(HWND& hwnd, int& mode) {
	//Save the mode first
	int save = mode;

	//start mode undo
	mode = MODE_REDO;

	//Change color immediately
	InvalidateRect(hwnd, &HandleEvent::rcHWND, FALSE);
	UpdateWindow(hwnd);

	//Return to the last mode
	mode = save;
}