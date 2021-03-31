#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"
class Renderer:public QOpenGLFunctions_3_3_Core
{
private:

public:
	void Draw(VertexArray &va,IndexBuffer &ib, Shader& shader);
	void Draw(VertexArray &va,  VertexBuffer &vb,  Shader &shader);

	void Clear();
};