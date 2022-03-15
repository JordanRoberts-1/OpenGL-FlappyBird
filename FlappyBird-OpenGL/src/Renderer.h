#pragma once

#include <GL/glew.h>
#include <iostream>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak(); //If x is not true, then add a breakpoint
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)) //Macro for wrapping errors in gl function calls

//Loops through all the current errors to empty the error list
void GLClearError();

//Checks for error in a previous function call
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};