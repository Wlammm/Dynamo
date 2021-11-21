#pragma once

#define CONSOLE_TEXT_COLOR_RED 12
#define CONSOLE_TEXT_COLOR_YELLOW 14
#define CONSOLE_TEXT_COLOR_GREEN 10
#define CONSOLE_TEXT_COLOR_WHITE 15

namespace Dynamo
{
	class Console
	{
	public:
		static void Create();
		static void Destroy();

		static void Log(const char* aMsg, ...);
		static void LogOnce(const char* aMsg, ...);

		static void LogVector(const char* aMsg, const Vec2f& aVec);
		static void LogVector(const char* aMsg, const Vec3f& aVec);
		static void LogVector(const char* aMsg, const Vec4f& aVec);
		static void ErrorLog(const char* aMsg, ...);

	private:
		static void SetConsoleColor(const int aColor);

		static std::unordered_set<const char*> myOnceMessages;
	};
}