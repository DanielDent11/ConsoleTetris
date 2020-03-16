#pragma once

#include <interfaces/IBuilder.h>
#include <interfaces/IDrawable.h>
#include <interfaces/IElement.h>

#include <unordered_map>
#include <memory>

class Buffer;
class Game
{
public:
	Game();
	Game(IBuilder *puzzleBuilder, const Pair<int, int> &mapSize, Buffer *buffer);
	~Game();

	void Run();
	void Spawn();
	void GameOver();

	void AddElementToScene(IElement *element);
	void RemoveElementFromScene(IElement *element);

	void AddPuzzle(IElement *element);
	void RemovePuzzle(IElement *element);

	void FillMapWithElement(IElement *element);

	wchar_t *GetFilledCells() const;
	Pair<int, int> GetMapSize() const;
	void FillCellsByElement(IElement *element);
private:
	IBuilder *m_puzzleBuilder;
	Pair<int, int> m_mapSize;
	Buffer *m_buffer;
	wchar_t *m_filledCells;
	std::vector<IElement *> m_puzzles;
	std::unordered_map<int, IElement *> m_updateables;
	std::unordered_map<int, IDrawable *> m_drawables;

	void DrawFilledCells();
};