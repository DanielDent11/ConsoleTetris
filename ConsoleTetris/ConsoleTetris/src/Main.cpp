#include <Game.h>
#include <Buffer.h>
#include <PuzzleBuilder.h>
#include <Puzzle.h>
#include <Pair.h>

int main()
{
	Buffer buffer(120, 30);
	PuzzleBuilder puzzleBuilder(&buffer, Pair<int,int>(3, 3));
	Game game(&puzzleBuilder, Pair<int,int>(90, 30), &buffer);
	puzzleBuilder.SetGame(&game);

	types::Shape shape = new wchar_t[3 * 3];
	shape[0] = L'.';
	shape[1] = L'.';
	shape[2] = L'.';
	shape[3] = buffer.FILLING_SYMBOL;
	shape[4] = buffer.FILLING_SYMBOL;
	shape[5] = L'.';
	shape[6] = L'.';
	shape[7] = buffer.FILLING_SYMBOL;
	shape[8] = L'.';
	IElement *puzzle = puzzleBuilder
		.Build(
			shape
			, Pair<int, int>{50, 0}
		);

	game.AddPuzzle(puzzle);

	game.Spawn();
	game.Run();

	return 0;
}