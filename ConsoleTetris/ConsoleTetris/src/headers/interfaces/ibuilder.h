#pragma once

#include <game.h>

#include <interfaces/ielement.h>

class IBuilder
{
public:
	virtual IElement *Build(
		help_types::Shape shape
		, const Pair<int, int> &coord) = 0;
	virtual void Destroy(IElement *) = 0;
};