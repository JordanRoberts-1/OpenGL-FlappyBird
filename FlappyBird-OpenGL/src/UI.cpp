#include "UI.h"
#include "ImGUI/imgui.h"
#include "Score.h"

void UI::RenderScore()
{
	ImGui::Begin("Score");
	ImGui::Text("Score: %d", Score::GetScore());
	ImGui::End();
}
