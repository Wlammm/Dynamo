#pragma once

#include <DbgHelp.h>

namespace Dynamo
{
	class CrashHandler
	{
	public:
		static LONG WINAPI ExceptionFilterFunction(_EXCEPTION_POINTERS* anException);
		static void CreateMiniDump(EXCEPTION_POINTERS* someExceptionPointers, MINIDUMP_TYPE someFlags);
	};
}