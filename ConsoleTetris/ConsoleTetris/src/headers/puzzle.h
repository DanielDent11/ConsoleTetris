#pragma once

#include <buffer.h>
#include <game.h>
#include <interfaces/idrawable.h>
#include <interfaces/ielement.h>
#include <chrono>

class Puzzle :
	public IDrawable
	, public IElement
{
public:
	Puzzle()
	{
		m_isAlive = true;
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
	virtual help_types::Shape GetShape() const override;
	virtual void SetShape(help_types::Shape) override;
	virtual Pair<int, int> GetCoord() const override;
	virtual void SetCoord(const Pair<int, int> &) override;
	virtual void Move(const Pair<int, int> &) override;
	virtual void SetShapeSize(const Pair<int, int> &) override;
	virtual Pair<int, int> GetShapeSize() const override;
	virtual int GetId() const override;
	virtual void SetId(int) override;
	
	void SetBuffer(Buffer *);
	void SetGame(Game *);
	void SetBoundRect(help_types::Rectangle &boundRect);
	help_types::Rectangle GetBoundRect() const;
	Game *GetGame() const;

	void CalculateBoundRectByShape(help_types::Shape);
	help_types::Shape RotateClockwise();
	help_types::Shape RotateCounterClockwise();
private:
	const float m_maxCoolDownForPlayerMove = 0.07f;
	const float m_maxCoolDownForRotation = 0.3f;
	const float m_MAX_COOL_DOWN_FOR_MOVE_BY_GRAVITY = 0.3f;
	const int m_playerMoveSpeedX = 1;
	const int m_playerMoveSpeedY = 1;
	const int m_GRAVITY = 1;

	Game *m_game;
	Buffer *m_buffer;
	help_types::Rectangle m_boundRect;
	Pair<int, int> m_coord;
	Pair<int, int> m_shapeSize;
	help_types::Shape m_shape;
	int m_id;
	bool m_isAlive;
	std::chrono::duration<float> m_coolDownForPlayerMove;
	std::chrono::duration<float> m_coolDownForRotation;
	std::chrono::duration<float> m_coolDownMoveDownByGravity;

	void MoveDownByGravity();
	void DecreaseCoolDown(std::chrono::duration<float> &coolDown);
	bool SufficeConstraints(help_types::Shape shape, Pair<int, int> center) const;
	bool InBounds(Pair<int, int> coord) const;
	bool IsFree(Pair<int, int> coord) const;
};
