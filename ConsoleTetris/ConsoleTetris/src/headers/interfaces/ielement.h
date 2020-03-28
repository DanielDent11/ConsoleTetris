#pragma once

#include <help_types.h>

class IElement
{
public:
	virtual help_types::Shape GetShape() const = 0;
	virtual void SetShape(help_types::Shape) = 0;
	virtual Pair<int, int> GetCoord() const = 0;
	virtual void SetCoord(const Pair<int, int> &) = 0;
	virtual void Move(const Pair<int, int> &) = 0;
	virtual Pair<int, int> GetShapeSize() const = 0;
	virtual void SetShapeSize(const Pair<int, int> &) = 0;
	virtual void Update() = 0;
	virtual int GetId() const = 0;
	virtual void SetId(int) = 0;
};