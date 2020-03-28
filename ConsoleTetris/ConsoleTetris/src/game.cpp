#pragma once

#include <game.h>
#include <buffer.h>
#include <puzzle_builder.h>
#include <chrono>
#include <iostream>
#include <game_time.h>

#include <algorithm>
#include <random>

Game::Game() 
{
}

Game::Game(IBuilder *puzzleBuilder, Buffer *buffer) :
	m_gameOver(false)
	, m_puzzleBuilder(puzzleBuilder)
	, m_buffer(buffer)
{
}

Game::~Game()
{
	for (auto puzzle : m_updateables)
	{
		RemoveElementFromScene(puzzle.second);
	}
}

void Game::Run()
{
	game_time::Init();

	m_buffer->DrawBorder();

	while (true)
	{
		if (m_gameOver)
		{
			GameOver();
		}

		m_elementsToBeRemovedIds.clear();

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
		m_buffer->Draw();
	}
}

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
	int startX = m_buffer->GetBoundsX().x + 10;
	int startY = m_buffer->GetBoundsY().x + 4;

	const wchar_t *gameOverMessage = L"GAME OVER";
	for (int i = 0; i < 9; ++i)
	{
		m_buffer->Set(gameOverMessage[i], startX + i, startY);
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






