#include "stdafx.h"
#include "resource.h"

#include "HandleEvent.h"
#include "HandleDraw.h"
#include "HandleFile.h"
#include "Helper.h"
#include "Figure.h"

#include <windowsx.h>
#include <winuser.h>

//Init static system variable
HINSTANCE HandleEvent::_hInst;

//Init static variable
std::vector<std::vector<std::shared_ptr<Shape>>> HandleEvent::states;
int HandleEvent::typeFigure = TYPE_NULL;
int HandleEvent::fromX;
int HandleEvent::fromY;
int HandleEvent::toX;
int HandleEvent::toY;
int HandleEvent::mode = MODE_NULL;
int HandleEvent::dx;
int HandleEvent::dy;
bool HandleEvent::isCutAndCopyOn = false;
bool HandleEvent::isAfterDraw = false;
RECT HandleEvent::rcHWND;
RECT HandleEvent::rcSelect;
RECT HandleEvent::rcSelectOriginal;
HWND HandleEvent::hToolBarWnd;

void HandleEvent::setInstance(HINSTANCE& hInst) {
	_hInst = hInst;
}

BOOL HandleEvent::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) {

	//Create dialog
	InitCommonControls();

	//Add new, open and save button (3 default buttons)
	TBBUTTON tbButtons[] =
	{
		{ STD_FILENEW,	ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ STD_FILEOPEN,	ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ STD_FILESAVE,	ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
	};

	hToolBarWnd = CreateToolbarEx(hwnd,
		WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
		ID_TOOLBAR, sizeof(tbButtons) / sizeof(TBBUTTON), HINST_COMMCTRL,
		0, tbButtons, sizeof(tbButtons) / sizeof(TBBUTTON),
		BUTTON_WIDTH, BUTTON_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT,
		sizeof(TBBUTTON));

	
	TBBUTTON buttonsToAdd[] =
	{
	{ 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
	{ STD_CUT,	ID_EDIT_CUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_COPY,	ID_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_PASTE, ID_EDIT_PASTE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_DELETE, ID_EDIT_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_UNDO, ID_EDIT_UNDO, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
	{ STD_REDOW, ID_EDIT_REDO, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
	};

	SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM) sizeof(buttonsToAdd) / sizeof(TBBUTTON),
	(LPARAM)(LPTBBUTTON)&buttonsToAdd);
	
	//Disable Cut/Copy
	SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(0, 0));
	SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(0, 0));


	//Add shape draw buttons
	TBBUTTON userButtons[] =
	{

		{ 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
		{ 0, ID_DRAW_ELLIPSE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 1, ID_DRAW_RECTANGLE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 2, ID_DRAW_LINE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
		{ 3, ID_DRAW_SELECT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
	};


	TBADDBITMAP	tbBitmap = { HandleEvent::_hInst, IDB_BITMAP1 };

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


void HandleEvent::OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

	if (IDM_ABOUT == id) {
		DialogBoxW(_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		return;
	}

	if (IDM_FILE_EXIT == id) {
		DestroyWindow(hwnd);
		return;
	}

	if (ID_DRAW_ELLIPSE == id) {
		HandleDraw::activateDrawShape(mode, TYPE_ELLIPSE);
		return;
	}

	if (ID_DRAW_RECTANGLE == id) {
		HandleDraw::activateDrawShape(mode, TYPE_RECTANGLE);
		return;
	}

	if (ID_DRAW_LINE == id) {
		HandleDraw::activateDrawShape(mode, TYPE_LINE);
		return;
	}

	if (ID_DRAW_SELECT == id) {
		HandleDraw::activateSelect(mode);
		return;
	}

	if (ID_FILE_NEW == id || IDM_FILE_NEW == id) {
		HandleFile::New(hwnd);
		return;
	}

	if (ID_FILE_OPEN == id || IDM_FILE_OPEN == id) {
		HandleFile::Open(hwnd);
		return;
	}

	if (ID_FILE_SAVE == id || IDM_FILE_SAVE == id) {
		HandleFile::Save(hwnd);
		return;
	}

	if (IDM_FILE_SAVEAS == id) {
		HandleFile::SaveAs(hwnd);
		return;
	}

	if (IDM_CHOOSE_COLOR == id) {
		HandleDraw::activateChooseColor(hwnd, mode);
		return;
	}


	if (ID_EDIT_DELETE == id) {
		HandleEdit::activateDelete(hwnd, mode);
		return;
	}

	if (ID_EDIT_COPY == id) {
		HandleEdit::activateCopy(hwnd, mode);
		return;
	}

	if (ID_EDIT_CUT == id) {
		HandleEdit::activateCut(hwnd, mode);
		return;
	}

	if (ID_EDIT_PASTE == id) {
		HandleEdit::activatePaste(hwnd, mode);
		return;
	}

	if (ID_EDIT_UNDO == id) {
		HandleEdit::activateUndo(hwnd, mode);
		return;
	}

	if (ID_EDIT_REDO == id) {
		HandleEdit::activateRedo(hwnd, mode);
		return;
	}
}

void HandleEvent::OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags) {

	if (GetKeyState(VK_CONTROL) < 0) {
		if ('S' == vk) HandleFile::Save(hwnd);							//Ctrl + S => Save
		else if ('O' == vk) HandleFile::Open(hwnd);						//Ctrl + O => Open
		else if ('N' == vk) HandleFile::New(hwnd);						//Ctrl + N => New
		else if ('X' == vk) HandleEdit::activateCut(hwnd, mode);		//Ctrl + X => Cut
		else if ('C' == vk) HandleEdit::activateCopy(hwnd, mode);		//Ctrl + C => Copy
		else if ('V' == vk) HandleEdit::activatePaste(hwnd, mode);		//Ctrl + V => Paste
		else if ('Z' == vk) HandleEdit::activateUndo(hwnd, mode);		//Ctrl + Z => Undo
		else if ('Y' == vk) HandleEdit::activateRedo(hwnd, mode);		//Ctrl + Y => Redo
	}
	else if (VK_SHIFT == vk && MODE_DRAG == mode) {						//SHIFT while dragging => duplicate
		HandleEdit::activateCopy(hwnd, mode);
		HandleEdit::activatePaste(hwnd, mode);
	}
	else if (VK_DELETE == vk) HandleEdit::activateDelete(hwnd, mode);	//Delete => Delete
	else if (VK_F12 == vk) HandleFile::SaveAs(hwnd);					//F12 => Save As
}

void HandleEvent::OnDestroy(HWND hwnd) {
	PostQuitMessage(0);
}

/*On pain :<*/
void HandleEvent::OnPaint(HWND hwnd) {

	//Setup draw space
	GetClientRect(hwnd, &HandleEvent::rcHWND);

	//Prevent every time we use FillRect(), we do not erase the toolbar
	HandleEvent::rcHWND.top += IMAGE_HEIGHT + 10;

	//Begin pain :<<
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	HDC memoryDC;
	HBITMAP memoryBMP;

	//Init variables
	HandleDraw::initToDraw(hdc, memoryDC, memoryBMP);

	//Mode decision
	if (MODE_PREVIEW == mode) HandleDraw::modePreview(hdc, memoryDC, mode);
	else if (MODE_DRAW == mode) HandleDraw::modeDraw(hdc, memoryDC, mode);
	else if (MODE_SELECT_CANCEL == mode) HandleDraw::modeDrawPause(hdc, memoryDC, mode);
	else if (MODE_SELECT == mode) HandleDraw::modeSelect(hdc, memoryDC, mode);
	else if (MODE_DRAG == mode) HandleDraw::modeDrag(hdc, memoryDC, HandleEvent::isAfterDraw, mode);
	else if (MODE_CHANGECOLOR == mode) HandleDraw::modeChangeColor(hdc, memoryDC, mode);
	else if (MODE_DELETE == mode) HandleEdit::modeDelete(hdc, memoryDC, mode);
	else if (MODE_CUT == mode) HandleEdit::modeCut(hdc, memoryDC, mode);
	else if (MODE_COPY == mode) HandleEdit::modeCopy(hdc, memoryDC, mode);
	else if (MODE_PASTE == mode) HandleEdit::modePaste(hdc, memoryDC, mode);
	else if (MODE_UNDO == mode) HandleEdit::modeUndo(hdc, memoryDC, mode);
	else if (MODE_REDO == mode) HandleEdit::modeRedo(hdc, memoryDC, mode);

	//End pain :<<<
	DeleteObject(hdc);
	DeleteObject(memoryBMP);
	DeleteDC(memoryDC);
	EndPaint(hwnd, &ps);
}

void HandleEvent::OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) {

	if (MODE_NULL == mode || MODE_PREVIEW == mode || MODE_DRAG == mode) {
		HandleEdit::clearRedoStates();
	}

	if (MODE_RESTORE_BEFORE_SELECT == mode) {
		if (!isAfterDraw) mode = MODE_SELECT;	//Setup to begin mode select (when in normal drag mode)
		else {
			mode = MODE_PREVIEW;				//Setup to begin preview mode (when in drag-after-draw mode)
			isAfterDraw = false;
		}
	}

	if (MODE_DRAG == mode) {

		//If cursor isn't in the rcSelect
		//	=> Finish to drag 
		if (!(rcSelect.left <= x && x <= rcSelect.right &&
			rcSelect.top <= y && y <= rcSelect.bottom)) {

			//If there are no figures in rcSelectOriginal => drag nothing
			HandleDraw::finishToDrag(states);

			isCutAndCopyOn = false;

			//Disable sCut/Copy
			SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(0, 0));
			SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(0, 0));
			
			mode = MODE_SELECT_CANCEL;
		}

	}

	if (MODE_DELETE == mode) {
		if (!isAfterDraw) mode = MODE_SELECT;		//Setup to begin mode select (when was in normal drag mode)
		else {
			mode = MODE_PREVIEW;					//Setup to begin preview select (when dragging after draw)
			isAfterDraw = false;
		}
	}

	//Activate preview mode
	if (MODE_NULL == mode) {
		mode = MODE_PREVIEW;
	}
	else if (MODE_SELECT == mode) {
		typeFigure = TYPE_RECTANGLE;
	}



	//Get the mouse's start position
	fromX = x;
	fromY = y;
	toX = x;
	toY = y;

	//If we are selecting (or selecting after drag)
	//	=> Initialize the selecting rectangle cordinates 
	if (MODE_SELECT == mode || MODE_SELECT_CANCEL == mode) {
		rcSelect.left = x;
		rcSelect.top = y;
		rcSelect.right = x;
		rcSelect.bottom = y;
	}

	HDC hdc = GetDC(hwnd);
	MoveToEx(hdc, fromX, fromY, NULL);
}

void HandleEvent::OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) {
	if (TYPE_NULL == typeFigure) return;

	if (MODE_SELECT_CANCEL == mode) {

		InvalidateRect(hwnd, &rcHWND, FALSE);
		UpdateWindow(hwnd);

		mode = MODE_RESTORE_BEFORE_SELECT;
		return;
	}

	//After preview mode
	if (MODE_PREVIEW == mode) {

		//Activate draw mode
		mode = MODE_DRAW;
	}

	InvalidateRect(hwnd, &rcHWND, FALSE);
	UpdateWindow(hwnd);

	//After draw mode
	if (MODE_DRAW == mode) {

		//Activate Drag mode
		mode = MODE_SELECT;

		//Save the 4 corner of a figure after draw it
		rcSelect.left = fromX;
		rcSelect.top = fromY;
		rcSelect.right = toX;
		rcSelect.bottom = toY;

		//Swap left, top, right, bottom of rcSelect to correct position
		Helper::standardlizeRect(rcSelect);

		//Update the original position of rcSelect
		rcSelectOriginal = rcSelect;

		//Save the current type of figure
		int saved = typeFigure;

		//Set the type of figure for rcSelect
		typeFigure = TYPE_RECTANGLE;

		//Draw rcSelect (Cause we are in select mode)
		InvalidateRect(hwnd, &rcHWND, FALSE);
		UpdateWindow(hwnd);

		//End Select mode, activate drag mode
		mode = MODE_DRAG;

		//Restore type of figure
		typeFigure = saved;

		isAfterDraw = true;

		//Unlock Cut/Copy
		isCutAndCopyOn = true;
		SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(1, 0));
		SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(1, 0));

		return;
	}

	//After select mode
	if (MODE_SELECT == mode) {

		//Swap left, top, right, bottom of rcSelect to correct position
		Helper::standardlizeRect(rcSelect);

		//Save the original position of rcSelect
		rcSelectOriginal = rcSelect;

		//Activate drag mode
		mode = MODE_DRAG;

		//Unlock Cut/Copy
		if (!(fromX == x && fromY == y)) {
			isCutAndCopyOn = true;
			SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_CUT, MAKELONG(1, 0));
			SendMessage(hToolBarWnd, TB_ENABLEBUTTON, (WPARAM)ID_EDIT_COPY, MAKELONG(1, 0));
		}
		return;
	}

}

void HandleEvent::OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) {
	if (TYPE_NULL == typeFigure) return;
	if (MODE_RESTORE_BEFORE_SELECT == mode) return;
	if (MODE_SELECT_CANCEL == mode) {
		if (!isAfterDraw) mode = MODE_SELECT;	//Setup to begin mode select (when in normal drag mode)
		else {
			mode = MODE_PREVIEW;				//Setup to begin preview mode (when in drag-after-draw mode)
			isAfterDraw = false;
		}
	}

	//Hold SHIFT while chosing Ellipse (or Rectangle)
	//	=> Draw Circle (or Square)
	if (GetKeyState(VK_SHIFT) < 0) {
		if (TYPE_ELLIPSE == typeFigure) typeFigure = TYPE_CIRCLE;
		else if (TYPE_RECTANGLE == typeFigure) typeFigure = TYPE_SQUARE;
	}
	else {

		//Release SHIFT while drawing Circle (or Square)
		//	=> Return to Ellipse (or Rectangle)
		if (TYPE_CIRCLE == typeFigure) typeFigure = TYPE_ELLIPSE;
		else if (TYPE_SQUARE == typeFigure) typeFigure = TYPE_RECTANGLE;
	}


	//There are nothing to do while mouse is moving on mode delte
	if (MODE_DELETE == mode) return;

	//Every moment, delete the preview object 
	//(this make every moment, there is only 1 preview object existing)
	if (MODE_PREVIEW == mode || MODE_SELECT == mode) {

		//Get the mouse current position
		toX = x;
		toY = y;

		//If we are selecting
		//	=>Update the cordinates of the selecting cordinates
		if (MODE_SELECT == mode) {
			rcSelect.right = x;
			rcSelect.bottom = y;
		}

		InvalidateRect(hwnd, &rcHWND, FALSE);
		return;
	}

	//In drag mode
	if (MODE_DRAG == mode) {

		//Hold left mouse button
		if (GetKeyState(VK_LBUTTON) < 0) {
			toX = x;
			toY = y;

			dx = toX - fromX;
			dy = toY - fromY;

			rcSelect.left += dx;
			rcSelect.top += dy;
			rcSelect.right += dx;
			rcSelect.bottom += dy;

			InvalidateRect(hwnd, &rcHWND, FALSE);

			fromX = x;
			fromY = y;
		}

	}
}