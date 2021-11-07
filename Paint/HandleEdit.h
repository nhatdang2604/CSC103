#pragma once
#ifndef _HANDLE_EDIT_H_
#define _HANDLE_EDIT_H_

#include "Shape.h"
#include <vector>

class HandleEdit {
private:
	static std::vector<std::vector<std::shared_ptr<Shape>>> _redoStates;	//Everything after undo would be contain in this
	static std::vector<std::shared_ptr<Shape>> _clipboard;
	static RECT _rcSelectCopy;
	static int _undoStartIndex;				//the smallest index in states that you can undo
private:
	static void traversalDraggedShapes(void(*)(int, std::vector<std::shared_ptr<Shape>>&), std::vector<std::shared_ptr<Shape>>&);
	//Help Delete/Cut/Copy/Paste travel in states
	//static void traversalStates(void(int, int));

public:	//Getter and setter
	static std::vector<std::shared_ptr<Shape>> clipboard();	//getter for EventHandle to use (only .empty())
	static void setUndoStartIndex(int);

public: //Helper 
	static void clearRedoStates();

public:
	static void modeDelete(HDC&, HDC&, int);
	static void modeCopy(HDC&, HDC&, int);
	static void modeCut(HDC&, HDC&, int);
	static void modePaste(HDC&, HDC&, int);
	static void modeUndo(HDC&, HDC&, int);
	static void modeRedo(HDC&, HDC&, int);

public:
	static void activateDelete(HWND&, int&);
	static void activateCopy(HWND&, int&);
	static void activateCut(HWND&, int&);
	static void activatePaste(HWND&, int&);
	static void activateUndo(HWND&, int&);
	static void activateRedo(HWND&, int&);
};

#endif // !_HANDLE_EDIT_H_




