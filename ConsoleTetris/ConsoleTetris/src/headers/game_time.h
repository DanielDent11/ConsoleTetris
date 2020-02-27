#pragma once

#include <chrono>
#include <iostream>

namespace game_time
{
	std::chrono::duration<float> deltaTime;
	std::chrono::high_resolution_clock::time_point lastTick;
	std::chrono::high_resolution_clock::time_point start;

	void Init()
	{
		lastTick = std::chrono::high_resolution_clock::now();
		start = lastTick;
	}

	void Update()
	{
		auto now = std::chrono::high_resolution_clock::now();
		deltaTime = now - lastTick;
		lastTick = now;
	}
}
