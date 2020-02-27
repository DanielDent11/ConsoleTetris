#include <Game.h>
#include <Buffer.h>
#include <chrono>
#include <iostream>
#include <game_time.h>

Game::Game() {}

Game::Game(IBuilder *puzzleBuilder, const Pair<int, int> &map, Buffer *buffer) :
	m_puzzleBuilder(puzzleBuilder)
	, m_map(map)
	, m_buffer(buffer)
{}

Game::~Game()
{
	for (auto puzzle : m_updateables)
	{
		RemoveElement(puzzle.second);
	}
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

		for (auto elem : m_drawables)
		{
			elem.second->Draw();
		}

		m_buffer->Draw();
	}
}

void Game::GameOver()
{

}

void Game::AddElement(IElement *element)
{
	m_updateables.insert(std::make_pair(element->GetId(), element));
	m_drawables.insert(std::make_pair(element->GetId(), 
		dynamic_cast<IDrawable *>(element)));
}

void Game::RemoveElement(IElement *element)
{
	m_updateables.erase(element->GetId());
	m_drawables.erase(element->GetId());
	m_puzzleBuilder->Destroy(element);
}


