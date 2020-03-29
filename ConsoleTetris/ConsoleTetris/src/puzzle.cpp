#include <puzzle.h>
#include <input.h>

#include <iostream>
#include <cassert>
#include <stdlib.h>

namespace game_time
{
	extern std::chrono::duration<float> deltaTime;
	extern std::chrono::high_resolution_clock::time_point lastTick;
	extern std::chrono::high_resolution_clock::time_point start;

	void Init();
	void Update();
}

void Puzzle::Update()
{
	if (m_coolDownForPlayerMove == std::chrono::duration<float>::zero())
	{
		// Movement input
		if (Input::GetKeyDown('D'))
		{
			Move(Pair<int, int>(m_playerMoveSpeedX, 0));
			m_coolDownForPlayerMove = std::chrono::duration<float>(m_maxCoolDownForPlayerMove);
		}
		if (Input::GetKeyDown('A'))
		{
			Move(Pair<int, int>(-m_playerMoveSpeedX, 0));
			m_coolDownForPlayerMove = std::chrono::duration<float>(m_maxCoolDownForPlayerMove);
		}
		if (Input::GetKeyDown('W'))
		{
			Move(Pair<int, int>(0, -m_playerMoveSpeedY));
			m_coolDownForPlayerMove = std::chrono::duration<float>(m_maxCoolDownForPlayerMove);
		}
		if (Input::GetKeyDown('S'))
		{
			Move(Pair<int, int>(0, m_playerMoveSpeedY));
			m_coolDownForPlayerMove = std::chrono::duration<float>(m_maxCoolDownForPlayerMove);
		}
	}
	else
	{
		DecreaseCoolDown(m_coolDownForPlayerMove);
	}

	if (m_coolDownForRotation == std::chrono::duration<float>::zero())
	{
		// Rotation input
		if (Input::GetKeyDown('E'))
		{
			m_shape = RotateClockwise();
			m_coolDownForRotation = std::chrono::duration<float>(m_maxCoolDownForRotation);
		}
		if (Input::GetKeyDown('Q'))
		{
			m_shape = RotateCounterClockwise();
			m_coolDownForRotation = std::chrono::duration<float>(m_maxCoolDownForRotation);
		}
	}
	else
	{
		DecreaseCoolDown(m_coolDownForRotation);
	}

	MoveDownByGravity();
}

void Puzzle::Draw()
{
	for (int y = -m_shapeSize.y / 2; y <= m_shapeSize.y / 2; ++y)
	{
		for (int x = -m_shapeSize.x / 2; x <= m_shapeSize.x / 2; ++x)
		{
			int cy = y + m_shapeSize.y / 2;
			int cx = x + m_shapeSize.x / 2;

			if (m_shape[cy * m_shapeSize.x + cx] == L'#')
			{
				m_buffer->Set(L'#', m_coord.x + x, m_coord.y + y);
			}
		}
	}
}

help_types::Shape Puzzle::GetShape() const
{
	return m_shape;
}

void Puzzle::SetBuffer(Buffer *buffer)
{
	m_buffer = buffer;
}

void Puzzle::SetGame(Game *game)
{
	m_game = game;
}

void Puzzle::SetBoundRect(help_types::Rectangle &boundRect)
{
	m_boundRect = boundRect;
}

help_types::Rectangle Puzzle::GetBoundRect() const
{
	return m_boundRect;
}

Game *Puzzle::GetGame() const
{
	return m_game;
}

void Puzzle::SetShape(help_types::Shape shape)
{
	m_shape = shape;
}

Pair<int, int> Puzzle::GetCoord() const
{
	return m_coord;
}

void Puzzle::SetCoord(const Pair<int, int> &coord)
{
	m_coord = coord;
}

void Puzzle::Move(const Pair<int, int> &move)
{
	auto coord = m_coord;
	coord.x += move.x;
	coord.y += move.y;

	if (SufficeConstraints(m_shape, coord))
	{
		m_coord.x += move.x;
		m_coord.y += move.y;
	}
	else
	{
		if (move.y > 0 && m_isAlive)
		{
			m_isAlive = false;
			m_game->FillCells(this);
			m_game->RemoveElementFromScene(this);
			m_game->Spawn();
		}
	}
}

void Puzzle::SetShapeSize(const Pair<int, int> &s)
{
	m_shapeSize = s;
}

Pair<int, int> Puzzle::GetShapeSize() const
{
	return m_shapeSize;
}

int Puzzle::GetId() const
{
	return m_id;
}

void Puzzle::SetId(int val)
{
	m_id = val;
}

void Puzzle::CalculateBoundRectByShape(help_types::Shape)
{}

help_types::Shape Puzzle::RotateClockwise()
{
	wchar_t *tmp = new wchar_t[m_shapeSize.x * m_shapeSize.y];
	for (int i = 0; i < m_shapeSize.x * m_shapeSize.y; ++i)
	{
		tmp[i] = m_shape[i];
	}

	for (int y = -m_shapeSize.y / 2; y <= m_shapeSize.y / 2; ++y)
	{
		for (int x = -m_shapeSize.x / 2; x <= m_shapeSize.x / 2; ++x)
		{
			int cx = x + m_shapeSize.x / 2;
			int cy = y + m_shapeSize.y / 2;

			int nx = y;
			int ny = -x;

			int cnx = nx + m_shapeSize.x / 2;
			int cny = ny + m_shapeSize.y / 2;

			tmp[cy * m_shapeSize.x + cx] = m_shape[cny * m_shapeSize.x + cnx];
			if (L'#' == tmp[cy * m_shapeSize.x + cx])
			{
				if (!IsFree(Pair<int, int>{m_coord.x + x, m_coord.y + y}))
				{
					return m_shape;
				}
			}
		}
	}

	for (int i = 0; i < m_shapeSize.x * m_shapeSize.y; ++i)
	{
		m_shape[i] = tmp[i];
	}

	delete[] tmp;

	return m_shape;
}

help_types::Shape Puzzle::RotateCounterClockwise()
{
	wchar_t *tmp = new wchar_t[m_shapeSize.x * m_shapeSize.y];
	for (int i = 0; i < m_shapeSize.x * m_shapeSize.y; ++i)
	{
		tmp[i] = m_shape[i];
	}

	for (int y = -m_shapeSize.y / 2; y <= m_shapeSize.y / 2; ++y)
	{
		for (int x = -m_shapeSize.x / 2; x <= m_shapeSize.x / 2; ++x)
		{
			int cx = x + m_shapeSize.x / 2;
			int cy = y + m_shapeSize.y / 2;

			int nx = -y;
			int ny = x;

			int cnx = nx + m_shapeSize.x / 2;
			int cny = ny + m_shapeSize.y / 2;

			tmp[cy * m_shapeSize.x + cx] = m_shape[cny * m_shapeSize.x + cnx];
			if (L'#' == tmp[cy * m_shapeSize.x + cx])
			{
				if (!IsFree(Pair<int, int>{m_coord.x + x, m_coord.y + y}))
				{
					return m_shape;
				}
			}
		}
	}

	for (int i = 0; i < m_shapeSize.x * m_shapeSize.y; ++i)
	{
		m_shape[i] = tmp[i];
	}

	delete[] tmp;

	return m_shape;
}

void Puzzle::MoveDownByGravity()
{
	if (m_coolDownMoveDownByGravity == std::chrono::duration<float>::zero())
	{
		Move(Pair<int, int>{0, m_GRAVITY});
		m_coolDownMoveDownByGravity = std::chrono::duration<float>(m_MAX_COOL_DOWN_FOR_MOVE_BY_GRAVITY);
	}
	else
	{
		DecreaseCoolDown(m_coolDownMoveDownByGravity);
	}
}

void Puzzle::DecreaseCoolDown(std::chrono::duration<float> &coolDown)
{
	coolDown -= game_time::deltaTime;
	if (coolDown.count() <= 0.0f)
	{
		coolDown = std::chrono::duration<float>::zero();
	}
}

bool Puzzle::SufficeConstraints(help_types::Shape shape, Pair<int, int> center) const
{
	for (int y = -m_shapeSize.y / 2; y <= m_shapeSize.y / 2; ++y)
	{
		for (int x = -m_shapeSize.x / 2; x <= m_shapeSize.x / 2; ++x)
		{
			int cx = x + m_shapeSize.x / 2;
			int cy = y + m_shapeSize.y / 2;

			if (shape[cy * m_shapeSize.x + cx] == '#')
			{
				Pair<int, int> coord{ center.x + x, center.y + y };
				if (!InBounds(coord) || !IsFree(coord))
				{
					return false;
				}
			}
		}
	}

	return true;
}

bool Puzzle::InBounds(Pair<int, int> coord) const
{
	auto boundsX = m_buffer->GetBoundsX();
	auto boundsY = m_buffer->GetBoundsY();

	return (coord.x >= boundsX.x && coord.x < boundsX.y
		&& coord.y < boundsY.y);
}

bool Puzzle::IsFree(Pair<int, int> coord) const
{
	if (coord.y < m_buffer->GetBoundsY().x)
	{
		return true;
	}
	wchar_t c = m_buffer->Get(coord.x, coord.y);
	return (c != 'b' && c != '*');
}

