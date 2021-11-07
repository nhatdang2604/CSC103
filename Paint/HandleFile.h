#pragma once
#ifndef _HANDLE_FILE_H_
#define _HANDLE_FILE_H_

#include <windowsx.h>
#include <string>
#include <fstream>
#include <commdlg.h>

//#include "HandleEvent.h"
//#include "Pen.h"

#define SIGN "This is a Paint project file"
#define EXTENSION L".pnimg"	//pain image
#define EXTENSION_FILTER L"Pain image\0*.pnimg\0" // type filter when save/open file


class HandleFile {
private:
	static bool _isSaved;			//Check if the current opened Paint is saved
	static std::wstring _filePath;	//Path of the current opened Paint

private:

	//Update window tilte when New/Open/Save/SaveAs
	static void updateTitle(HWND);

	//Init before open or save file
	static OPENFILENAME initOpenFileName(HWND);

	//- Read all shapes in fileName
	//- Load all read shapes into window
	static bool loadOpenedFile(std::wstring, HWND);

	//Write all shapes in states into fileName
	static bool saveCurrentFile(std::wstring);


public:
	static void Open(HWND);
	static void SaveAs(HWND);
	static void Save(HWND hwnd);
	static void New(HWND hwnd);
};


#endif // !_HANDLE_FILE_H_


