
/********************Preprocessor Directives*****************************/
#include "stdafx.h"
#include <string>
#include "Interface.h"

/********************Functions*****************************/
void GenerateBot()
{
	bot = new ChatBot();
}

int PingReply(const char* statement)
{
	reply = bot->Chat(std::string(statement)); //Getting the reply from the bot

	return reply.size();
}

void GetReply(char* statement, char* replyBff)
{
	//Editing the buffer
	for (int a = 0; a < reply.size(); ++a)
	{
		replyBff[a] = reply[a];
	}
	replyBff[reply.size()] = '\0'; //Adding the terminating character
}

void CleanBot()
{
	delete bot; //Deallocating the allocated memory
}
