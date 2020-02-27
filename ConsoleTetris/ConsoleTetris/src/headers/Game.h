#pragma once

#include <interfaces/IBuilder.h>
#include <interfaces/IDrawable.h>

#include <unordered_map>

class Buffer;
class Game
{
public:
	Game();
	Game(IBuilder *puzzleBuilder, const Pair<int, int> &map, Buffer *buffer);
	~Game();

	void Run();
	void GameOver();

	void AddElement(IElement *element);
	void RemoveElement(IElement *element);
private:
	IBuilder *m_puzzleBuilder;
	Pair<int, int> m_map;
	Buffer *m_buffer;
	std::unordered_map<int, IElement *> m_updateables;
	std::unordered_map<int, IDrawable *> m_drawables;
};