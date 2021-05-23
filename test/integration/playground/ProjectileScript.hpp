#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"

class Renderer;

CLASS(ProjectileScript, Script) {

	PRI(Renderer, NONE, Renderer*)

	PRI(ExplosionTime, NONE, f32)
	PRI(ExplosionTimeCounter, NONE, f32)
	PRI(IsExploding, NONE, bool)

private:
	static u32 smProjectileIndex;

public:

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void explode();
	bool isExploded() const {
		return mIsExploding;
	}

};