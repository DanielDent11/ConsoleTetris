#include <puzzle_builder.h>

PuzzleBuilder::PuzzleBuilder(Buffer *buffer, const Pair<int, int> &shapeSize) :
	m_buffer(buffer)
	, m_shapeSize(shapeSize)
{}

Puzzle *PuzzleBuilder::Build(
	help_types::Shape shape
	, const Pair<int, int> &coord)
{
	Puzzle *p = new Puzzle();
	p->SetBuffer(m_buffer);
	p->SetCoord(coord);
	p->SetShape(shape);
	p->SetShapeSize(m_shapeSize);
	p->SetId(m_puzzleCount++);
	p->SetGame(m_game);

	return p;
}

void PuzzleBuilder::Destroy(IElement *element)
{
	--m_puzzleCount;
	delete element;
}

void PuzzleBuilder::SetGame(Game *game)
{
	m_game = game;
}

Game *PuzzleBuilder::GetGame() const
{
	return m_game;
}



