#pragma once

/********************Preprocessor Directives*****************************/
#include "ChatBot.h"

/********************Variables*****************************/
ChatBot* bot;
std::string reply;

/********************Functions*****************************/
extern "C"
{
	_declspec(dllexport) void GenerateBot(); /*Returns an instance of a chatbot to the GUI
program*/
	_declspec(dllexport) int PingReply(const char* statement); /*Returns the length of the reply*/
	_declspec(dllexport) void GetReply(char* statement, char* replyBff); /*Gets
	a reply from the provided bot and stores it in the provided buffer*/
	_declspec(dllexport) void CleanBot(); /*Deletes the instance of the bot after use*/
}


