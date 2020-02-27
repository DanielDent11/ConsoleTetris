#pragma once

template <typename T1, typename T2>
class Pair
{
public:
	Pair() : x(0), y(0) {}
	Pair(int _x, int _y) : x(_x), y(_y) {}
	Pair(const Pair &other)
	{
		x = other.x;
		y = other.y;
	}

	T1 x;
	T2 y;
};