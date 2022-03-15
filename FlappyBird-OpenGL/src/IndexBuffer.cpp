#pragma once
#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
	m_Count = count;
	GLCall(glGenBuffers(1, &m_RendererID)); //Create a buffer and set the ID to the above buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //Bind the above buffer ID to an Array buffer. This is what is going to be drawn in the next draw call.
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); //Actually create and assign the data to the buffer
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID))
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //Bind the above buffer ID to an Array buffer. This is what is going to be drawn in the next draw call.
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0)); //Bind the above buffer ID to an Array buffer. This is what is going to be drawn in the next draw call.

}
