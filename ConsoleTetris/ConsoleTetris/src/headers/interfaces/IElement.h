#pragma once

#include <Pair.h>

class IElement
{
public:
  /*
  3x3 char array
  Example of Shape:
	 ...
	 ###
	 ...
  */
	typedef const wchar_t *Shape;

	virtual Shape GetShape() const = 0;
	virtual void SetShape(Shape) = 0;
	virtual Pair<int, int> GetCoord() const = 0;
	virtual void SetCoord(const Pair<int, int> &) = 0;
	virtual void Move(const Pair<int, int> &) = 0;
	virtual Pair<int, int> GetShapeSzie() const = 0;
	virtual void SetShapeSize(const Pair<int, int> &) = 0;
	virtual void Update() = 0;
	virtual int GetId() const = 0;
	virtual void SetId(int) = 0;
};