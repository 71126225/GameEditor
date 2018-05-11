#ifndef _TX_OPENGL_BUFFER_H_
#define _TX_OPENGL_BUFFER_H_

#include "txMP_wrap.h"

struct OPENGL_VERTEX
{
	GLfloat x, y;
#ifdef MAGIC_3D
	GLfloat z;
#endif
	GLfloat u, v;
	GLuint color;
};

// eng: Class of vertexes buffer
class OpenGLBuffer
{
protected:
#ifdef OPENGL_SHADER
	// VBO buffers
	GLuint buffers[2];
#endif

	int k_particles;
	int max_particles;

	OPENGL_VERTEX* m_vertex;

	GLushort* m_index;

public:
	OpenGLBuffer();

	virtual ~OpenGLBuffer();

	void Create(const int& count);

	const int& GetCount(){ return k_particles; }

	OPENGL_VERTEX* GetArray(){ return m_vertex; }

	GLushort* GetIndexArray(){ return m_index; }

	OPENGL_VERTEX* NextParticles()
	{
		OPENGL_VERTEX* vert = &(m_vertex[k_particles * 4]);
		++k_particles;
		return vert;
	}

#ifdef OPENGL_SHADER
	// put all data in GPU
	void CopyBuffersToGPU()
	{
		// target - �������Ҫ�洢���������ͣ�ֻ������ֵ�� GL_ARRAY_BUFFER, �� GL_ELEMENT_ARRAY_BUFFER��
		// ����Ƕ����������ԣ����磺 �������ꡢ�������ꡢ������������ɫ����ȣ�Ҫʹ�� GL_ARRAY_BUFFER��
		// �������飬Ҫʹ�� GL_ELEMENT_ARRAY_BUFFER���Ա� glDrawElements() ʹ�á�
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(OPENGL_VERTEX)* k_particles * 4, m_vertex, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	}
#endif
};

#endif