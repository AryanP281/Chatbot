#pragma once

/********************Preprocessor Directives*****************************/
#include<string>
#include<vector>

/********************Typedef*****************************/
typedef std::vector<std::string> Conversation; //A conversation stored in the bots memory
typedef std::vector<std::pair<std::string, std::vector<std::string>>> PTIs; /*A list of the
Points Of Interest(names of movies, songs, etc) and the sentences containing them*/
typedef std::pair<std::string, std::vector<std::string>> Pti; /*A Point of interest*/

/********************Constants*****************************/
const float IDEAL_SIMILARITY = 75.0f; /*If the similarity factor of the texts is greater than or
equal to this value, then they are considered similar*/ 
const std::string DEF_REPLY = "Oh" ; /*The default reply given by the bot if it is not able to find
a reply*/

/********************Classes*****************************/
class ChatBot
{
private:
	//Private Variables
	const char* FILE_PATH; //The path of the Initialization File
	std::vector<Conversation> convs; //The past conversations that the bot has had
	Conversation currentConvs; //A record of the conversation the bot is currently having
	PTIs ptis; //The points of interest
	std::vector<int> frequencies; //The no. of times statements have been encountered
	std::pair<float, std::string> highestSimFac; /*The max similarity factor which didnt pass
	the limit and its reply*/

	//Private Functions
	bool LoadConversationData(); //Loads the conversation data from the bots past conversations
	int ReadInitializationFile(); //Reads the Initialization File and returns the data from it
	bool ReadConversationFile(std::string& filePath); /*Reads a conversation and loads the conversation
	data present in it to memory*/
	bool LoadPointsOfInterst(const std::string& filePath); /*Loads the information regarding the
	the points of interest*/
	std::string IsolateText(std::string& str); /*Removes the identifier and the frequency from the statements
	found in conversation files*/
	float Compare(const std::string& txt, const std::string& compareTo); /*Compares the texts
	and returnsthe similarity factor*/
	float Test1(const std::string& txt, const std::string& compareTo); /*Compares the strings letter by
	letter and returns the similarity factor*/
	float Test2(const std::string& txt, const std::string& compareTo); /*Counts the no. of common letters
	in the strings and returns the similarity factor*/
	std::vector<std::string> TellWhetherContainsPTI(const std::string& txt); /*Tells whether a statement contains
	a point of interest and returns it*/
	std::string ExtractPTI(const std::string& txt, int begIndex, char flag = 0); //Extracts the point of interest from the text
	int PtiExists(const std::string& pti, const std::vector<Pti>& ptis); /*Returns the index of the
	pti in the list of ptis*/
	int PtiExists(const std::string& pti, const std::vector<std::string>& ptis);/*Returns the
	index of the pti in the list of ptis*/
	std::string ContainsPTI(const std::vector<std::string>& ptiList, const std::string& txt); /*Handles statements 
	which contain points of interest*/
	std::string DoesNotContainPTI(const std::string& txt); /*Handles the statements which do not contain
	points of interest*/
	std::vector<std::pair<float, std::string>> GetReplies(const std::string& txt); /*Collects 
	a set of replies for the user's statement*/
	std::vector<std::string> CurateReplies(std::vector<std::pair<float, std::string>> replies); /*
	Curates a set of replies to given from the list of replies*/
	std::string ReturnRandomReply(); /*Returns a random reply if the bot doesn't find any match*/
	void SaveData(); //Saves the data from the current chat session
	void SaveConversation(); //Saves the current conversation that the bot has had
	bool SavePtis(const std::string& filePath); /*Saves the PTIs encountered in the current chat 
	session*/
	bool UpdateInitializationFile(); /*Updates the file containing the initialization data*/

public:
	//Constructors And Destructors
	ChatBot();
	~ChatBot();

	//Methods
	std::string Chat(const std::string& txt); //Analyzes what the user has said and returns a reply
};

