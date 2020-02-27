#pragma once

#include <interfaces/IElement.h>

class IBuilder
{
public:
	virtual IElement *Build(IElement::Shape) = 0;
	virtual void Destroy(IElement *) = 0;
};