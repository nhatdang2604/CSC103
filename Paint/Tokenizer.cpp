#include "stdafx.h"
#include "Tokenizer.h"

std::vector<std::string> Tokenizer::split(std::string& haystack, std::string& needle) {

	//result
	std::vector<std::string> tokens;

	//current token found
	std::string token;

	//current position of needle found in haystack
	std::size_t foundPos;

	//Start position to find needle in haystack
	std::size_t startPos = 0;

	while (true) {

		//Haven't reached the final token yet:
		if ((foundPos = haystack.find(needle, startPos)) != std::string::npos) {
			token = haystack.substr(startPos, foundPos - startPos);
			tokens.push_back(token);
			startPos = foundPos + needle.size();
		}
		else {
			//Reach the final token => get it
			token = haystack.substr(startPos, haystack.size() - startPos);
			tokens.push_back(token);
			break;
		}
	}
	return tokens;
};