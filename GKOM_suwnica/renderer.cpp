#include "renderer.h"


void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

	//unbind?
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, unsigned int amountOfVerticies) const
{
	shader.Bind();
	va.Bind();

	glDrawArrays(GL_TRIANGLES, 0, amountOfVerticies);

	//unbind?
}


void Renderer::Clear() const
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}