#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(const VertexArray& va, const Shader& shader, unsigned int amountOfVerticies) const;
};

