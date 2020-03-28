#pragma once 

#include <game.h>
#include <buffer.h>
#include <puzzle_builder.h>
#include <puzzle.h>
#include <help_types.h>

int main()
{
	Buffer buffer(120, 30);
	PuzzleBuilder puzzleBuilder(&buffer, Pair<int, int>(3, 3));
	Game game(&puzzleBuilder, &buffer);
	puzzleBuilder.SetGame(&game);

	std::vector<help_types::Shape> shapes;

	shapes.push_back(help_types::WStr2Shape(
	   L"..."
		"##."
		".#.", 9));
	shapes.push_back(help_types::WStr2Shape(
	   L"..."
		"###"
		"...", 9));
	shapes.push_back(help_types::WStr2Shape(
	   L".#."
		"###"
		"...", 9));

	for (auto shape : shapes)
	{
		IElement *puzzle = puzzleBuilder
			.Build(
				shape
				, Pair<int, int>{50, 0}
		);
		game.AddPuzzle(puzzle);
	}

	game.Spawn();
	game.Run();

	return 0;
}