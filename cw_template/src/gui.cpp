#include "sceneManager.h"
#include "gui.h"

using namespace ImGui;
SceneManager* mySceneMan;
GLFWwindow* myWindow;
static int keystate;

static void showFrames(bool opened) {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	if (!ImGui::Begin("Overlay", &opened, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::End();
		return;
	}

	ImGui::Text("Zoe Wall Physics-Based Animation - 40182161");
	ImGui::Separator();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	ImGui::End();
}

static void menu() {

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"

	bool opened;
	if (!ImGui::Begin("Menu", &opened, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::Button("Reset Ball"))
	{
		mySceneMan->reset_ball();
	}

	if (ImGui::Button("Add Ball"))
	{
		mySceneMan->add_ball();
	}

	if (ImGui::Button("Clear Balls"))
	{
		mySceneMan->clear_balls();
	}

	

	ImGui::End();
}

void updateGUI() {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_NewFrame();

	showFrames(true);
	menu();
}

void initialiseGUI(GLFWwindow* inwindow, SceneManager &man) {
	mySceneMan = &man;
	myWindow = inwindow;
	bool f = ImGui_ImplGlfwGL3_Init(myWindow, true);
}

void renderGUI() { ImGui::Render(); }

void cleanGUI() { ImGui_ImplGlfwGL3_Shutdown(); }