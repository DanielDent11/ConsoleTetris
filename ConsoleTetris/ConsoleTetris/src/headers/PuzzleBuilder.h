#pragma once

#include <Buffer.h>
#include <Puzzle.h>

#include <interfaces/IBuilder.h>

class PuzzleBuilder : public IBuilder
{
public:
	PuzzleBuilder(Buffer *buffer, const Pair<int, int> &shapeSize);

	virtual Puzzle *Build(IElement::Shape shape) override;
	virtual void Destroy(IElement *) override;
private:
	int m_puzzleCount;
	Buffer *m_buffer;
	Pair<int, int> m_shapeSize;
};