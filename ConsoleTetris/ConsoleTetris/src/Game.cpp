#include <Game.h>
#include <Buffer.h>
#include <PuzzleBuilder.h>
#include <chrono>
#include <iostream>
#include <game_time.h>

#include <algorithm>
#include <random>

Game::Game() {}

Game::Game(IBuilder *puzzleBuilder, const Pair<int, int> &mapSize, Buffer *buffer) :
	m_puzzleBuilder(puzzleBuilder)
	, m_mapSize(mapSize)
	, m_buffer(buffer)
{
	m_filledCells = new wchar_t[mapSize.x * mapSize.y];
}

Game::~Game()
{
	for (auto puzzle : m_updateables)
	{
		RemoveElementFromScene(puzzle.second);
	}

	delete[] m_filledCells;
}

void Game::Run()
{
	game_time::Init();

	while (true)
	{
		// Time update
		game_time::Update();

		// State update
		for (auto elem : m_updateables)
		{
			elem.second->Update();
		}

		// Graphics update
		m_buffer->ClearBuffer();
		DrawFilledCells();

		for (auto elem : m_drawables)
		{
			elem.second->Draw();
		}

		m_buffer->Draw();
	}
}

void Game::Spawn()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);

	int index = gen() % m_puzzles.size();
	auto bounds = m_buffer->GetBounds();
	int width = bounds.y.x - bounds.x.x;
	int height = bounds.y.y - bounds.x.y;

	int xPos = bounds.x.x + (gen() % width - 1);
	int yPos = 0;
	std::cout << xPos << ' ' << yPos << ' ' << index << '\n';;
	std::cout << m_puzzles.size() << "\n\n";

	IElement *puzzle = m_puzzleBuilder
		->Build(
			m_puzzles[index]->GetShape()
			, Pair<int, int>{xPos, yPos}
		);

	AddElementToScene(puzzle);
}

void Game::GameOver()
{

}

void Game::AddElementToScene(IElement *element)
{
	m_updateables.insert(std::make_pair(element->GetId(), element));
	m_drawables.insert(std::make_pair(element->GetId(),
		dynamic_cast<IDrawable *>(element)));
}

void Game::RemoveElementFromScene(IElement *element)
{
	m_updateables.erase(element->GetId());
	m_drawables.erase(element->GetId());
}

void Game::AddPuzzle(IElement *element)
{
	m_puzzles.push_back(element);
}

void Game::RemovePuzzle(IElement *element)
{
	m_puzzles.erase(std::find(m_puzzles.begin(), m_puzzles.end(), element));
}

void Game::FillMapWithElement(IElement *element)
{
	auto coord = element->GetCoord();
	auto boundingRect = element->GetBoundingRect();
	auto shape = element->GetShape();
	auto shapeSize = element->GetShapeSzie();

	for (int y = -shapeSize.y / 2; y <= shapeSize.y; ++y)
	{
		for (int x = -shapeSize.x / 2; x <= shapeSize.x; ++x)
		{
			int cy = y + shapeSize.y / 2;
			int cx = x + shapeSize.x / 2;

			if (shape[cy * shapeSize.x + x] == m_buffer->FILLING_SYMBOL)
			{
				m_filledCells[(coord.y + y) * m_mapSize.x + (coord.x + x)] = m_buffer->FILLING_SYMBOL;
			}
		}
	}
}

wchar_t *Game::GetFilledCells() const
{
	return m_filledCells;
}

Pair<int, int> Game::GetMapSize() const
{
	return m_mapSize;
}

void Game::FillCellsByElement(IElement *element)
{
	auto coord = element->GetCoord();
	auto shapeSize = element->GetShapeSzie();
	auto shape = element->GetShape();
	for (int y = -shapeSize.y / 2; y <= shapeSize.y / 2; ++y)
	{
		for (int x = -shapeSize.x / 2; x <= shapeSize.x / 2; ++x)
		{
			int cy = y + shapeSize.y / 2;
			int cx = x + shapeSize.x / 2;
			if (shape[cy * shapeSize.x + cx] == m_buffer->FILLING_SYMBOL)
			{
				int filled_cx = coord.x + x;
				int filled_cy = coord.y + y;
				m_filledCells[filled_cy * m_mapSize.x + filled_cx] = m_buffer->FILLING_SYMBOL;
			}
		}
	}
}

void Game::DrawFilledCells()
{
	for (int y = 0; y < m_mapSize.y; ++y)
	{
		for (int x = 0; x < m_mapSize.x; ++x)
		{
			if (m_filledCells[y * m_mapSize.x + x] == m_buffer->FILLING_SYMBOL)
			{
				m_buffer->Set(m_buffer->FILLING_SYMBOL, x, y);
			}
		}
	}
}




