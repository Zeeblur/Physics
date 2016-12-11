#include "sceneManager.h"
#include "gui.h"

using namespace ImGui;

GLFWwindow* myWindow;
static bool showMenu;
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

	extern SceneManager* myScene;

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"

	bool opened;
	if (!ImGui::Begin("Menu", &opened, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Text("Optimization");
		if (ImGui::Button("Draw Lines"))
		{
			//myScene->toggleDebug();
		}

		if (ImGui::Button("Fix Culling"))
		{
			// myScene->toggleCull();
		}
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
		if (ImGui::Button("Free Camera"))
		{
			//myScene->cam = myScene->cameraList[2];
		}

		ImGui::SameLine();

		if (ImGui::Button("Point Light Camera"))
		{
		//	myScene->cam = myScene->cameraList[1];
		}

		ImGui::SameLine();

		if (ImGui::Button("Platform Camera"))
		{
//1111			myScene->cam = myScene->cameraList[0];
		}
	}


	if (ImGui::CollapsingHeader("Post-Processing"))
	{
		if (ImGui::Button("Blur"))
		{
			//myScene->toggleBlur();
		}

		if (ImGui::Button("Greyscale"))
		{
			//my/Scene->toggleGrey();
		}

		if (ImGui::Button("Vignette"))
		{
	//		myScene->toggleVignette();
		}

		if (ImGui::Button("Bloom"))
		{
		//	myScene->toggleBloom();
		}

	}

	ImGui::End();
}

void updateGUI() {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_NewFrame();

	showFrames(true);
	menu();
}

void initialiseGUI(GLFWwindow* inwindow) {
	myWindow = inwindow;
	bool f = ImGui_ImplGlfwGL3_Init(myWindow, true);
	showMenu = false;
}

void renderGUI() { ImGui::Render(); }

void cleanGUI() { ImGui_ImplGlfwGL3_Shutdown(); }