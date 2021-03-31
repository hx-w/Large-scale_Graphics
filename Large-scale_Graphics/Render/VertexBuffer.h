#pragma once
#include "GLErrorMessage.h"

class VertexBuffer:public QOpenGLFunctions_3_3_Core
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	VertexBuffer(const void *data, unsigned int size,unsigned int count);
	// size means struct in bytes,count means element numbers
	~VertexBuffer();

	void Bind();
	void Unbind();

	inline unsigned int GetCount() const { return m_Count; }
	// TODO: lock and unlocks
protected:

};