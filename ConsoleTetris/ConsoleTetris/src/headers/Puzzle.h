#pragma once

#include <Buffer.h>
#include <interfaces/IDrawable.h>
#include <interfaces/IElement.h>
#include <chrono>

class Puzzle :
	public IDrawable
	, public IElement
{
public:
	Puzzle()
	{
		m_coolDownForPlayerMove = std::chrono::duration<float>::zero();
	}
	
	// IDrawable
	void Draw() override;

	// IElement
	void Update() override;
	virtual Shape GetShape() const override;
	virtual void SetShape(Shape) override;
	virtual Pair<int, int> GetCoord() const override;
	virtual void SetCoord(const Pair<int, int> &) override;
	virtual void Move(const Pair<int, int> &) override;
	virtual void SetShapeSize(const Pair<int, int> &) override;
	virtual Pair<int, int> GetShapeSzie() const override;
	virtual int GetId() const override;
	virtual void SetId(int) override;

	void SetBuffer(Buffer *);

private:
	const float m_maxCoolDownForPlayerMove = 0.2f;
	const int m_playerMoveSpeedX = 2;
	const int m_playerMoveSpeedY = 1;

	Buffer *m_buffer;
	Pair<int, int> m_coord;
	Pair<int, int> m_shapeSize;
	Shape m_shape;
	int m_id;
	std::chrono::duration<float> m_coolDownForPlayerMove;
};