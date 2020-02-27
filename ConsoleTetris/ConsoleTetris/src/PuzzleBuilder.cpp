#include <PuzzleBuilder.h>

PuzzleBuilder::PuzzleBuilder(Buffer *buffer, const Pair<int, int> &shapeSize) :
	m_buffer(buffer)
	, m_shapeSize(shapeSize)
{}

Puzzle *PuzzleBuilder::Build(IElement::Shape shape)
{
	Puzzle *p = new Puzzle();
	p->SetBuffer(m_buffer);
	p->SetCoord(Pair<int, int>(0, 0));
	p->SetShape(shape);
	p->SetShapeSize(m_shapeSize);
	p->SetId(m_puzzleCount++);
	++m_puzzleCount;

	return p;
}

void PuzzleBuilder::Destroy(IElement *element)
{
	--m_puzzleCount;
	delete element;
}
