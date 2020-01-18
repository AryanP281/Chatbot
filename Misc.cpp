/********************Preprocessor Directives*****************************/
#include "stdafx.h"
#include "Misc.h"

/********************Functions*****************************/

char CharToNum(char ch)
{
	switch (ch)
	{
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	default: return '\0';
	}
}

std::string ProcessString(const std::string& str)
{
	std::string processedString;

	//Eliminating the whitespaces and the invalid char
	for (int a = 0; a < str.size(); ++a)
	{
		if (!IsInvalidChar(str[a]))
			processedString += str[a];
	}

	return processedString;
}

bool IsInvalidChar(const char ch)
{
	//Checking if the character is whitespace or a punctuation mark
	if ((int)(ch) >= 32 && (int)(ch) <= 47)
		return true;

	return false;
}

bool StringContains(const char* ch, const std::string& str, char flag)
{
	for (int a = 0; a < str.size(); ++a)
	{
		if (flag == 0)
		{
			if (str[a] == *ch)
				return true;
		}
		else if (flag == 1)
		{
			if (tolower(str[a]) == tolower(*ch))
				return true;
		}
	}

	return false;
}

template<typename T> bool VectorContains(std::vector<T>& vec, T& element)
{
	//Iterating through the vector
	for (T obj : vec)
	{
		if (obj == element)
			return true;
	}

	return false;
}

float ReturnNoOfIndividualChars(const std::string& str, char flag)
{
	std::vector<char> includedChars; //The chars that have already been included in the list
	std::string processedTxt = ProcessString(str);

	for (int a = 0; a < processedTxt.size(); ++a)
	{
		if (flag == 0)
		{
			if (!VectorContains<char>(includedChars, processedTxt[a]))
				includedChars.push_back(processedTxt[a]);
		}
		else if (flag == 1)
		{
			char ch = tolower(processedTxt[a]);

			if (!VectorContains<char>(includedChars, ch))
				includedChars.push_back(tolower(processedTxt[a]));
		}
	}

	return includedChars.size();
}

bool IsUppercase(const char* ch)
{
	//Checking if the character lies in the ASCII limits of uppercase characters
	if ((int)*ch >= 65 && (int)*ch <= 90)
		return true;

	return false;
}

bool HasFullstop(const std::string& txt, int index)
{
	for (int a = index - 1; index >= 0; --a)
	{
		if (txt[a] == '.' || txt[a] == '!' || txt[a] == '?')
			return true;
		else if (txt[a] != ' ')
			return false;
	}

	return false;
}

void EliminateTrailingWhitespaces(std::string& str)
{
	for (int a = str.size() - 2; a >= 0; --a)
	{
		if (str[a] > 31 && str[a] != '\0')
			break;

		str.erase(a, 1);
	}
}
