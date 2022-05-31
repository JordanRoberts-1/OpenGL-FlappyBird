#pragma once

#include <GL/glew.h>
#include <iostream>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

class TexturedQuad;
class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void ClearRendering();
	static void RenderGeometry();
	static void RenderGUI();
private:
	static void Clear();
	static std::array<float, 16> BuildVertices(TexturedQuad object);
};