#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"

CLASS(Brush, ObjectBase)
{
	private:

		void createSprite(const Vector2 &position);

	public:

		/*
		*	BRUSH MODE:

			- Free: it can create/select objects on every point
			- Grid: only can create/select objects in a grid

			Both use raycast.
		*/
		void init();
		void onPressed(const Vector2& position);
};