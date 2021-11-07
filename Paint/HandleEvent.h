#pragma once
#ifndef _HANDLE_EVENT_H_
#define _HANDLE_EVENT_H_

#include "stdafx.h"
#include "Shape.h"
#include "Paint.h"
//#include "Pen.h"
//#include "HandleDraw.h"
//#include "HandleFile.h"

#include <memory>
#include <vector>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

//Foward declaration for About Box
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

//Foward declaration to avoid circular dependencies
class HandleDraw;
class HandleFile;
class HandleEdit;
class Pen;

//Type of mode
#define MODE_NULL 0
#define MODE_PREVIEW 1
#define MODE_DRAW 2
#define MODE_SELECT 3
#define MODE_DRAG 4
#define MODE_CHANGECOLOR 5
#define MODE_DELETE 6
#define MODE_COPY 7
#define MODE_CUT 8
#define MODE_PASTE 9
#define MODE_UNDO 10
#define MODE_REDO 11
#define MODE_SELECT_CANCEL 12
#define MODE_RESTORE_BEFORE_SELECT 13

//Button and button's images size
#define IMAGE_WIDTH     18
#define IMAGE_HEIGHT    18
#define BUTTON_WIDTH    0
#define BUTTON_HEIGHT   0

class HandleEvent {
private: //System varibale
	static HINSTANCE _hInst;

private:
	//All the Shape which have been drawn
	static std::vector<std::vector<std::shared_ptr<Shape>>> states;

	 //Drawing global variable
	static int typeFigure;	//type of figure which is being drawn
	static int fromX;		//the x - cordinates when click left button
	static int fromY;		//the y - cordinates when click left button
	static int toX;			//the x - cordinates while moving and releasing left button
	static int toY;			//the y - cordinates while moving and releasing left button
	static int mode;		//mode in drawing
	static int dx;			//dx = fromX - toX: this is the  x - cordinates of direction vectors while dragging shape
	static int dy;			//dy = fromY - toY: this is the  y - cordinates of direction vectors while dragging shape

	static bool isCutAndCopyOn;		//Use for LButtonUp in select mode or preview mode, to check if the 


								// mode = MODE_SELECT or MODE_PREVIEW is came from LButtonDown, in drag mode
	static bool isAfterDraw;	// isAfterDraw = true immediately after finish Draw mode
								// Else, after dragging when finish draw, isAfterDraw = false
								// This is use for dragging after drawing

	static HWND hToolBarWnd;		// toolbar

	static RECT rcHWND;				//the rectangle of drawn space
	static RECT rcSelect;			//the dash dot rectangle when using select
	static RECT rcSelectOriginal;	//the dash dot rectangle after finish select mode and before start drag mode

private:	//private access to modify states
	friend HandleDraw;
	friend HandleFile;
	friend HandleEdit;
	friend Pen;

public:	//Setter system variable
	static void setInstance(HINSTANCE&);

public:
	static BOOL OnCreate(HWND, LPCREATESTRUCT);
	static void OnCommand(HWND, int, HWND, UINT);
	static void OnKey(HWND, UINT, BOOL, int, UINT);
	static void OnDestroy(HWND);
	static void OnPaint(HWND);
	static void OnLButtonDown(HWND, BOOL, int, int, UINT);
	static void OnLButtonUp(HWND, int, int, UINT);
	static void OnMouseMove(HWND, int, int, UINT);
};


#endif // !_HANDLE_EVENT_H_

