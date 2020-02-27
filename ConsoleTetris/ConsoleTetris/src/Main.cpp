#include <Game.h>
#include <Buffer.h>
#include <PuzzleBuilder.h>
#include <Pair.h>

int main()
{
	Buffer buffer(120, 30);
	PuzzleBuilder puzzleBuilder(&buffer, Pair<int,int>(3, 3));
	Game game(&puzzleBuilder, Pair<int,int>(90, 30), &buffer);

	IElement *puzzle = puzzleBuilder
		.Build(L"...###...");

	game.AddElement(puzzle);
	game.Run();

	return 0;
}