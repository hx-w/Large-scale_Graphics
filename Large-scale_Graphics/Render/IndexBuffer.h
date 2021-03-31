#pragma once
#include "GLErrorMessage.h"

class IndexBuffer:public QOpenGLFunctions_3_3_Core
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	//������������������65535��Ҳ����unsigned short ��Լ�ڴ�
	~IndexBuffer();

	void Bind() ;
	void Unbind() ;
	// const means in this function it won't modify anything 

	inline unsigned int GetCount() const { return m_Count; }
	// TODO: lock and unlocks
protected:

};