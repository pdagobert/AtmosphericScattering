#ifndef __BUFFER_LAYOUT_HPP__
#define __BUFFER_LAYOUT_HPP__

#include <map>
#include <functional>

enum VertexAttribute
{
	Position = 0x1,
	TexCoords = 0x2,
	Normal = 0x4,
	Tangent = 0x8
};

class BufferLayout
{
public:
	BufferLayout();

	void add( int id, int size, int divisor = 0 );

	void forEachAttributes( std::function< void( int, int, int ) > func ) const;

	bool has( int id ) const;

	int getVertexSize() const;

	int getAttributeSize( int id ) const;
	int getAttributeOffset( int id ) const;

private:
	struct Attribute
	{
		int size;
		int offset;
		int divisor;
	};

	std::map< int, Attribute > m_attributes;
	int m_flag;
	int m_size;
};
#endif