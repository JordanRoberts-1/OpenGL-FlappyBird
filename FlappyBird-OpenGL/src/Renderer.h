#pragma once

#include <GL/glew.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
	static void Clear();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};