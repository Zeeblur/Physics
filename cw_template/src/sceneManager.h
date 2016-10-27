#pragma once
#include "game.h"
#include <graphics_framework.h>

class graphics_framework::camera;

class Entity;

namespace sm
{
	// create own scene manager namespace to control all render/camera functions

	void Init();
	void Update(double delta_time);
	//camera* GetCam();

	//Entity CreateParticles(const vec3 &pos);

}