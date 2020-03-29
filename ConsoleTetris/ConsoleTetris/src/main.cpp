#pragma once 

#include <game.h>
#include <buffer.h>
#include <puzzle_builder.h>
#include <puzzle.h>
#include <help_types.h>
#include <database.h>

#include <iostream>
#include <ios>
#include <limits>

#undef max

int main()
{
	Buffer buffer(120, 30);
	PuzzleBuilder puzzleBuilder(&buffer, Pair<int, int>(3, 3));
	DataBase dataBase;
	Game game(&puzzleBuilder, &buffer, &dataBase);
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

	game.Run();

	return 0;
}