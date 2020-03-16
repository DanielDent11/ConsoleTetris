#pragma once

#include <Pair.h>
#include <interfaces/IDrawable.h>
#include <interfaces/IElement.h>

#include <Windows.h>

class Buffer : IDrawable
{
public:
	const wchar_t FILLING_SYMBOL = 0x2588;
public:
	Buffer(int w, int h) :
		m_width(w)
		, m_height(h)
	{
		hStdout = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE
			, 0
			, NULL
			, CONSOLE_TEXTMODE_BUFFER
			, NULL);
		SetConsoleActiveScreenBuffer(hStdout);
		m_buffer = new wchar_t[m_width * m_height + 1];
	}
	~Buffer() { delete[] m_buffer; }

	virtual void Draw() override
	{
		DrawBorder();
		m_buffer[m_width * m_height] = L'\0';
		WriteConsoleOutputCharacter(hStdout, m_buffer, m_width * m_height + 1, { 0,0 }, &m_bytesWritten);
	}

	wchar_t Get(int x, int y) { return m_buffer[y * (m_width)+x]; }

	void Set(wchar_t val, int x, int y) { m_buffer[y * (m_width)+x] = val; }

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
	void DrawBorder()
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_BORDERWIDTH.x; ++x)
			{
				m_buffer[y * m_width + x] = m_SOLIDSHADE;
			}
		}


		for (int y = 0; y < m_height; ++y)
		{
			for (int x = m_width - 1; x >= m_width - m_BORDERWIDTH.x; --x)
			{
				m_buffer[y * m_width + x] = m_SOLIDSHADE;
			}
		}

		for (int y = m_height - m_BORDERWIDTH.y; y < m_height; ++y)
		{
			for (int x = m_BORDERWIDTH.x; x < m_width - m_BORDERWIDTH.x; ++x)
			{
				m_buffer[y * m_width + x] = m_SOLIDSHADE;
			}
		}
	}
private:
	const Pair<int, int> m_BORDERWIDTH{ 40, 3 };
	const wchar_t m_SOLIDSHADE = 0x2591;

	int m_width;
	int m_height;
	wchar_t *m_buffer;
	HANDLE hStdout;
	DWORD m_bytesWritten;
};