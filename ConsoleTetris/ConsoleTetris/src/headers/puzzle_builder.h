#pragma once

#include <buffer.h>
#include <puzzle.h>
#include <game.h>

#include <interfaces/IBuilder.h>

class PuzzleBuilder : public IBuilder
{
public:
	PuzzleBuilder(Buffer *buffer, const Pair<int, int> &shapeSize);

	virtual Puzzle *Build(
		help_types::Shape shape
		, const Pair<int, int> &coord) override;
	virtual void Destroy(IElement *) override;

	void SetGame(Game *);
	Game *GetGame() const;
private:
	int m_puzzleCount;
	Game *m_game;
	Buffer *m_buffer;
	Pair<int, int> m_shapeSize;
};