// ExtractPTIs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Misc.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

const float IDEAL_SIMILARITY = 75.0f;

typedef std::pair<std::string, std::vector<std::string>> Pti;

bool Compare(const std::string& txt, const std::string& compareTo);
float Test1(const std::string& txt, const std::string& compareTo);
float Test2(const std::string& txt, const std::string& compareTo);
template<typename T> bool VectorContains(std::vector<T>& vec, T& element);
std::string ExtractPTI(const std::string& txt, int begIndex, char flag = 0);
int PtiExists(const std::string& pti, const std::vector<Pti>& ptis);
int PtiExists(const std::string& pti, const std::vector<std::string>& ptis);

void GetAndSavePtis(const std::string& filePath, const std::string& ptiFilePath);
std::vector<Pti> GetPtis(const std::vector<std::string>& filePath);
std::vector<std::string> TellWhetherContainsPTI(const std::string& txt);
bool SavePti(const std::string& filePth, std::vector<Pti>& ptis);

int main()
{
	for (int a = 67; a <= 73; ++a)
	{
		std::string filePath = "C:/Users/Aryan/Documents/Visual Studio 2017/Projects/C#/ChatBotGUI/ChatBotGUI/bin/Debug/Data/" + std::to_string(a) + ".txt";
		std::string ptiFilePath = "C:/Users/Aryan/Documents/Visual Studio 2017/Projects/C#/ChatBotGUI/ChatBotGUI/bin/Debug/Data/PTIs.txt";

		//std::getline(std::cin, filePath);
		//std::getline(std::cin, ptiFilePath);

		GetAndSavePtis(filePath, ptiFilePath);
	}

	std::getchar();
}

bool Compare(const std::string& txt, const std::string& compareTo)
{
	float xCoord = Test1(txt, compareTo); //The result of the 1st similarity test
	float yCoord = Test2(txt, compareTo); //The result of the 2nd similarity test

	float similarityFac = (sqrt(pow(xCoord, 2) + pow(yCoord, 2)) / sqrt(1 + 1)) * 100; /*Calculating
	the final similarity factor*/

	 //Checking if the texts are similar
	if (similarityFac >= IDEAL_SIMILARITY)
		return true;

	return false;
}

float Test1(const std::string& txt, const std::string& compareTo)
{
	float res = 0;
	float similarityFactor;

	//Comparing the words letter by letter
	for (int a = 0; a < txt.size() && a < compareTo.size(); ++a)
	{
		if (tolower(txt[a]) == tolower(compareTo[a]))
			res++;
	}
	txt.size() >= compareTo.size() ? similarityFactor = res / txt.size() : similarityFactor = res / compareTo.size();

	return similarityFactor; //Returning the similarity factor
}

float Test2(const std::string& txt, const std::string& compareTo)
{
	std::string text = ProcessString(txt); //Gets a processed version of the string for analysis
	std::string compareToTxt = ProcessString(compareTo); /*Gets a processed version of the string to
														 be compared to*/
	float noOfCommonChar = 0; //The number of common characters that the two strings have
	std::vector<char> includededChs; //The characters which have already been counted

	for (int a = 0; a < text.size(); ++a)
	{
		char ch = tolower(text[a]);
		if (StringContains(&ch, compareToTxt, 1) && !VectorContains<char>(includededChs, ch))
		{
			noOfCommonChar++; //Incrementing the counter
			includededChs.push_back(ch); //Keeping track of the already counted chars
		}
	}

	return (noOfCommonChar / ReturnNoOfIndividualChars(compareToTxt, 1));
}

std::string ExtractPTI(const std::string& txt, int begIndex, char flag)
{
	std::string pti; //The extracted PTI

	pti += txt[begIndex];
	if (begIndex != txt.size() - 1)
	{

		if (flag == 0) //Handling PTIs which are not in inverted commas
		{
			//Extracting the PTI
			for (int b = begIndex + 1; b < txt.size(); ++b)
			{
				if (txt[b] == '.' || txt[b] == '!' || txt[b] == '?' || txt[b] == ',')
					break;

				if (txt[b] != ' ')
				{
					pti += txt[b];
				}
				else if (txt[b] == ' ')
				{
					if (b != txt.size() - 1)
					{
						if (IsUppercase(&txt[++b]))
						{
							pti += " ";
							pti += txt[b];
							continue;
						}
						else
							break;
					}
				}
			}
		}
		else if (flag == 1) //Handling PTIs which are in inverted commas
		{
			if (begIndex != txt.size() - 1)
			{
				for (int b = begIndex + 1; b < txt.size(); ++b)
				{
					if (txt[b] != 34)
					{
						pti += txt[b];
						continue;
					}

					break;
				}
			}
		}
	}

	return pti;
}

void GetAndSavePtis(const std::string& filePath, const std::string& ptiFilePath)
{
	std::vector<std::string> convs;
	std::fstream file(filePath, std::ios::in);

	if (!file.is_open())
		std::cout << "Unable to open file\n";

	while (true)
	{
		std::string buffer;

		std::getline(file, buffer);

		if (buffer == "#")
			break;

		convs.push_back(buffer);
	}

	file.close();

	std::vector<Pti> ptis = GetPtis(convs);

	if (!SavePti(ptiFilePath, ptis))
		std::cout << "Unable to save PTIs\n";

	std::cout << "Process completed \n";
}

std::vector<Pti> GetPtis(const std::vector<std::string>& convs)
{
	std::vector<Pti> ptis;
	
	for (int a = 0; a < convs.size(); ++a)
	{
		std::vector<std::string> extractedPtis = TellWhetherContainsPTI(convs[a]);

		if (extractedPtis.size() != 0)
		{
			for (std::string pti : extractedPtis)
			{
				if (PtiExists(pti, ptis) > ptis.size())
				{
					ptis.push_back(Pti(pti, std::vector<std::string>()));
					ptis[ptis.size() - 1].second.push_back(convs[a]);

					for (int b = a + 1; b < convs.size(); ++b)
					{
						std::vector<std::string> cmprPtis = TellWhetherContainsPTI(convs[b]);
						for (std::string cmprPti : cmprPtis)
						{
							if (Compare(pti, cmprPti))
							{
								ptis[ptis.size() - 1].second.push_back(convs[b]);
							}
						}
					}
				}
			}
		}
	}

	return ptis;
}

std::vector<std::string> TellWhetherContainsPTI(const std::string& txt)
{
	std::vector<std::string> ptis; //The point of interest

	//Checking the text for points of interest
	for (int a = 0; a < txt.size(); )
	{
		//Checking if a word beggining with uppercase letters is found
		if (a != 0 && !HasFullstop(txt, a) && IsUppercase(&txt[a]))
		{
			std::string pti = ExtractPTI(txt, a);
			
			if(PtiExists(pti, ptis) > ptis.size())
				ptis.push_back(pti); //Getting the extracted PTI
			a += ptis[ptis.size() - 1].size();
			continue;
		}
		else if ((int)txt[a] == 34)
		{		
			ptis.push_back(ExtractPTI(txt, a + 1, 1));
			break;
		}

		++a;
	}

	return ptis;
}

bool SavePti(const std::string& filePth, std::vector<Pti>& ptis)
{
	std::fstream file(filePth, std::ios::in);

	if (!file.is_open())
		return false;

	int a = 0;
	while (true)
	{
		std::string buffer;

		std::getline(file, buffer);

		if (buffer == "#")
			break;

		if (buffer[0] == '+')
		{
			buffer.erase(0, 1);
			int index = PtiExists(buffer, ptis);
			if (index > ptis.size())
			{
				ptis.push_back(Pti(buffer, std::vector<std::string>()));
				a = ptis.size() - 1;
			}
			else
			{
				a = index;
			}
		}
		else
		{
			buffer.erase(0, 1);
			ptis[a].second.push_back(buffer);
		}
	}
	file.close();

	file = std::fstream(filePth, std::ios::out);

	if (!file.is_open())
		return false;

	for(int a = 0; a < ptis.size(); ++a)
	{
		file << "+" << ptis[a].first << "\n";
		for (std::string st : ptis[a].second)
		{
			file << "-" << st << "\n";
		}
	}

	file << "#";

	file.close();
}

int PtiExists(const std::string& pti, const std::vector<Pti>& ptis)
{
	for (int a = 0; a < ptis.size(); ++a)
	{
		if (Compare(pti, ptis[a].first))
			return a;
	}

	return (ptis.size() + 1);
}

int PtiExists(const std::string& pti, const std::vector<std::string>& ptis)
{
	for (int a = 0; a < ptis.size(); ++a)
	{
		if (Compare(pti, ptis[a]))
			return a;
	}

	return (ptis.size() + 1);
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
