#pragma once
#ifndef _EVENT_HANDLE_H_
#define _EVENT_HANDLE_H_

#include "windowsx.h"
#include "stdafx.h"
#include "Paint.h"
#include "Pen.h"
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")

#define IMAGE_WIDTH     18
#define IMAGE_HEIGHT    18
#define BUTTON_WIDTH    0
#define BUTTON_HEIGHT   0

int fromX;
int fromY;
int toX;
int toY;
bool isPreviewing = false;
bool isDrawing = false;

class EventHandle {
public:

	static BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {

		//Get font from system
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName
		);

		//Create dialog
		InitCommonControls();

		//Add new, open and save button (3 default buttons)
		TBBUTTON tbButtons[] =
		{
			{ STD_FILENEW,	ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ STD_FILEOPEN,	ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ STD_FILESAVE,	ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
		};

		HWND hToolBarWnd = CreateToolbarEx(hwnd,
			WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
			ID_TOOLBAR, sizeof(tbButtons) / sizeof(TBBUTTON), HINST_COMMCTRL,
			0, tbButtons, sizeof(tbButtons) / sizeof(TBBUTTON),
			BUTTON_WIDTH, BUTTON_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT,
			sizeof(TBBUTTON));
		
		TBBUTTON buttonsToAdd[] =
		{
			{ 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 }, // Nút phân cách
			{ STD_CUT,	ID_EDIT_CUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ STD_COPY,	ID_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ STD_PASTE, ID_EDIT_PASTE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ STD_DELETE, ID_EDIT_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
		};
		SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM) sizeof(buttonsToAdd) / sizeof(TBBUTTON),
			(LPARAM)(LPTBBUTTON)&buttonsToAdd);

		
		//Add user button
		TBBUTTON userButtons[] =
		{
			{ 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
			{ 0, ID_DRAW_ELLIPSE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ 1, ID_DRAW_RECTANGLE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ 2, ID_DRAW_LINE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
			{ 3, ID_DRAW_TEXT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
		};
	
		TBADDBITMAP	tbBitmap = { hInst, IDB_BITMAP1 };
	
		int idx = SendMessage(hToolBarWnd, TB_ADDBITMAP, (WPARAM) sizeof(tbBitmap) / sizeof(TBADDBITMAP),
			(LPARAM)(LPTBADDBITMAP)&tbBitmap);
		
		userButtons[1].iBitmap += idx;
		userButtons[2].iBitmap += idx;
		userButtons[3].iBitmap += idx;
		userButtons[4].iBitmap += idx;

		SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM) sizeof(userButtons) / sizeof(TBBUTTON),
			(LPARAM)(LPTBBUTTON)&userButtons);
		

		return TRUE;

	}


	static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

	}

	static void OnDestroy(HWND hwnd) {
		PostQuitMessage(0);
	}


	/*On pain :<*/
	static void OnPaint(HWND hwnd) {
		PAINTSTRUCT ps;

		//Begin pain :<<<
		HDC hdc = BeginPaint(hwnd, &ps);

		//Todo:

		//Clear screen continously in preview mode
		if (isPreviewing) FillRect(hdc, &ps.rcPaint, (HBRUSH)COLORREF(0xFFFFFF));
		
		//Restore all figures which are drawn in drawing mode
		Pen pen(hdc);
		pen.drawAll(hdc);

		
		std::shared_ptr<figure::Line> line =
			std::make_shared<figure::Line>(figure::Point(fromX, fromY), figure::Point(toX, toY));
		pen.setFigure(line);
		pen.draw(hdc);
		
		/*
		std::shared_ptr<figure::Ellipse> ellipse =
			std::make_shared<figure::Ellipse>(figure::Point(fromX, fromY), figure::Point(toX, toY));
		pen.setFigure(ellipse);
		pen.draw(hdc);
		*/

		/*
		std::shared_ptr<figure::Rectangle> rectangle = 
			std::make_shared <figure::Rectangle> (figure::Point(fromX, fromY), figure::Point(toX, toY));
		pen.setFigure(rectangle);
		pen.draw(hdc);
		*/

		//If in draw mode => this is the draw shape => save it
		if (isDrawing) pen.saveShape();

		//End pain :<<<<
		EndPaint(hwnd, &ps);

		//End draw mode
		if (isDrawing) isDrawing = false;
	}

	static void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {

		//Activate preview mode
		isPreviewing = true;

		//Get the mouse's start position
		fromX = x;
		fromY = y;

		HDC hdc = GetDC(hwnd);
		MoveToEx(hdc, fromX, fromY, NULL);
	}

	static void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {

		//End preview mode
		isPreviewing = false;

		//Start to draw
		isDrawing = true;

		//Delete the last preview object 
		InvalidateRect(hwnd, NULL, TRUE);
	}

	static void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {

		//Every moment, delete the preview object 
		//(this make every moment, there is only 1 preview object existing)
		if (isPreviewing) {

			//Get the mouse current position
			toX = x;
			toY = y;

			
			InvalidateRect(hwnd, NULL, TRUE);
			//RedrawWindow(hwnd, &region, NULL, RDW_INVALIDATE | RDW_ERASENOW);
		}
	}


};



#endif // !_EVENT_HANDLE_H_

