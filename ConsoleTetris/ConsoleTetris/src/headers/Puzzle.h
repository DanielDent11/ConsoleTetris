#pragma once

#include <Buffer.h>
#include <Game.h>
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
		m_coolDownForRotation = std::chrono::duration<float>::zero();
		m_coolDownMoveDownByGravity = std::chrono::duration<float>::zero();
	}
	Puzzle(const Puzzle &other) :
		Puzzle()
	{
		SetCoord(other.GetCoord());
	}
	Puzzle(Puzzle *other) :
		Puzzle()
	{
		SetCoord(other->GetCoord());
	}

	// IDrawable
	void Draw() override;

	// IElement
	void Update() override;
	virtual types::Shape GetShape() const override;
	virtual void SetShape(types::Shape) override;
	virtual Pair<int, int> GetCoord() const override;
	virtual void SetCoord(const Pair<int, int> &) override;
	virtual void Move(const Pair<int, int> &) override;
	virtual void SetShapeSize(const Pair<int, int> &) override;
	virtual Pair<int, int> GetShapeSzie() const override;
	virtual int GetId() const override;
	virtual void SetId(int) override;
	virtual void SetBoundingRect(const types::Rectangle &boundingRect) override;
	virtual types::Rectangle GetBoundingRect() const override;
	
	void SetBuffer(Buffer *);
	void SetGame(Game *);
	void SetBounds(types::Bounds &bounds);
	types::Bounds GetBounds() const;
	Game *GetGame() const;

	types::Shape RotateClockwise();
	types::Shape RotateCounterClockwise();
	types::Rectangle CalculateBoundingRectByShape(types::Shape shape);
private:
	const float m_maxCoolDownForPlayerMove = 0.2f;
	const float m_maxCoolDownForRotation = 0.3f;
	const float m_MAX_COOL_DOWN_FOR_MOVE_BY_GRAVITY = 0.3f;
	const int m_playerMoveSpeedX = 3;
	const int m_playerMoveSpeedY = 1;
	const int m_GRAVITY = 1;

	Game *m_game;
	Buffer *m_buffer;
	types::Rectangle m_boundingRect;
	types::Bounds m_bounds;
	Pair<int, int> m_coord;
	Pair<int, int> m_shapeSize;
	types::Shape m_shape;
	int m_id;
	std::chrono::duration<float> m_coolDownForPlayerMove;
	std::chrono::duration<float> m_coolDownForRotation;
	std::chrono::duration<float> m_coolDownMoveDownByGravity;

	void MoveDownByGravity();
	void DecreaseCoolDown(std::chrono::duration<float> &coolDown);
	void RestrictByBoundingRect();
};
