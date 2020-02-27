#pragma once

#include <interfaces/IDrawable.h>

#include <Windows.h>

class Buffer : IDrawable
{
public:
	Buffer(int w, int h) :
		m_width(w)
		, m_height(h)
	{
		hStdout = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(hStdout);
		m_buffer = new wchar_t[m_width * m_height];
	}
	~Buffer() { delete[] m_buffer; }

	virtual void Draw() override
	{
		m_buffer[m_width * m_height - 1] = L'\0';
		WriteConsoleOutputCharacter(hStdout, m_buffer, m_width * m_height, {0,0}, &m_bytesWritten);
	}

	wchar_t Get(int x, int y) { return m_buffer[y * (m_width) + x]; }

	void Set(wchar_t val, int x, int y) { m_buffer[y * (m_width) + x] = val; }

	void ClearBuffer()
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				m_buffer[m_width * y + x] = L' ';
			}
		}
	}

private:
	int m_width;
	int m_height;
	wchar_t *m_buffer;
	HANDLE hStdout;
	DWORD m_bytesWritten;
};