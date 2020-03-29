#pragma once

#include <input.h>
#include <game.h>
#include <mainpage.h>

#include <interfaces/ipage.h>

class RegisterPage : public IPage
{
public:
	RegisterPage()
	{}

	RegisterPage(Buffer *buffer, Game *game)
	{
		m_game = game;
	}

	virtual void Draw() override
	{}

	virtual void Update() override;

private:
	const int ESC = 27;
	
	Game *m_game;
};