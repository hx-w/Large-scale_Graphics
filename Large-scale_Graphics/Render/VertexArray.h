#pragma once
#include "GLErrorMessage.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray:public QOpenGLFunctions_3_3_Core
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer( VertexBuffer& vb, const VertexBufferLayout &layout);

	void Bind() ;
	void Unbind();
};