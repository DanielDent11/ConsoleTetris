#pragma once

#include <Windows.h>

class Input
{
public:
	static bool GetKeyDown(unsigned short key)
	{
		return (GetAsyncKeyState(key) & 0x8000);
	}
};