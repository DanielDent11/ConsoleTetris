#pragma once

#include <interfaces/ibuilder.h>
#include <interfaces/idrawable.h>
#include <interfaces/ielement.h>

#include <unordered_map>
#include <memory>

class Buffer;
class Game
{
public:
	Game();
	Game(IBuilder *puzzleBuilder, Buffer *buffer);
	~Game();

	void Run();
	void Spawn();
	void GameOver();

	void AddElementToScene(IElement *element);
	void RemoveElementFromScene(IElement *element);

	void AddPuzzle(IElement *element);
	void RemovePuzzle(IElement *element);

	void FillCells(IElement *element);

	bool IsGameOver() const;
	void SetGameOver(bool value);
private:
	bool m_gameOver;
	IBuilder *m_puzzleBuilder;

	Buffer *m_buffer;
	std::vector<IElement *> m_puzzles;
	std::unordered_map<int, IElement *> m_updateables;
	std::unordered_map<int, IDrawable *> m_drawables;

	std::vector<int> m_elementsToBeRemovedIds;

	void RemoveElements();
	bool IsRowFilled(int row) const;
	void DeleteFilledRows();
};