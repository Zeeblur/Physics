#pragma once


#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include <GLFW\glfw3.h>

void initialiseGUI(GLFWwindow* inwindow);
void updateGUI();
void renderGUI();
void cleanGUI();