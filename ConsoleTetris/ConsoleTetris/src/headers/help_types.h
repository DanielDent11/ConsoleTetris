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

namespace types {
	/*
	3x3 char array
	Example of Shape:
	  ...
	  ###
	  ...
	*/
	typedef wchar_t * Shape;

	enum Direciton {Top, Right, Bottom, Left};

	struct Bounds
	{
		Pair<int, int> top;
		Pair<int, int> left;
		Pair<int, int> bottom;
		Pair<int, int> right;
	};
}
