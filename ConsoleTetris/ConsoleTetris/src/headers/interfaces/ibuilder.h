#pragma once

#include <Game.h>

#include <interfaces/IElement.h>

class IBuilder
{
public:
	virtual IElement *Build(
		types::Shape shape
		, const Pair<int, int> &coord) = 0;
	virtual void Destroy(IElement *) = 0;
};