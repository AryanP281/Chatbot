
/********************Preprocessor Directives*****************************/
#include "stdafx.h"
#include <fstream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Misc.h"
#include "ChatBot.h"
using namespace std;

/********************Constructors And Destructors*****************************/
ChatBot::ChatBot() : FILE_PATH("Data/Initialization file.txt")
{
	//Loading the conversation data from past conversations
	if (!LoadConversationData())
	{
		cout << "\nUnable to Load Data\n";
		exit(0);
	}

	//Loading the Points Of Interest
	if (!LoadPointsOfInterst("Data/PTIs.txt"))
	{
		cout << "\nUnable To Load Data\n";
		exit(0);
	}
}

ChatBot::~ChatBot()
{
	//SaveData();
}

/********************Private Functions*****************************/
bool ChatBot::LoadConversationData()
{
	int noOfFiles = ReadInitializationFile(); //The no. of conversation files the bot has stored

	//Reading the data from the conversation files
	for (int a = 1; a <= noOfFiles; ++a)
	{
		std::string filePath = "Data/" + to_string(a) + ".txt"; //Constructing the file name

		if (!ReadConversationFile(filePath)) //Checking if the conversation was successfully read
			return false;
	}

	return true;
}

int ChatBot::ReadInitializationFile()
{
	//Local Variables
	int noOfFiles = 0; //The no. of conversation files
	
	//Opening the Initializing File
	fstream initializationFile(this->FILE_PATH, ios::in);

	//Checking if the Initializing File has opened
	if (!initializationFile.is_open())
		return 0;

	//Reading the data
	initializationFile >> noOfFiles;

	//Getting the frequently encountered texts
	//LoadFreqTxts(initializationFile);

	initializationFile.close(); //Closing the file as we are done with it

	return noOfFiles;
}

bool ChatBot::ReadConversationFile(std::string& filePath)
{
	fstream file(filePath, ios::in); //Opening the file
	Conversation conv; //The conversations

	if (!file.is_open()) //Checking if the file has opened
		return false;

	while (true)
	{	
		//char identifier; //A identifer telling whether the statement was said by a human or the bot
		std::string text; //The actual statement
		std::string buffer; //A buffer for holding a line in the conversation

		getline(file, buffer); //Reading a line

		//Checking if the file has ended
		if (buffer == "#")
			break;

		//identifier = buffer[0]; //Getting the identifier
		text = buffer; //Extracting the statement

		//Saving the statement from the conversation
		conv.push_back(text);
	}

	//Adding the conversation to the list of conversations
	this->convs.push_back(conv);

	file.close();

	return true;
}

bool ChatBot::LoadPointsOfInterst(const string& filePath)
{
	fstream file(filePath, ios::in); //Opening the file

	if (!file.is_open()) //Checking if the file has opened
		return false;

	//Reading the points of interests and the accompanying statements
	while (true)
	{
		string line; //A line from the file

		getline(file, line); //Reading a line from the file

		//Checking if its the end of the file
		if (line == "#")
			break;

		if (line[0] == '+') //Checking if the sentence is a point of interest or a statement containing the PTI
		{
			line.erase(0, 1); //Removing the identifier
			ptis.push_back(pair<string, vector<string>>(line, vector<string>())); /*Adding the
			point of interest*/
		}
		else if (line[0] == '-')
		{
			line.erase(0, 1); //Removing the identifier
			ptis[ptis.size() - 1].second.push_back(line); //Adding the statement containing the PTI
		}
	}

	file.close();

	return true;
}

std::string ChatBot::IsolateText(std::string& str)
{
	int pos = 0; //The position from where the text begins
	std::string isolatedStr; //A string containing only the required text

	//Getting the start of the text'
	for (int a = 0; true; ++a)
	{
		if (str[a] == ' ')
		{
			pos = a + 1;
			break;
		}
	}

	//Creating a new string containing only the text
	for (int b = pos; b < str.size(); ++b)
	{
		isolatedStr += str[b];
	}

	return isolatedStr;
}

float ChatBot::Compare(const std::string& txt, const std::string& compareTo)
{
	float xCoord = Test1(txt, compareTo); //The result of the 1st similarity test
 	float yCoord = Test2(txt, compareTo); //The result of the 2nd similarity test

	float similarityFac = (sqrt(pow(xCoord, 2) + pow(yCoord, 2)) / sqrt(1 + 1)) * 100; /*Calculating 
	the final similarity factor*/

	return similarityFac;

	/*//Checking if the texts are similar
	if (similarityFac >= IDEAL_SIMILARITY)
		return true;



	return false;*/

}

float ChatBot::Test1(const std::string& txt, const std::string& compareTo)
{
	float res = 0;

	//Comparing the words letter by letter
	for (int a = 0; a < txt.size() && a < compareTo.size(); ++a)
	{
		if (tolower(txt[a]) == tolower(compareTo[a]))
			res++;
	}

	float similarityFactor;
	txt.size() >= compareTo.size() ? similarityFactor = res / txt.size() : similarityFactor = res / compareTo.size();

	return similarityFactor; //Returning the similarity factor
}

float ChatBot::Test2(const std::string& txt, const std::string& compareTo)
{
	std::string text = ProcessString(txt); //Gets a processed version of the string for analysis
	std::string compareToTxt = ProcessString(compareTo); /*Gets a processed version of the string to
	be compared to*/
	float noOfCommonChar = 0; //The number of common characters that the two strings have
	vector<char> includededChs; //The characters which have already been counted

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

std::vector<std::pair<float, std::string>> ChatBot::GetReplies(const std::string& txt)
{
	vector<pair<float, string>> replies; //The replies to the given statement

	//Resetting the highest similarity factor
	highestSimFac = pair<float, string>(0.0f, "");

	//Comparing with the previous conversations
	for (int a = 0; a < convs.size(); ++a)
	{
		for (int b = 0; b < convs[a].size(); ++b)
		{
			float similarityFac = Compare(txt, convs[a][b]);
			if (similarityFac >= IDEAL_SIMILARITY)
			{
				if (b < convs[a].size() - 2) //Making sure that it is not the end of the conversation
				{
					replies.push_back(pair<float,string>(similarityFac, convs[a][b + 1]));
					replies.push_back(pair<float, string>(similarityFac, convs[a][b + 2]));
				}
				else if(b != convs[a].size() - 1 )
					replies.push_back(pair<float,string>(similarityFac, convs[a][b + 1]));
			}
			else if (similarityFac > highestSimFac.first)
			{
				if (b != convs[a].size() - 1)
				{
					highestSimFac.first = similarityFac;
					highestSimFac.second = convs[a][b + 1];
				}
			}
		}
	}

	//Checking for replies from the current conversation
	for (int a = 0; a < currentConvs.size(); ++a)
	{
		float similarityFac = Compare(txt, currentConvs[a]);
		if (similarityFac >= IDEAL_SIMILARITY)
		{
			if(a != currentConvs.size() - 1)
				replies.push_back(pair<float,string>(similarityFac, currentConvs[a + 1]));
		}
	}

	//Checking if no reply was found
	if (replies.size() == 0)
		replies.push_back(pair<float,string>(highestSimFac.first,highestSimFac.second)); /*Returning the most similar reply*/

	return replies;
}

std::vector<std::string> ChatBot::CurateReplies(std::vector<std::pair<float, std::string>> replies)
{
	const short REPLY_SELECTION_RATIO = 2; //The ratio in which the number of replies will be selected

	//Checking if the replies list has enough elements to pass the selection
	if (REPLY_SELECTION_RATIO >= replies.size()) 
	{
		vector<string> reps;
		for (int a = 0; a < replies.size(); ++a)
		{
			reps.push_back(replies[a].second);
		}
		return reps;
	}

	//Arranging the replies in the descending order of similarity 
	for (int a = 0; a < replies.size(); ++a)
	{
		for (int b = a + 1; b < replies.size(); ++b)
		{
			if (replies[b].first > replies[a].first)
			{
				pair<float, string> temp = replies[a];
				replies[a] = replies[b];
				replies[b] = temp;
			}
		}
	}

	//Curating the replies according to the selection ratio
	vector<string> reps;
	for (int a = 0; a < replies.size() / REPLY_SELECTION_RATIO; ++a)
	{
		reps.push_back(replies[a].second);
	}

	return reps;
}

string ChatBot::ReturnRandomReply()
{
	srand((int)time(0));
	int a = rand() % convs.size();
	int b = rand() % convs[a].size();

	return convs[a].at(b);
}

vector<string> ChatBot::TellWhetherContainsPTI(const string& txt)
{
	std::vector<std::string> ptis; //The point of interest

	 //Checking the text for points of interest
	for (int a = 0; a < txt.size(); )
	{
		//Checking if a word beggining with uppercase letters is found
		if (a != 0 && !HasFullstop(txt, a) && IsUppercase(&txt[a]))
		{
			std::string pti = ExtractPTI(txt, a);

			if (PtiExists(pti, ptis) > ptis.size())
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

string ChatBot::ExtractPTI(const string& txt, int begIndex, char flag)
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

int ChatBot::PtiExists(const std::string& pti, const std::vector<Pti>& ptis)
{
	for (int a = 0; a < ptis.size(); ++a)
	{
		if (Compare(pti, ptis[a].first) >= IDEAL_SIMILARITY)
			return a;
	}

	return (ptis.size() + 1);
}

int ChatBot::PtiExists(const std::string& pti, const std::vector<std::string>& ptis)
{
	for (int a = 0; a < ptis.size(); ++a)
	{
		if (Compare(pti, ptis[a]) >= IDEAL_SIMILARITY)
			return a;
	}

	return (ptis.size() + 1);
}

string ChatBot::ContainsPTI(const vector<string>& ptiList, const string& txt)
{
	vector<string> replies; //The replies for the PTI
	
	//Comparing with the know PTIs
	for (string pti : ptiList)
	{
		for (int a = 0; a < ptis.size(); ++a)
		{
			if (Compare(pti, ptis[a].first) >= IDEAL_SIMILARITY)
			{
				replies = ptis[a].second;
				ptis[a].second.push_back(txt); //Adding the statement to the list of statements contianing the PTI
				break;
			}
		}
	}

	if (replies.size() != 0) //Checking if the PTI exists
	{
		//Randomly selecting a reply
		srand(time(0)); //Seeding the number generator
		return replies[rand() % replies.size()];
	}
	else
		return "";
}

string ChatBot::DoesNotContainPTI(const string& txt)
{
	string reply; //The reply to be returned
	vector<pair<float,string>> replies = GetReplies(txt);  //Getting the replies to the text
	vector<string> curatedReplies = CurateReplies(replies); //Getting the curated replies

	 //Randomly selecting a reply
	srand((int)time(0)); //Seeding the random-number generator
	int index = (rand() % curatedReplies.size()); //The index of the reply to return
	reply = curatedReplies.at(index); //Getting the reply

	//Recording the conversation
	currentConvs.push_back(txt); //Recording the users statement
	if(reply != DEF_REPLY)
		currentConvs.push_back(reply); //Recording the bots reply

	return reply;
}

void ChatBot::SaveData()
{
	//Saving the conversation
	SaveConversation(); 

	//Saving the pti
	if (!SavePtis("Data/PTIs.txt"))
		cout << "\nUnable to save PTIs";

	//Updating the initialization file
	if (currentConvs.size() != 0)
	{
		if (!UpdateInitializationFile())
			cout << "Unable to update Initialization file!";
	}
}

void ChatBot::SaveConversation()
{
	string filePath = "Data/" + to_string((convs.size() + 1)) + ".txt"; /*Initializing the filepath*/

	fstream file(filePath, ios::out | ios::trunc); //Opening the file

	for (int a = 0; a < currentConvs.size(); ++a)
	{
		string txt;
		txt += currentConvs[a] + "\n";
		EliminateTrailingWhitespaces(txt);
		file << txt;
	}

	file << "#"; /*Adding the character which marks the end of the file*/

	file.close();
}

bool ChatBot::SavePtis(const string& filePath)
{
	fstream file = std::fstream(filePath, std::ios::out);

	if (!file.is_open())
		return false;

	for (int a = 0; a < ptis.size(); ++a)
	{
		file << "+" << ptis[a].first << "\n";
		for (std::string st : ptis[a].second)
		{
			file << "-" << st << "\n";
		}
	}

	file << "#";

	file.close();

	return true;
}

bool ChatBot::UpdateInitializationFile()
{
	fstream file(FILE_PATH, ios::out | ios::trunc);

	if (!file.is_open())
		return false;

	file << convs.size() + 1;

	file.close();
}

/********************Methods*****************************/
string ChatBot::Chat(const std::string& txt)
{
	string reply; //The reply from the bot
	vector<string> ptiList = TellWhetherContainsPTI(txt);

	if (ptiList.size() == 0)
	{
		reply = DoesNotContainPTI(txt);
	}
	else
	{
		reply = ContainsPTI(ptiList, txt);

		//Checking if the PTI has been encountered before
		if (reply == "")
		{
			reply = DoesNotContainPTI(txt); //Getting a normal reply

			//Saving the PTI and the statement containing it
			for (string pti : ptiList)
			{
				int index = PtiExists(pti, ptis); /*Checking if the pti already exists in the list
				of ptis and if yes, getting its position*/
				
				if (index > ptis.size())
				{
					ptis.push_back(pair<string, vector<string>>(pti, vector<string>()));
					ptis[ptis.size() - 1].second.push_back(txt);
				}
				else
				{
					ptis[index].second.push_back(txt);
				}
			}
		}
	}

	return reply;
}
