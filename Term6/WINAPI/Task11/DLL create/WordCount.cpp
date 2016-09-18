#define MYLIBAPI extern "C" __declspec(dllexport)

#include "WordCount.h"
#include <Windows.h>
#include <cctype>

MYLIBAPI int WordsCount(const wchar_t* text) {

	if (text == NULL || text[0] == '\0') {
		return 0;
	}

	int ind = 0;
	while (std::isspace(text[ind])) {
		ind++;
		if (text[ind] == '\0') {
			return 0;
		}
	}

	int word_count = 0;
	wchar_t prev_char = text[ind++];

	while (text[ind] != '\0') {
		if (std::isspace(text[ind]) && !std::isspace(prev_char)) {
			word_count++;
		}

		prev_char = text[ind++];
	}
	if (!std::isspace(text[ind - 1])) {
		word_count++;
	}

	return word_count;
}