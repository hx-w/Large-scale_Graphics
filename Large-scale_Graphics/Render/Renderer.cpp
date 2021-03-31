#include "Renderer.h"

void Renderer::Draw(VertexArray &va, IndexBuffer &ib,Shader& shader) 
// 应该加上索引数据，因为指定索引个数
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw( VertexArray &va,  VertexBuffer &vb,  Shader &shader) 
{
	shader.Bind();
	va.Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, vb.GetCount()));
}

void Renderer::Clear() 
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
