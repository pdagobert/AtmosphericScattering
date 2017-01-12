#ifndef __VERTEX_ARRAY_HPP__
#define __VERTEX_ARRAY_HPP__

#include <GL/glew.h>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	VertexArray( VertexArray&& vertexArray );
	VertexArray& operator=( VertexArray&& vertexArray );

	void bind() const;
	void unbind() const;

private:
	GLuint m_handle;
};
#endif