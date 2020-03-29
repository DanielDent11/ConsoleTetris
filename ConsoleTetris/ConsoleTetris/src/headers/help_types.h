#pragma once

template <typename T1, typename T2>
class Pair
{
public:
	Pair() {}
	Pair(T1 _x, T2 _y) : x(_x), y(_y) {}
	Pair(const Pair &other)
	{
		x = other.x;
		y = other.y;
	}

	T1 x;
	T2 y;
};

namespace help_types
{
	/*
	3x3 char array
	Example of Shape:
	  ...
	  ###
	  ...
	*/
	typedef wchar_t *Shape;

	enum Direciton { Top, Left, Bottom, Right };
	enum ValidateResult { Correct = 0, Error, NotCorrect, NotExists, Exists };

	struct Rectangle
	{
		Pair<int, int> top;
		Pair<int, int> left;
		Pair<int, int> bottom;
		Pair<int, int> right;
	};

	Shape WStr2Shape(const wchar_t *str, int size);
}
