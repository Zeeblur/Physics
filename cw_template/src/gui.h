#pragma once


#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include <GLFW\glfw3.h>

class SceneManager;

void initialiseGUI(GLFWwindow* inwindow, SceneManager &man);
void updateGUI();
void renderGUI();
void cleanGUI();