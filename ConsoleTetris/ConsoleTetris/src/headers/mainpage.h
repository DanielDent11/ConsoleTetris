#pragma once

#include <interfaces/ipage.h>

#include <buffer.h>
#include <input.h>
#include <game.h>
#include <loginpage.h>
#include <registerpage.h>

#include <vector>
#include <thread>

class MainPage : public IPage
{
public:
	MainPage()
	{}

	MainPage(Buffer *buffer, Game *game)
	{
		m_buffer = buffer;
		m_game = game;
		m_isLoggedIn = false;
	}

	virtual void Draw() override
	{
		int startX = m_buffer->GetBoundsX().x;
		int endX = m_buffer->GetBoundsX().y;
		int startY = m_buffer->GetBoundsY().x;
		int endY = m_buffer->GetBoundsY().y;

		int width = endX - startX;
		int height = endY - startY;

		const int LogoOffsetX = -18;
		const int LogoOffsetY = 7;

		DrawLogo(startX + LogoOffsetX, startY + LogoOffsetY);

		const int offsetX = 7;
		const int offsetY = 15;

		for (int i = 0; i < m_links.size(); ++i)
		{
			if (i == m_selectedLink)
			{
				m_buffer->Set(L'>', startX + offsetX - 1, startY + offsetY + i);
			}
			for (int j = 0; j < m_links[i].first.length(); ++j)
			{
				m_buffer->Set((wchar_t)m_links[i].first[j], startX + offsetX + j, startY + offsetY + i);
			}
		}
	}

	virtual void Update() override
	{
		if (m_game->m_playerStats.x == "Player" && !m_isLoggedIn)
		{
			m_links.clear();
			IPage *p = m_game->ToIPage(m_game->m_loginPage);
			m_links.push_back(std::make_pair("LOGIN", p));
			p = m_game->ToIPage(m_game->m_registerPage);
			m_links.push_back(std::make_pair("REGISTER", p));
			m_links.push_back(std::make_pair("EXIT", nullptr));
			m_selectedLink = 0;
			m_isLoggedIn = true;
		}
		else if (m_game->m_playerStats.x != "Player" && m_isLoggedIn)
		{
			m_links.clear();
			m_links.push_back(std::make_pair("START", nullptr));
			m_links.push_back(std::make_pair("EXIT", nullptr));
			m_selectedLink = 0;
			m_isLoggedIn = false;
		}

		if (Input::GetKeyDown('S'))
		{
			Down();
			std::this_thread::sleep_for(std::chrono::duration<float>(0.2));
		}
		else if (Input::GetKeyDown('W'))
		{
			Up();
			std::this_thread::sleep_for(std::chrono::duration<float>(0.2));
		}

		else if (Input::GetKeyDown(ENTER))
		{
			if (m_selectedLink == -1)
			{
				std::cin.clear();
				std::cin.ignore(10000, '\n');
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
				m_selectedLink = 0;
				return;
			}
			if (m_links[m_selectedLink].first == "EXIT")
			{
				exit(0);
			}
			else if (m_links[m_selectedLink].first == "START")
			{
				m_game->StartGame();
			}
			else if (m_links[m_selectedLink].first == "LOGIN")
			{
				m_buffer->ClearBuffer(true);
				m_buffer->Draw();
				m_game->m_currentPage = m_game->ToIPage(m_game->m_loginPage);
				m_selectedLink = -1;
			}
			else if (m_links[m_selectedLink].first == "REGISTER")
			{
				m_buffer->ClearBuffer(true);
				m_buffer->Draw();
				m_game->m_currentPage = m_game->ToIPage(m_game->m_registerPage);
				m_selectedLink = -1;
			}

			std::cin.clear();
			std::cin.ignore(10000, '\n');
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			//FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
			std::this_thread::sleep_for(std::chrono::duration<float>(0.5));
		}
	}

	void SetBuffer(Buffer *value)
	{
		m_buffer = value;
	}
private:
	bool m_isLoggedIn;
	const int ENTER = 13;
	Buffer *m_buffer;
	Game *m_game;
	std::vector < std::pair<std::string, IPage *> > m_links;
	int m_selectedLink;

	void Down()
	{
		if (m_selectedLink + 1 >= m_links.size())
		{
			return;
		}
		++m_selectedLink;
	}

	void Up()
	{
		if (m_selectedLink == 0)
		{
			return;
		}
		--m_selectedLink;
	}

	void DrawLogo(int startX, int startY)
	{
		const wchar_t *LOGO =
		   L"##########.#######.##########.#######.##....####.#######."
			"....##.....##..........##.....##...##.##...##.##.##......"
			"....##.....######......##.....#######.##..##..##.##......"
			"....##.....##..........##.....##......##.##...##.##......"
			"....##.....#######.....##.....##......####....##.#######."
			".........................................................";

		for (int y = 0; y < 6; ++y)
		{
			for (int x = 0; x < 57; ++x)
			{
				if (LOGO[y * 57 + x] == L'#')
				{
					m_buffer->Set(L'#', startX + x, startY + y);
				}
			}
		}
	}
};
