#pragma once

#include <GL/glew.h>
#include <iostream>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void ClearRendering();
	static void RenderGeometry(VertexArray& vao, IndexBuffer& ib, Shader& shader);
	static void RenderGUI();
private:
	static void Clear();
};