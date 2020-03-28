#pragma once

#include <help_types.h>
#include <interfaces/idrawable.h>
#include <interfaces/ielement.h>

#include <iostream>
#include <unordered_map>

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
		// Left inclusive, right exclusive
		m_boundsX.x = m_BORDERWIDTH.x;
		m_boundsX.y = m_width - m_BORDERWIDTH.x;
		m_boundsY.x = 0;
		m_boundsY.y = m_height - m_BORDERWIDTH.y;

		hStdout = CreateConsoleScreenBuffer(
			GENERIC_READ | GENERIC_WRITE
			, 0
			, NULL
			, CONSOLE_TEXTMODE_BUFFER
			, NULL);
		SetConsoleActiveScreenBuffer(hStdout);
		m_buffer = new wchar_t[m_width * m_height + 1];
		for (int i = 0; i < m_width * m_height + 1; ++i)
		{
			m_buffer[i] = L' ';
		}

		m_convertedBuffer = new wchar_t[m_width * m_height + 1];
	}
	~Buffer()
	{
		delete[] m_buffer;
	}

	virtual void Draw() override
	{
		m_buffer[m_width * m_height] = L'\0';
		ConvertToDrawingSymbols();
		WriteConsoleOutputCharacter(hStdout, m_convertedBuffer, m_width * m_height + 1, { 0,0 }, &m_bytesWritten);
	}

	wchar_t Get(int x, int y)
	{
		return m_buffer[y * (m_width)+x];
	}

	void Set(wchar_t val, int x, int y)
	{
		if (y < 0)
		{
			return;
		}

		m_buffer[y * (m_width) + x] = val;
	}

	void ClearBuffer()
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				if (m_buffer[m_width * y + x] == L'#')
				{
					m_buffer[m_width * y + x] = L' ';
				}
			}
		}
	}

	Pair<int, int> GetBoundsX() const
	{
		return m_boundsX;
	}

	Pair<int, int> GetBoundsY() const
	{
		return m_boundsY;
	}

	bool InBounds(Pair<int, int> coord) const
	{
		auto boundsX = GetBoundsX();
		auto boundsY = GetBoundsY();

		return (coord.x >= boundsX.x && coord.x <= boundsX.y
			&& coord.y <= boundsY.y);
	}

	void DrawBorder()
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_BORDERWIDTH.x; ++x)
			{
				m_buffer[y * m_width + x] = L'b';
			}
		}


		for (int y = 0; y < m_height; ++y)
		{
			for (int x = m_width - 1; x >= m_width - m_BORDERWIDTH.x; --x)
			{
				m_buffer[y * m_width + x] = L'b';
			}
		}

		for (int y = m_height - m_BORDERWIDTH.y; y < m_height; ++y)
		{
			for (int x = m_BORDERWIDTH.x; x < m_width - m_BORDERWIDTH.x; ++x)
			{
				m_buffer[y * m_width + x] = L'b';
			}
		}
	}

private:
	const Pair<int, int> m_BORDERWIDTH{ 50, 15 };
	const wchar_t m_SOLIDSHADE = 0x2591;
	std::unordered_map<wchar_t, wchar_t> m_DRAWING_SYMBOLS_TABLE =
	{
		{L'b', 0x2591},
		{L'*', 0x2588},
		{L'#', 0x2588}
	};
	int m_width;
	int m_height;
	wchar_t *m_buffer;
	wchar_t *m_convertedBuffer;
	Pair<int, int> m_boundsX;
	Pair<int, int> m_boundsY;
	HANDLE hStdout;
	DWORD m_bytesWritten;
private:
	void ConvertToDrawingSymbols()
	{
		for (int i = 0; i < m_width * m_height + 1; ++i)
		{
			if (m_DRAWING_SYMBOLS_TABLE.count(m_buffer[i]))
			{
				m_convertedBuffer[i] = m_DRAWING_SYMBOLS_TABLE[m_buffer[i]];
			}
			else
			{
				m_convertedBuffer[i] = m_buffer[i];
			}
		}
	}
};