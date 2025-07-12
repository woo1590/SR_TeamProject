#pragma once
struct Emitter
{
	_uint burstCount = 0;
	_float ratePerSec;
	_bool looping = false;

	_float timer = 0.f;
	_bool alive = true;

	void Spawn(std::vector<Particle>);
};

