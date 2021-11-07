#include "stdafx.h"
#include "HandleFile.h"
#include "HandleEvent.h"
#include "Shape.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Pen.h"

#include <string>
#include <commdlg.h>

//Init for static variables
bool HandleFile::_isSaved = false;
std::wstring HandleFile::_filePath = L"Untilted";

void HandleFile::updateTitle(HWND hwnd) {

	std::wstring title;
	std::wstring filename;
	std::size_t pos;

	//If there aren't any "\" => it must be "Untitled"
	if ((pos = _filePath.rfind(L"\\")) == std::string::npos) {
		filename = _filePath;
	}
	else {
		filename = _filePath.substr(pos + 1, _filePath.length() - 1);
	}

	title += filename + L" - Paint";
	SetWindowTextW(hwnd, title.c_str());
}

//Init before open or save file
OPENFILENAME  HandleFile::initOpenFileName(HWND hwnd) {
	OPENFILENAME ofn;						  // common dialog box structure
	wchar_t* szFile = new wchar_t[256];       // buffer for file name

											  // Init ofn
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = 256 * sizeof(szFile);

	//Save/Load as .txt files
	ofn.lpstrFilter = EXTENSION_FILTER;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	//The default dir always is the project folder
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return ofn;
}

//- Read all shapes in fileName
//- Load all read shapes into window
bool HandleFile::loadOpenedFile(std::wstring fileName, HWND hwnd) {
	std::ifstream infile(fileName);

	//If cannot open fileName => return;
	if (!infile.is_open()) return false;

	//Check if the opened file had correct format
	std::string buffer;
	getline(infile, buffer);
	if (SIGN != buffer) return false;


	//clear current states
	HandleEvent::states.clear();
	Shape bufferShape;

	while (!infile.eof()) {
		getline(infile, buffer);

		if ("" != buffer) {
			//Split Style and Figure information
			std::vector<std::string> tokens = Tokenizer::split(buffer, std::string("| "));
			std::vector<std::string> tokens_styles = Tokenizer::split(tokens[0], std::string(" "));

			//Get Figure
			bufferShape.figure = Parser::parse(tokens[1]);
			if (!bufferShape.figure) break;

			//Get Style
			bufferShape.iStyle = std::stoi(tokens_styles[0]);
			bufferShape.cWidth = std::stoi(tokens_styles[1]);
			bufferShape.color = std::stoi(tokens_styles[2]);

			//Init flag
			bufferShape.isValid = true;
			bufferShape.executedFrom = nullptr;

			std::vector<std::shared_ptr<Shape>> state;
			state.push_back(std::make_shared<Shape>(bufferShape));
			HandleEvent::states.push_back(state);
		}
	}

	//Print all figure after read step

	//HDC for printing
	HDC hdc = GetDC(hwnd);
	Pen pen(hdc);
	RECT rcHWND;

	//Get the RECT of hwnd when working
	GetClientRect(hwnd, &rcHWND);

	//Prevent making FillRect() fill the toolbar
	rcHWND.top += IMAGE_HEIGHT + 10;

	//Fill the workspace with (COLOR_WINDOW + 1) color
	FillRect(hdc, &rcHWND, (HBRUSH)(COLOR_WINDOW + 1));

	//this would make draw transparent figure
	SelectObject(hdc, GetStockObject(NULL_BRUSH));

	//Draw all shape in states
	Pen::drawAll(hdc);


	DeleteDC(hdc);
	infile.close();
	return true;
}

//Write all shapes in states into fileName
bool HandleFile::saveCurrentFile(std::wstring fileName) {

	//Seperator for Style and Figure information
	std::string seperator = "| ";
	std::ofstream outfile(fileName);

	if (!outfile.is_open()) {
		return false;
	}

	//Add file sign
	outfile << SIGN << std::endl;

	for (std::vector<std::shared_ptr<Shape>> i : HandleEvent::states) {
		for (std::shared_ptr<Shape> j: i) {
			if (j->isValid) {
				outfile << j->iStyle << " " << j->cWidth << " " << j->color;
				outfile << seperator;
				outfile << j->figure->toString();
				outfile << std::endl;
			}
		}
	}

	outfile.close();
	return true;
}


void HandleFile::Open(HWND hwnd) {
	OPENFILENAME ofn = initOpenFileName(hwnd);

	// Display the Open dialog box. 
	if (GetOpenFileNameW(&ofn)) {

		std::wstring loadFileName(ofn.lpstrFile);
		if (HandleFile::loadOpenedFile(ofn.lpstrFile, hwnd)) {
			_isSaved = true;
			_filePath = loadFileName;
			updateTitle(hwnd);

			//Clear everything in the previous files
			HandleEdit::setUndoStartIndex(HandleEvent::states.size());
			HandleEdit::clearRedoStates();
		}
	}
	if (ofn.lpstrFile) delete ofn.lpstrFile;
}

void HandleFile::SaveAs(HWND hwnd) {
	OPENFILENAME ofn = initOpenFileName(hwnd);
	ofn.Flags |= OFN_OVERWRITEPROMPT;

	// Display the Save dialog box. 
	if (GetSaveFileNameW(&ofn)) {

		std::wstring saveFileName(ofn.lpstrFile);

		//Add extension if the user input only fileName
		if (saveFileName.find(EXTENSION) == std::string::npos) {
			saveFileName.append(EXTENSION);
		}

		if (HandleFile::saveCurrentFile(saveFileName)) {
			_isSaved = true;
			_filePath = saveFileName;
			updateTitle(hwnd);
		}
	}
	if (ofn.lpstrFile) delete ofn.lpstrFile;
}

void HandleFile::Save(HWND hwnd) {
		if (!_isSaved) {
			SaveAs(hwnd);
		}
		else {
			//After the first save, if you haven't click New, this would not be Save As anymore
			if (HandleFile::saveCurrentFile(_filePath)) {
				_isSaved = true;
			}
		}
	}

void HandleFile::New(HWND hwnd) {

	//HDC for printing
	HDC hdc = GetDC(hwnd);
	RECT rcHWND;

	//Get the RECT of hwnd where to work
	GetClientRect(hwnd, &rcHWND);

	//Prevent making FillRect() fill the toolbar
	rcHWND.top += IMAGE_HEIGHT + 10;

	//Fill the workspace with (COLOR_WINDOW + 1) color
	FillRect(hdc, &rcHWND, (HBRUSH)(COLOR_WINDOW + 1));

	//Clear everything which has been drawn
	HandleEvent::states.clear();

	//Clear everything in the previous files
	HandleEdit::setUndoStartIndex(-1);
	HandleEdit::clearRedoStates();

	_isSaved = false;
	_filePath = L"Untilted";
	updateTitle(hwnd);
	DeleteDC(hdc);
}

