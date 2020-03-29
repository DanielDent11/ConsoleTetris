#pragma once

#include <game.h>
#include <mainpage.h>

#include <interfaces/ipage.h>

class LoginPage : public IPage
{
public:
	LoginPage()
	{}

	LoginPage(Buffer *buffer, Game *game)
	{
		m_game = game;
	}

	virtual void Draw() override
	{
		using namespace std;
		const string username = "username";
		const string email = "e-mail";
		const string password = "password";
		
		WriteStrToBuffer(username, 0, 0);
		WriteStrToBuffer(email, 0, 1);
		WriteStrToBuffer(password, 0, 2);

		m_game->Login();
	}

	virtual void Update() override;
	

private:
	const int ESC = 27;

	Game *m_game;

	void WriteStrToBuffer(const string &str, int x, int y)
	{
		for (int i = 0; i < str.length(); ++i)
		{
			m_game->m_buffer->Set(str[i], x + i, y);
		}
	}
};