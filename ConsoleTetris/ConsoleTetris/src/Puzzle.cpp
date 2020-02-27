#include <Puzzle.h>
#include <Input.h>
#include <iostream>

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
	std::cout << m_coolDownForPlayerMove.count() << '\n';
	if (m_coolDownForPlayerMove == std::chrono::duration<float>::zero())
	{
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
	} else {
		m_coolDownForPlayerMove -= game_time::deltaTime;
		if (m_coolDownForPlayerMove.count() <= 0.0f)
		{
			m_coolDownForPlayerMove = std::chrono::duration<float>::zero();
		}
	}

}

void Puzzle::Draw()
{
	for (int y = 0; y < m_shapeSize.y; ++y)
	{
		for (int x = 0; x < m_shapeSize.x; ++x)
		{
			if (m_shape[y * m_shapeSize.x + x] == L'#')
			{
				m_buffer->Set(L'#', m_coord.x + x, m_coord.y + y);
			}
		}
	}
}

IElement::Shape Puzzle::GetShape() const { return m_shape; }

void Puzzle::SetBuffer(Buffer *buffer) { m_buffer = buffer; }

void Puzzle::SetShape(IElement::Shape shape) { m_shape = shape; }

Pair<int, int> Puzzle::GetCoord() const { return m_coord; }

void Puzzle::SetCoord(const Pair<int, int> &coord) { m_coord = coord; }

void Puzzle::Move(const Pair<int, int> &move)
{
	m_coord.x += move.x;
	m_coord.y += move.y;
}

void Puzzle::SetShapeSize(const Pair<int, int> &s)
{
	m_shapeSize = s;
}

Pair<int, int> Puzzle::GetShapeSzie() const
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
