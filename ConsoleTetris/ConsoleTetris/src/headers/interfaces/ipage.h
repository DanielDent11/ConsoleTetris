#pragma once

#include <interfaces/idrawable.h>
#include <interfaces/iupdateable.h>

class IPage : public IDrawable, IUpdateable
{
public:
	virtual void Draw() = 0;
	virtual void Update() = 0;
};