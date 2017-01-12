#ifndef __MESH_HPP__
#define __MESH_HPP__

#include "OpenGL/Buffer.hpp"
#include "OpenGL/VertexArray.hpp"
#include "RenderTarget.hpp"

class GeometryData;
class ArrayBufferData;

class Mesh
{
public:
	Mesh() = default;
	Mesh( const GeometryData& g, Primitive primitive );
	Mesh( const GeometryData& g, Primitive primitive, const ArrayBufferData& instances );

	Mesh( Mesh&& ) = default;
	Mesh& operator=( Mesh&& ) = default;

	void render( RenderTarget& target ) const;

private:
	ArrayBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	std::unique_ptr< ArrayBuffer > m_instanceBuffer;

	VertexArray m_vertexArray;
	RenderCommand m_command;
};
#endif