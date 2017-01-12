#ifndef __INDEX_BUFFER_DATA_HPP__
#define __INDEX_BUFFER_DATA_HPP__

#include <vector>

class IndexBufferData
{
public:
	IndexBufferData( int numIndices );

	unsigned int& operator[]( int index );
	const unsigned int& operator[]( int index ) const;

	unsigned int getNumIndices() const;
	const std::vector< unsigned int >& getData() const;

private:
	std::vector< unsigned int > m_indices;
};
#endif