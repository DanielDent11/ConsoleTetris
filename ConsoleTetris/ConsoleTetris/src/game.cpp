#pragma once

#include <game.h>
#include <buffer.h>
#include <puzzle_builder.h>
#include <chrono>
#include <iostream>
#include <game_time.h>
#include <mainpage.h>

#include <algorithm>
#include <random>
#include <string>

Game::Game()
{
}

Game::Game(IBuilder *puzzleBuilder, Buffer *buffer) :
	m_gameOver(false)
	, m_puzzleBuilder(puzzleBuilder)
	, m_buffer(buffer)
{
	m_isInGame = false;
	m_playerStats = Pair<std::string, int>{ "Player", 0 };

	m_mainPage = new MainPage(buffer, this);

	m_currentPage = dynamic_cast<IPage *>(m_mainPage);
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
		if (!m_isInGame)
		{
			m_buffer->ClearBuffer(true);
			m_currentPage->Update();
			if (m_isInGame)
				continue;
			m_currentPage->Draw();
		}
		else
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
		} // else!

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
	m_isInGame = true;
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






