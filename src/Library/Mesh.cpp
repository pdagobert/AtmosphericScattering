#include "Mesh.hpp"
#include "RenderTarget.hpp"
#include "Geometry.hpp"
#include "ArrayBufferData.hpp"

Mesh::Mesh( const GeometryData& g, Primitive primitive ) :
	m_vertexBuffer(),
	m_indexBuffer(),
	m_instanceBuffer(),
	m_vertexArray(),
	m_command( { primitive, g.indices.getNumIndices(), 0, 0 } )
{
	m_vertexBuffer.setData( g.vertices.getData(), Usage::Static );
	m_indexBuffer.setData( g.indices.getData(), Usage::Static );

	m_vertexArray.bind();

	m_vertexBuffer.bind();
	g.vertices.bind( 0 );

	m_indexBuffer.bind();

	m_vertexArray.unbind();

	m_vertexBuffer.unbind();
	m_indexBuffer.unbind();
}

Mesh::Mesh( const GeometryData& g, Primitive primitive, const ArrayBufferData& instances ) :
	m_vertexBuffer(),
	m_indexBuffer(),
	m_instanceBuffer( new ArrayBuffer ),
	m_vertexArray(),
	m_command( { primitive, g.indices.getNumIndices(), instances.getNumVertices(), 0 } )
{
	m_vertexBuffer.setData( g.vertices.getData(), Usage::Static );
	m_indexBuffer.setData( g.indices.getData(), Usage::Static );
	m_instanceBuffer->setData( instances.getData(), Usage::Static );

	m_vertexArray.bind();

	m_vertexBuffer.bind();
	int start = g.vertices.bind( 0 );

	m_instanceBuffer->bind();
	instances.bind( start );

	m_indexBuffer.bind();

	m_vertexArray.unbind();

	m_vertexBuffer.unbind();
	m_indexBuffer.unbind();
}

void Mesh::render( RenderTarget& target ) const
{
	m_vertexArray.bind();
	target.draw( m_command );
	m_vertexArray.unbind();
}