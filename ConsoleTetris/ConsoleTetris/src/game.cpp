#pragma once

#include <game.h>
#include <buffer.h>
#include <puzzle_builder.h>
#include <chrono>
#include <iostream>
#include <game_time.h>
#include <mainpage.h>
#include <registerpage.h>
#include <loginpage.h>

#include <algorithm>
#include <random>
#include <string>
#include <iostream>
#include <cstdlib>
// for <streamsize> 
#include<ios>      
// for numeric_limits 
#include<limits>

#undef max

Game::Game()
{
}

Game::Game(IBuilder *puzzleBuilder, Buffer *buffer, DataBase *dataBase) :
	m_gameOver(false)
	, m_puzzleBuilder(puzzleBuilder)
	, m_dataBase(dataBase)
	, m_buffer(buffer)
{
	m_isInGame = false;
	m_isInLogin = false;
	m_isInRegister = false;

	m_playerStats = Pair<std::string, int>{ "Player", 0 };
	m_loginInfo = Pair<std::string, std::string>{ "", "" };

	m_mainPage = new MainPage(buffer, this);
	m_registerPage = new RegisterPage(buffer, this);
	m_loginPage = new LoginPage(buffer, this);
	
	m_currentPage = ToIPage(m_mainPage);
}

Game::~Game()
{
	for (auto puzzle : m_updateables)
	{
		RemoveElementFromScene(puzzle.second);
	}
	RemoveElements();

	delete m_mainPage;
}

void Game::Run()
{
	while (true)
	{
		m_buffer->ClearBuffer(true);
		if (!m_isInGame && !m_isInLogin && !m_isInRegister)
		{
			m_buffer->ClearBuffer(true);
			m_currentPage->Update();
			if (m_isInGame)
				continue;
			m_currentPage->Draw();
		}
		else if (m_isInGame)
		{
			m_elementsToBeRemovedIds.clear();

			if (Input::GetKeyDown(ESC))
			{
				m_buffer->ClearBuffer(true);
				m_isInGame = false;
				for (auto elem : m_updateables)
				{
					m_elementsToBeRemovedIds.push_back(elem.first);
				}
				RemoveElements();

				m_buffer->ClearBuffer(true);
				continue;
			}

			if (m_gameOver)
			{
				GameOver();
			}

			
			// Time update
			game_time::Update();

			// State update
			for (auto elem : m_updateables)
			{
				elem.second->Update();
				if (m_gameOver)
				{
					break;
				}
			}
			DrawPlayerInfo();

			if (m_gameOver)
			{
				continue;
			}
			RemoveElements();

			// Graphics update
			m_buffer->ClearBuffer();


			for (auto elem : m_drawables)
			{
				elem.second->Draw();
			}

			DeleteFilledRows();
		}
		else if (m_isInLogin)
		{
		/*	m_buffer->ClearBuffer(true);
			m_buffer->Draw();
			Login();
			m_isInLogin = false;*/
		} 
		else if (m_isInRegister)
		{
			/*m_buffer->ClearBuffer(true);
			m_buffer->Draw();
			Register();
			m_isInRegister = false;
			IPage *p = dynamic_cast<IPage *>(m_mainPage);
			SetCurrentPage(p);*/
		}

		m_buffer->Draw();

	} // while!

} // Run!

void Game::Spawn()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);

	int index = gen() % m_puzzles.size();

	auto boundsX = m_buffer->GetBoundsX();
	int width = boundsX.y - boundsX.x - 2;
	int randPosX = (boundsX.x + 1) + (gen() % width);

	auto puzzleTemplate = m_puzzles.at(index);

	IElement *puzzle = m_puzzleBuilder
		->Build(
			puzzleTemplate->GetShape()
			, Pair<int, int>{randPosX, -puzzleTemplate->GetShapeSize().y}
	);

	AddElementToScene(puzzle);
}

void Game::GameOver()
{
	m_buffer->ClearBuffer(true);

	const int GameOverOffsetX = 42;
	const int GameOverOffsetY = 11;
	
	std::string  gameOverMessage = "GAME OVER(press ESC to quit to the main menu)";
	for (int i = 0; i < gameOverMessage.length(); ++i)
	{
		m_buffer->Set((wchar_t)gameOverMessage[i], GameOverOffsetX + i, GameOverOffsetY);
	}
	m_buffer->Draw();
}

void Game::AddElementToScene(IElement *element)
{
	m_updateables.insert(std::make_pair(element->GetId(), element));
	m_drawables.insert(std::make_pair(element->GetId(),
		dynamic_cast<IDrawable *>(element)));
}

void Game::RemoveElementFromScene(IElement *element)
{
	m_elementsToBeRemovedIds.push_back(element->GetId());
}

void Game::AddPuzzle(IElement *element)
{
	m_puzzles.push_back(element);
}

void Game::RemovePuzzle(IElement *element)
{
	m_puzzles.erase(std::find(m_puzzles.begin(), m_puzzles.end(), element));
}

void Game::FillCells(IElement *element)
{
	auto puzzle = dynamic_cast<Puzzle *>(element);
	auto shape = puzzle->GetShape();
	auto center = puzzle->GetCoord();
	auto shapeSize = puzzle->GetShapeSize();

	for (int y = -shapeSize.y / 2; y <= shapeSize.y / 2; ++y)
	{
		for (int x = -shapeSize.x / 2; x <= shapeSize.x / 2; ++x)
		{
			int cx = x + shapeSize.x / 2;
			int cy = y + shapeSize.y / 2;

			Pair<int, int> coord{ center.x + x, center.y + y };

			auto startY = m_buffer->GetBoundsY().x;

			if (L'#' == shape[cy * shapeSize.x + cx] && coord.y < startY)
			{
				m_gameOver = true;
				return;
			}

			if (m_buffer->InBounds(coord)
				&& L'#' == shape[cy * shapeSize.x + cx])
			{
				m_buffer->Set(L'*', coord.x, coord.y);
			}
		}
	}
}

bool Game::IsGameOver() const
{
	return m_gameOver;
}

void Game::SetGameOver(bool value)
{
	m_gameOver = value;
}

void Game::SetCurrentPage(IPage *value)
{
	m_currentPage = value;
}

void Game::StartGame()
{
	game_time::Init();
	m_buffer->ClearBuffer(true);
	m_buffer->DrawBorder();
	Spawn();
	m_playerStats.y = 0;
	m_gameOver = false;
	m_isInGame = true;
	m_buffer->Draw();
}

IPage *Game::ToIPage(MainPage *mainPage)
{
	return dynamic_cast<IPage *>(mainPage);
}

IPage *Game::ToIPage(LoginPage *loginPage)
{
	return dynamic_cast<IPage *>(loginPage);
}

IPage *Game::ToIPage(RegisterPage *registerPage)
{
	return dynamic_cast<IPage *>(registerPage);
}

void Game::RemoveElements()
{
	for (auto i : m_elementsToBeRemovedIds)
	{
		Puzzle *puzzle = dynamic_cast<Puzzle *>(m_updateables[i]);
		m_updateables.erase(i);
		m_drawables.erase(i);
		delete puzzle;
	}
}

bool Game::IsRowFilled(int row) const
{
	bool res = true;
	auto start = m_buffer->GetBoundsX().x;
	auto end = m_buffer->GetBoundsX().y;

	for (int x = start; x < end; ++x)
	{
		if (m_buffer->Get(x, row) != '*')
		{
			res = false;
		}
	}

	return res;
}

void Game::DeleteFilledRows()
{
	int startX = m_buffer->GetBoundsX().x;
	int endX = m_buffer->GetBoundsX().y;
	int startY = m_buffer->GetBoundsY().x;
	int endY = m_buffer->GetBoundsY().y;

	int curY = -1;
	for (int y = endY - 1; y >= startY; --y)
	{
		if (IsRowFilled(y))
		{
			curY = y;
			while (curY >= startY && IsRowFilled(curY))
			{
				for (int x = startX; x < endX; ++x)
				{
					m_buffer->Set(L' ', x, curY);
				}
				m_playerStats.y += ScoresForRow;
				--curY;
			}
		}
	}

	if (curY >= startY && curY < endY - 1)
	{
		for (int x = startX; x < endX; ++x)
		{
			for (int y = curY; y >= startY && m_buffer->Get(x, y) == '*'; --y)
			{
				int cy = y + 1;
				while (cy < endY && m_buffer->Get(x, cy) == L' ')
				{
					m_buffer->Set(L' ', x, cy - 1);
					m_buffer->Set(L'*', x, cy);
					++cy;
				}
			}
		}
	}
}

void Game::DrawPlayerInfo()
{
	const int offsetX = -10;
	const int offsetY = 1;

	std::string str = m_playerStats.x + ": " + std::to_string(m_playerStats.y);
	for (int i = 0; i < str.length(); ++i)
	{
		m_buffer->Set(str[i], m_buffer->GetBoundsX().x + offsetX + i, offsetY);
	}
}

bool Game::Register()
{
	string username;
	string email;
	string password;
	GetDataFromUserRegister(username, email, password);

	const std::string query = 
						 "INSERT INTO `users` (`ID`, `username`, `e-mail`, `password`, `score`) "
						 "VALUES (NULL, '" + username + "', '" + email + "', '" + password +"', '0')";
	try
	{
		m_dataBase->ExecuteUpdate(query);
	}
	catch(sql::SQLException &e)
	{
		return false;
	}

	std::cout << "Success\nPress ESC to go to the main menu";

	return true;
}

bool Game::Login()
{
	string username;
	string email;
	string password;
	
	GetDataFromUserLogin(username, email, password);

	m_playerStats.x = username;
	m_loginInfo.x = email;
	m_loginInfo.y = password;

	std::cin.clear();
	std::cin.ignore(10000, '\n');
	std::cout << "Logged in succuseful.\nPress ESC to go the main menu";
	m_buffer->ClearBuffer(true);
	m_buffer->Draw();
	return true;
}

help_types::ValidateResult Game::ValidateEnteredData(
	std::string username
	, std::string email
	, std::string password)
{
	const std::string query =
		"SELECT * "
		"FROM `users`"
		"WHERE `username` = '" + username + "'";

	sql::ResultSet *res = nullptr;
	try
	{
		res = m_dataBase->ExecuteQuery(query);
	}
	catch (sql::SQLException &e)
	{
		delete res;
		return help_types::ValidateResult::Error;
	}

	while (res->next())
	{
		std::string _email = res->getString("e-mail");
		std::string _password = res->getString("password");

		if (email != _email || password != _password)
		{
			delete res;
			return help_types::ValidateResult::NotCorrect;
		}
		else
		{
			delete res;
			return help_types::ValidateResult::Correct;
		}
	}

	delete res;
	return help_types::ValidateResult::NotExists;
}

help_types::ValidateResult Game::ValidateExistance(std::string username, std::string email)
{
	const std::string queryUsername =
		"SELECT * "
		"FROM `users`"
		"WHERE `username` = '" + username + "'";
	const std::string queryEmail =
		"SELECT * "
		"FROM `users`"
		"WHERE `username` = '" + username + "'";

	sql::ResultSet *res = nullptr;

	try
	{
		res = m_dataBase->ExecuteQuery(queryUsername);
	}
	catch (sql::SQLException &e)
	{
		delete res;
		return help_types::ValidateResult::Error;
	}

	if (res->next())
	{
		std::cout << "A user with such username already exists\n";
		delete res;
		return help_types::ValidateResult::Exists;
	}

	try
	{
		delete res;
		res = m_dataBase->ExecuteQuery(queryEmail);
	}
	catch (sql::SQLException &e)
	{
		delete res;
		return help_types::ValidateResult::Error;
	}

	if (res->next())
	{
		std::cout << "A user with such email already exists\n";
		delete res;
		return help_types::ValidateResult::Exists;
	}

	delete res;
	return help_types::ValidateResult::NotExists;
}

void Game::GetDataFromUserRegister(std::string &username, std::string &email, std::string &password)
{
	const string _username = "username";
	const string _email = "e-mail";
	const string _password = "password";

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)_username.length() + 1, 0 });
	std::cin >> username;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)_email.length() + 1, 1 });
	std::cin >> email;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)_password.length() + 1, 2 });
	std::cin >> password;

	if (ValidateExistance(username, email) != help_types::ValidateResult::NotExists)
	{
		std::cout << "Please, try again\n";
		GetDataFromUserRegister(username, email, password);
	}
}

void Game::GetDataFromUserLogin(std::string &username, std::string &email, std::string &password)
{
	const string _username = "username";
	const string _email = "e-mail";
	const string _password = "password";

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)_username.length() + 1, 0 });
	std::cin >> username;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)_email.length() + 1, 1 });
	std::cin >> email;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ (SHORT)_password.length() + 1, 2 });
	std::cin >> password;

	if (ValidateEnteredData(username, email, password) != help_types::ValidateResult::Correct)
	{
		std::cout << "Incorrect, try again\n";
		GetDataFromUserLogin(username, email, password);
	}
	return;
}






