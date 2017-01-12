#include "Terrain.hpp"
#include "Loader.hpp"
#include "Collision/Intersection.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "GenerationFunctions.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

Terrain::Terrain( double diameter ) :
	m_radius( diameter / 2.0 ),
	m_quadTree( m_radius, getQuadTreeDepth(), *this ),
	m_shader( Loader::loadShader( "FlatTerrain.vs", "FlatTerrain.fs" ) )
{
	auto rangeFunction =
		[]( int depth )
		{
			return glm::pow( 2.0, glm::log2( 32.0 ) + depth );
			//return glm::pow( 2.0, glm::log2( 32.0 ) + depth );
		};
	
	m_quadTree.setSelectNodeFunction(
		[ rangeFunction ]( const Node& node, const Camera& camera )
		{
			Sphere sphere( camera.getPosition(), rangeFunction( node.depth ) );
			return Intersection::intersectAABBSphere( node.aabb, sphere );
		} );
	
	m_atlases.push_back( TextureAtlas( Terrain::PatchSize + 1, Terrain::AtlasesNumRegions, InternalFormat::AlphaFloat ) );
	m_atlases.push_back( TextureAtlas( Terrain::PatchSize + 1, Terrain::AtlasesNumRegions, InternalFormat::RGBFloat ) );
	
	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );

	int size = Terrain::PatchSize;
	m_patch = Mesh( Geometry::createSubdivisedQuad( size, size, 1.0f, layout, true ), Primitive::Triangles );
}

void Terrain::update( const Camera& camera )
{
	m_frameVisibleNodes = m_quadTree.updateVisibleNodes( camera );
}

void Terrain::renderPatches( RenderTarget& target, const Camera& camera )
{
	m_shader.bind();
	m_shader.setUniform( "vp", camera.getMatrix() );
	m_shader.setUniform( "far", 10000000000.0f );
	m_shader.setUniform( "heightmap", 0 );
	m_shader.setUniform( "normalmap", 1 );

	for( unsigned int i = 0; i < m_atlases.size(); i++ )
		m_atlases[ i ].bind( i );

	for( const Node& node : m_frameVisibleNodes )
	{
		const TerrainPatch& patch = m_patches[ &node ];

		glm::mat4 world = glm::translate( glm::mat4(), glm::vec3( patch.position - camera.getPosition() ) );
		world = glm::scale( world, glm::vec3( patch.scale ) );

		m_shader.setUniform( "relativeWorld", world );
		m_shader.setUniform( "regionPosition", glm::vec2( patch.region ) / (float)AtlasesNumRegions );
		m_shader.setUniform( "regionSize", 1.0f / (float)AtlasesNumRegions );

		m_patch.render( target );
	}
}

void Terrain::processTaskData( TaskData data, Node& node )
{
	node.isReady = true;
	node.setAABB( data.min, data.max );

	TerrainPatch& patch = m_patches[ &node ];
	patch.position = glm::dvec3( node.position.x, data.average, -node.position.y );
	patch.scale = glm::pow( 2.0, node.depth );

	m_atlases[ 0 ].update( patch.region, data.heightmap, PixelFormat::Alpha, PixelType::Float );
	m_atlases[ 1 ].update( patch.region, data.normalmap, PixelFormat::RGB, PixelType::Float );
}

void Terrain::orderTask( Node& node, int )
{
	requestTask( node );
	processTaskData( m_worker.waitTaskResults( &node ), node );
}

//#include "Simplex.h"
/*#include "FastNoise/FastNoise.h"

double billow( double a )
{
	return a;
}

extern double getPlanetHeight( const glm::dvec3& position );

template< class T >
double getHeight( T& noise, const glm::dvec2& position )
{
	//double height = Simplex::ridgedMF( Simplex::ridgedMF( glm::vec3( position / 3000000.0, 25.0 ), 0.0f, 16, 2.0f, 0.5f ) ) * 500000.0;
	//return height;
	/*double x = position.x;
	double y = position.y;

	double value = billow( noise.GetSimplex( x * 0.00003, y * 0.00003 ) ) * 500000.0;*/

//	return getPlanetHeight( glm::vec3( position, 10.0f ) );
	/*value += billow( noise.GetSimplex( x * 0.00003, y * 0.00003 ) ) * 50000.0;
	value += billow( noise.GetSimplex( x * 0.0003, y * 0.0003 ) ) * 5000.0;
	value += billow( noise.GetSimplex( x * 0.003, y * 0.003 ) ) * 500.0;
	value += billow( noise.GetSimplex( x * 0.03, y * 0.03 ) ) * 50.0;*/
	//double value = ridged( noise.GetSimplex( x * 0.00003, y * 0.00003 ) ) * 500000.0;
	//value += ridged( noise.GetSimplex( x * 0.0001, y * 0.0001 ) ) * 80000.0;
	//value += normal( noise.GetSimplex( x * 0.001, y * 0.001 ) ) * 8000.0;
	/*value += normal( noise.GetSimplex( x * 0.02, y * 0.02 ) ) * 800.0;
	value += normal( noise.GetSimplex( x * 0.2, y * 0.2 ) ) * 80.0;
	value += normal( noise.GetSimplex( x * 2.0, y * 2.0 ) ) * 8.0;
	value += normal( noise.GetSimplex( x * 20.0, y * 20.0 ) ) * 0.8;*/
	//value += ridged( noise.GetSimplex( x * 20.0, y * 20.0 ) ) * 1.0;

	//return value; 
//}

void Terrain::requestTask( const Node& node, int )
{
	static std::function< TaskData( int, const glm::dvec2&, double, int, int ) > func =
		[]( int regionSize, const glm::dvec2& position, double size, int depth, int maxDepth )
		{
			TaskData data;
			//data.min = std::numeric_limits< double >::max();
			//data.max = std::numeric_limits< double >::lowest();
			//data.min = data.max = 0.0;
			data.average = 0.0;
			data.heightmap.resize( regionSize * regionSize );

			std::vector< double > heightmap( regionSize * regionSize );

			for( int y = 0; y < regionSize; y++ )
			{
				for( int x = 0; x < regionSize; x++ )
				{
					glm::dvec2 pos = ( position - size / 2.0 ) + glm::dvec2( x, y ) * size / 32.0;

					/*double height = getHeight( glm::dvec2( x, y ) );
					data.heightmap.push_back( height );

					//data.heightmap.push_back( (double)y / 32.0 );

					if( height > data.max )
						data.max = height;
					
					if( height < data.min )
						data.min = height;
					
					data.average += height;*/
					//data.heightmap.push_back( x % 2 );
					heightmap[ y * regionSize + x ] = generateHeight( glm::dvec3( pos, 0.0 ) );
					//heightmap[ y * regionSize + x ] = 0.0;

					data.average += heightmap[ y * regionSize + x ];
				}
			}

			data.average /= heightmap.size();

			auto minmax = std::minmax_element( heightmap.begin(), heightmap.end() );

			data.min = *minmax.first;
			data.max = *minmax.second;
			

			for( unsigned int i = 0; i < data.heightmap.size(); i++ )
				data.heightmap[ i ] = (float)( heightmap[ i ] - data.average );
			
			data.normalmap.resize( regionSize * regionSize, glm::vec3( 0.0f, 1.0f, 0.0f ) );

			return data;
		};
	
	m_worker.addTask( &node, std::bind( func, m_atlases[ 0 ].getRegionSize(), node.position, node.size, node.depth, getQuadTreeDepth() ) );

	m_patches[ &node ].region = m_atlases[ 0 ].getNextAvailableRegion();
}

bool Terrain::checkNodeStatus( Node& node )
{
	auto data = m_worker.getTaskResults( &node );
	if( !data )
		return false;
	
	processTaskData( *data, node );
	return true;
}

void Terrain::aboutToBeDestroy( const Node& node )
{
	const TerrainPatch& patch = m_patches[ &node ];
	m_atlases[ 0 ].releaseRegion( patch.region );

	m_patches.erase( &node );
	m_worker.removeTask( &node );
}

void Terrain::bindTexture( TerrainTexture type, int unit ) const
{
	m_atlases[ (int)type ].bind( unit );
}

int Terrain::getQuadTreeDepth() const
{
	return glm::log2( m_radius ) - glm::log2( PatchSize / 2.0 );
}