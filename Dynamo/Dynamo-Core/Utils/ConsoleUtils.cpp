#include "pch.h"
#include "ConsoleUtils.h"
#include <iostream>

namespace Dynamo
{
	std::unordered_set<const char*> Console::myOnceMessages;

	void Console::Create()
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		setbuf(stdin, NULL);
		setbuf(stdout, NULL);
		setbuf(stderr, NULL);
	}

	void Console::Destroy()
	{
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
	}

	void Console::SetConsoleColor(const int aColor)
	{
		HANDLE hConsole;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		if (!hConsole)
			return;

		SetConsoleTextAttribute(hConsole, (WORD)aColor);
	}

	void Console::Log(const char* aMsg, ...)
	{
		SetConsoleColor(CONSOLE_TEXT_COLOR_WHITE);
		printf("[Dynamo]: ");
		va_list argptr;
		va_start(argptr, aMsg);
		vfprintf(stderr, aMsg, argptr);
		va_end(argptr);
		std::cout << std::endl;
	}

	void Console::LogOnce(const char* aMsg, ...)
	{
		if (myOnceMessages.count(aMsg))
			return;

		myOnceMessages.insert(aMsg);

		SetConsoleColor(CONSOLE_TEXT_COLOR_WHITE);
		printf("[Dynamo]: ");
		va_list argptr;
		va_start(argptr, aMsg);
		vfprintf(stderr, aMsg, argptr);
		va_end(argptr);
		std::cout << std::endl;
	}

	void Console::LogVector(const char* aMsg, const Vec2f& aVec)
	{
		std::string msg;
		msg += aMsg;
		msg += "x: %f, y: %f";
		Log(msg.c_str(), aVec.x, aVec.y);
	}

	void Console::LogVector(const char* aMsg, const Vec3f& aVec)
	{
		std::string msg;
		msg += aMsg;
		msg += "x: %f, y: %f, z: %f";
		Log(msg.c_str(), aVec.x, aVec.y, aVec.z);
	}

	void Console::LogVector(const char* aMsg, const Vec4f& aVec)
	{
		std::string msg;
		msg += aMsg;
		msg += "x: %f, y: %f, z: %f, w: %f";
		Log(msg.c_str(), aVec.x, aVec.y, aVec.z, aVec.w);
	}

	void Console::ErrorLog(const char* aMsg, ...)
	{
		SetConsoleColor(CONSOLE_TEXT_COLOR_RED);
		printf("[Dynamo]: ");
		va_list argptr;
		va_start(argptr, aMsg);
		vfprintf(stderr, aMsg, argptr);
		va_end(argptr);
		std::cout << std::endl;
	}
}
