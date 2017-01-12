#include "Geometry.hpp"
#include "BufferLayout.hpp"
#include <glm/gtc/constants.hpp>
#include <iostream>

namespace Geometry
{
	GeometryData createUVSphere( float radius, int precision, const BufferLayout& layout )
	{
		ArrayBufferData vertices( layout, precision * precision );

		float inv = 1.0f / ( precision - 1.0f );

		int index = 0;

		for( int i = 0; i < precision; i++ )
		{
			for( int j = 0; j < precision; j++ )
			{
				float x = glm::cos( 2.0f * glm::pi< float >() * j * inv ) * glm::sin( glm::pi< float >() * i * inv );
				float y = glm::sin( -glm::pi< float >() / 2.0f + glm::pi< float >() * i * inv );
				float z = glm::sin( 2.0f * glm::pi< float >() * j * inv ) * glm::sin( glm::pi< float >() * i * inv );

				glm::vec3 normal = glm::vec3( x, y, z );
				vertices.set( index, VertexAttribute::Position, normal * radius );

				if( layout.has( VertexAttribute::TexCoords ) )
					vertices.set( index, VertexAttribute::TexCoords, glm::vec2( j * inv, 1.0 - i * inv ) );

				if( layout.has( VertexAttribute::Normal ) )
					vertices.set( index, VertexAttribute::Normal, normal );

				index++;
			}
		}

		IndexBufferData indices( ( precision - 1 ) * ( precision - 1 ) * 6 );

		index = 0;

		for( int i = 0; i < precision - 1; i++ )
		{
			for( int j = 0; j < precision - 1; j++ )
			{
				indices[ index++ ] = i * precision + j;
				indices[ index++ ] = i * precision + j + 1;
				indices[ index++ ] = ( i + 1 ) * precision + j;
				indices[ index++ ] = ( i + 1 ) * precision + j;
				indices[ index++ ] = i * precision + j + 1;
				indices[ index++ ] = ( i + 1 ) * precision + j + 1;
			}
		}

		return { vertices, indices };
	}

	GeometryData createBox( const glm::vec3& midSize, const BufferLayout& layout )
	{
		ArrayBufferData vertices( layout, 36 );

		vertices.set( 0, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 1, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 2, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );
		vertices.set( 3, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );
		vertices.set( 4, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 5, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );

		vertices.set( 6, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 7, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 8, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
		vertices.set( 9, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
		vertices.set( 10, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 11, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );

		vertices.set( 12, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 13, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 14, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );
		vertices.set( 15, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );
		vertices.set( 16, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 17, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );

		vertices.set( 18, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 19, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 20, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
		vertices.set( 21, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
		vertices.set( 22, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 23, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );

		vertices.set( 24, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, midSize.z ) );
		vertices.set( 25, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
		vertices.set( 26, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
		vertices.set( 27, VertexAttribute::Position, glm::vec3( -midSize.x, midSize.y, -midSize.z ) );
		vertices.set( 28, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, midSize.z ) );
		vertices.set( 29, VertexAttribute::Position, glm::vec3( midSize.x, midSize.y, -midSize.z ) );

		
		vertices.set( 30, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 31, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 32, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 33, VertexAttribute::Position, glm::vec3( midSize.x, -midSize.y, -midSize.z ) );
		vertices.set( 34, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, midSize.z ) );
		vertices.set( 35, VertexAttribute::Position, glm::vec3( -midSize.x, -midSize.y, -midSize.z ) );

		if( layout.has( VertexAttribute::TexCoords ) )
		{
			for( int i = 0; i < 6; i++ )
			{
				vertices.set( i * 6 + 0, VertexAttribute::TexCoords, glm::vec2( 0.0f, 0.0f ) );
				vertices.set( i * 6 + 1, VertexAttribute::TexCoords, glm::vec2( 1.0f, 0.0f ) );
				vertices.set( i * 6 + 2, VertexAttribute::TexCoords, glm::vec2( 0.0f, 1.0f ) );
				vertices.set( i * 6 + 3, VertexAttribute::TexCoords, glm::vec2( 0.0f, 1.0f ) );
				vertices.set( i * 6 + 4, VertexAttribute::TexCoords, glm::vec2( 1.0f, 0.0f ) );
				vertices.set( i * 6 + 5, VertexAttribute::TexCoords, glm::vec2( 1.0f, 1.0f ) );	
			}
		}

		if( layout.has( VertexAttribute::Tangent ) )
		{
			vertices.set( 0, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );	
			vertices.set( 1, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 2, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 3, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 4, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 5, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );

			vertices.set( 6, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
			vertices.set( 7, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
			vertices.set( 8, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
			vertices.set( 9, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
			vertices.set( 10, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );
			vertices.set( 11, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, -1.0f ) );

			vertices.set( 12, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
			vertices.set( 13, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
			vertices.set( 14, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
			vertices.set( 15, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
			vertices.set( 16, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );
			vertices.set( 17, VertexAttribute::Tangent, glm::vec3( -1.0f, 0.0f, 0.0f ) );

			vertices.set( 18, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 19, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 20, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 21, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 22, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 23, VertexAttribute::Tangent, glm::vec3( 0.0f, 0.0f, 1.0f ) );

			vertices.set( 24, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 25, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 26, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 27, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 28, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 29, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );

			vertices.set( 30, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 31, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 32, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 33, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 34, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );
			vertices.set( 35, VertexAttribute::Tangent, glm::vec3( 1.0f, 0.0f, 0.0f ) );

		}

		IndexBufferData indices( 36 );

		for( int i = 0; i < 36; i++ )
			indices[ i ] = i;

		if( layout.has( VertexAttribute::Normal ) )
			computeSmoothNormals( vertices, indices );

		return { vertices, indices };
	}

	GeometryData createQuad( const glm::vec2& size, const BufferLayout& layout, const glm::vec2& offset )
	{
		glm::vec3 off = glm::vec3( offset, 0.0f );

		ArrayBufferData vertices( layout, 4 );

		vertices.set( 0, VertexAttribute::Position, glm::vec3( -size.x, -size.y, 0.0f ) + off );
		vertices.set( 1, VertexAttribute::Position, glm::vec3( size.x, -size.y, 0.0f ) + off );
		vertices.set( 2, VertexAttribute::Position, glm::vec3( -size.x, size.y, 0.0f ) + off );
		vertices.set( 3, VertexAttribute::Position, glm::vec3( size.x, size.y, 0.0f ) + off );

		if( layout.has( VertexAttribute::TexCoords ) )
		{
			vertices.set( 0, VertexAttribute::TexCoords, glm::vec2( 0.0f, 0.0f ) );
			vertices.set( 1, VertexAttribute::TexCoords, glm::vec2( 1.0f, 0.0f ) );
			vertices.set( 2, VertexAttribute::TexCoords, glm::vec2( 0.0f, 1.0f ) );
			vertices.set( 3, VertexAttribute::TexCoords, glm::vec2( 1.0f, 1.0f ) );
		}

		if( layout.has( VertexAttribute::Normal ) )
		{
			vertices.set( 0, VertexAttribute::Normal, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 1, VertexAttribute::Normal, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 2, VertexAttribute::Normal, glm::vec3( 0.0f, 0.0f, 1.0f ) );
			vertices.set( 3, VertexAttribute::Normal, glm::vec3( 0.0f, 0.0f, 1.0f ) );
		}

		IndexBufferData indices( 6 );
		indices[ 0 ] = 0;
		indices[ 1 ] = 1;
		indices[ 2 ] = 2;
		indices[ 3 ] = 2;
		indices[ 4 ] = 1;
		indices[ 5 ] = 3;

		return { vertices, indices };
	}

	GeometryData createUnitQuad( const BufferLayout& layout )
	{
		return createQuad( glm::vec2( 1.0f ), layout );
	}

	GeometryData createSubdivisedQuad( int width, int height, float size, const BufferLayout& layout, bool centered, std::function< float( int, int ) > heightFunc )
	{
		int vWidth = width + 1;
		int vHeight = height + 1;

		ArrayBufferData vertices( layout, vWidth * vHeight );

		glm::vec3 originOffset( -width * size / 2.0f, 0.0f, height * size / 2.0f );

		for( int i = 0; i < vHeight; i++ )
		{
			for( int j = 0; j < vWidth; j++ )
			{
				int vertexIndex = i * vWidth + j;
				glm::vec3 position( j * size, 0.0f, -i * size );

				if( heightFunc )
					position.x = heightFunc( j, i );

				if( centered )
					position += originOffset;

				vertices.set( vertexIndex, VertexAttribute::Position, position );
				
				if( layout.has( VertexAttribute::TexCoords ) )
					vertices.set( vertexIndex, VertexAttribute::TexCoords, glm::vec2( static_cast< float >( j ) / width, static_cast< float >( i ) / height ) );
			}
		}

		IndexBufferData indices( width * height * 6 );
		int index = 0;

		for( int i = 0; i < height; i++ )
		{
			for( int j = 0; j < width; j++ )
			{
				int firstIndex = i * vWidth + j;

				indices[ index++ ] = firstIndex;
				indices[ index++ ] = firstIndex + 1;
				indices[ index++ ] = firstIndex + vWidth;
				indices[ index++ ] = firstIndex + vWidth;
				indices[ index++ ] = firstIndex + 1;
				indices[ index++ ] = firstIndex + vWidth + 1;
			}
		}

		if( layout.has( VertexAttribute::Normal ) )
			computeSmoothNormals( vertices, indices );

		return { vertices, indices };
	}

	GeometryData createTerrainPatch( int width, int height, float size, const BufferLayout& layout )
	{
		int vWidth = width + 1;
		int vHeight = height + 1;

		int numVertices = vWidth * vHeight + vWidth * 2 + vHeight * 2;

		ArrayBufferData vertices( layout, numVertices );

		glm::vec3 originOffset( -width * size / 2.0f, 0.0f, height * size / 2.0f );

		for( int i = 0; i < vHeight; i++ )
		{
			for( int j = 0; j < vWidth; j++ )
			{
				int vertexIndex = i * vWidth + j;
				glm::vec3 position( j * size, 1.0f, -i * size );

				position += originOffset;

				vertices.set( vertexIndex, VertexAttribute::Position, position );
				
				if( layout.has( VertexAttribute::TexCoords ) )
					vertices.set( vertexIndex, VertexAttribute::TexCoords, glm::vec2( static_cast< float >( j ) / width, static_cast< float >( i ) / height ) );
			}
		}

		int numIndices = width * height * 6;
		numIndices += ( width * 2 + height * 2 ) * 6;

		IndexBufferData indices( numIndices );
		int index = 0;

		for( int i = 0; i < height; i++ )
		{
			for( int j = 0; j < width; j++ )
			{
				int firstIndex = i * vWidth + j;

				indices[ index++ ] = firstIndex;
				indices[ index++ ] = firstIndex + 1;
				indices[ index++ ] = firstIndex + vWidth;
				indices[ index++ ] = firstIndex + vWidth;
				indices[ index++ ] = firstIndex + 1;
				indices[ index++ ] = firstIndex + vWidth + 1;
			}
		}

		int vertexIndex = vWidth * vHeight;

		for( int j = 0; j < vWidth; j++ )
		{
			glm::vec3 position( j * size, -1.0f, 0.0f );
			position += originOffset;

			vertices.set( vertexIndex, VertexAttribute::Position, position );

			if( j == width )
			{
				vertexIndex++;
				continue;
			}

			indices[ index++ ] = vertexIndex;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = j;
			indices[ index++ ] = j;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = j + 1;

			vertexIndex++;
		}

		for( int j = 0; j < vWidth; j++ )
		{
			glm::vec3 position( j * size, -1.0f, -height * size );
			position += originOffset;

			vertices.set( vertexIndex, VertexAttribute::Position, position );

			if( j == width )
			{
				vertexIndex++;
				continue;
			}
			
			int startIndex = vWidth * height;

			/*indices[ index++ ] = vertexIndex;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = startIndex + j;
			indices[ index++ ] = startIndex + j;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = startIndex + j + 1;*/

			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = vertexIndex;
			indices[ index++ ] = startIndex + j + 1;
			indices[ index++ ] = startIndex + j + 1;
			indices[ index++ ] = vertexIndex;
			indices[ index++ ] = startIndex + j;

			vertexIndex++;
		}

		for( int j = 0; j < vHeight; j++ )
		{
			glm::vec3 position( 0.0f, -1.0f, -j * size );
			position += originOffset;

			vertices.set( vertexIndex, VertexAttribute::Position, position );

			if( j == height )
			{
				vertexIndex++;
				continue;
			}
			
			int startIndex = vWidth * j;

			/*indices[ index++ ] = vertexIndex;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = startIndex;
			indices[ index++ ] = startIndex;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = startIndex + vWidth;*/

			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = vertexIndex;
			indices[ index++ ] = startIndex + vWidth;
			indices[ index++ ] = startIndex + vWidth;
			indices[ index++ ] = vertexIndex;
			indices[ index++ ] = startIndex;

			vertexIndex++;
		}

		for( int j = 0; j < vHeight; j++ )
		{
			glm::vec3 position( width * size, -1.0f, -j * size );
			position += originOffset;

			vertices.set( vertexIndex, VertexAttribute::Position, position );

			if( j == height )
			{
				vertexIndex++;
				continue;
			}
			
			int startIndex = vWidth * j + width;

			indices[ index++ ] = vertexIndex;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = startIndex;
			indices[ index++ ] = startIndex;
			indices[ index++ ] = vertexIndex + 1;
			indices[ index++ ] = startIndex + vWidth;

			vertexIndex++;
		}

		return { vertices, indices };
	}

	void computeSmoothNormals( ArrayBufferData& vertices, const IndexBufferData& indices )
	{
		for( unsigned int i = 0; i < indices.getNumIndices(); i += 3 )
		{
			glm::vec3 v0 = vertices.getVec3( indices[ i ], VertexAttribute::Position );
			glm::vec3 v1 = vertices.getVec3( indices[ i + 1 ], VertexAttribute::Position );
			glm::vec3 v2 = vertices.getVec3( indices[ i + 2 ], VertexAttribute::Position );

			glm::vec3 e1, e2;
			e1 = glm::normalize( v1 - v0 );
			e2 = glm::normalize( v2 - v0 );

			glm::vec3 normal = glm::cross( e1, e2 );

			for( int j = 0; j < 3; j++ )
				vertices.add( indices[ i + j ], VertexAttribute::Normal, normal );
		}

		for( unsigned int i = 0; i < vertices.getNumVertices(); i++ )
		{
			glm::vec3 normal = glm::normalize( vertices.getVec3( i, VertexAttribute::Normal ) );
			vertices.set( i, VertexAttribute::Normal, normal );
		}
	}
}