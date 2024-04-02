#pragma once

#include <glm/glm.hpp>

class Light
{
	public:
		AE::Color_rgb8 color;
		glm::vec2 position;
		float size;
};

