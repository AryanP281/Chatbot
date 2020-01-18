#pragma once

/********************Preprocessor Directives*****************************/
#include<string>
#include <vector>
#include <fstream>

/********************Functions*****************************/
std::string ReadLine(std::fstream& file); //Reads a line from the file
char CharToNum(char ch); //Converts the character to the corresponding numerical digit
std::string ProcessString(const std::string& str); //Prepares the strings for analysis
bool IsInvalidChar(const char ch); //Tells whether a char is considered to be invalid
bool StringContains(const char* ch, const std::string& str, char flag = 0); /*Tells whether the string contains
the character*/
float ReturnNoOfIndividualChars(const std::string& str, char flag = 0); /*Returns the number of individual 
characters that a string contains*/
template<typename T> bool VectorContains(std::vector<T>& vec, T& element); /*Tells whether the
vector contains the element*/
bool IsUppercase(const char* ch); //Tells whether the character is an uppercase character
bool HasFullstop(const std::string& txt, int index); /*Checking if a part of the text has a 
fullstop before it*/
void EliminateTrailingWhitespaces(std::string& str); /*Removes the whitespaces present at the end 
of the text*/
