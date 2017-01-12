#include "Planet.hpp"
#include "Camera.hpp"
#include "Loader.hpp"
#include "Geometry.hpp"
#include "GenerationFunctions.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Planet::FacesNormals[ 6 ] =
{
	glm::vec3( 1.0f, 0.0f, 0.0f ),
	glm::vec3( -1.0f, 0.0f, 0.0f ),
	glm::vec3( 0.0f, 1.0f, 0.0f ),
	glm::vec3( 0.0f, -1.0f, 0.0f ),
	glm::vec3( 0.0f, 0.0f, 1.0f ),
	glm::vec3( 0.0f, 0.0f, -1.0f )
};

glm::vec3 Planet::FacesTangents[ 6 ] =
{
	glm::vec3( 0.0f, 0.0f, -1.0f ),
	glm::vec3( 0.0f, 0.0f, 1.0f ),
	glm::vec3( 1.0f, 0.0f, 0.0f ),
	glm::vec3( -1.0f, 0.0f, 0.0f ),
	glm::vec3( 1.0f, 0.0f, 0.0f ),
	glm::vec3( -1.0f, 0.0f, 0.0f )
};

glm::dvec3 Planet::Points[] =
{
	glm::dvec3( -1.0, 0.0, 1.0 ),
	glm::dvec3( 1.0, 0.0, 1.0 ),
	glm::dvec3( -1.0, 0.0, -1.0 ),
	glm::dvec3( 1.0, 0.0, -1.0 ),
	glm::dvec3( 0.0, 0.0, 0.0 ),
	glm::dvec3( -1.0, 0.0, 0.0 ),
	glm::dvec3( 1.0, 0.0, 0.0 )
};

enum class PointType
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	Center,
	Left,
	Right
};

Planet::Planet( double diameter ) :
	m_radius( diameter / 2.0 ),
	m_shader( Loader::loadShader( "Planet.vs", "Planet.fs" ) )
{
	auto rangeFunction =
		[]( int depth )
		{
			return glm::pow( 2.0, glm::log2( 32.0 ) + depth );
			//return glm::pow( 2.0, glm::log2( 32.0 ) + depth );
		};
	
	double radius = m_radius;
	
	for( int i = 0; i < 6; i++ )
	{
		m_quadTrees.push_back( QuadTree( m_radius, getQuadTreeDepth(), *this, i ) );

		glm::vec3 normal = Planet::FacesNormals[ i ];
		glm::vec3 tangent = Planet::FacesTangents[ i ];

		m_quadTrees[ i ].setSelectNodeFunction(
			[ radius, normal, tangent, rangeFunction ]( const Node& node, const Camera& camera )
			{
				glm::vec3 bitangent = glm::normalize( glm::cross( normal, tangent ) );

				glm::dvec3 worldPosition = glm::dvec3( normal ) * radius +
					glm::dvec3( tangent ) * node.position.x + glm::dvec3( bitangent ) * -node.position.y;
				
				worldPosition = glm::normalize( worldPosition ) * ( radius + node.average );

				return ( glm::distance( worldPosition, camera.getPosition() ) < rangeFunction( node.depth ) );
			} );
	}

	m_atlases.push_back( TextureAtlas( Planet::PatchSize + 1, Planet::AtlasesNumRegions, InternalFormat::AlphaFloat ) );
	m_atlases.push_back( TextureAtlas( Planet::PatchSize + 1, Planet::AtlasesNumRegions, InternalFormat::RGBFloat ) );
	m_atlases.push_back( TextureAtlas( Planet::PatchSize + 1, Planet::AtlasesNumRegions, InternalFormat::RGBFloat ) );

	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );

	int size = Planet::PatchSize;
	m_patch = Mesh( Geometry::createTerrainPatch( size, size, 1.0f, layout ), Primitive::Triangles );

	Texture2DParameters params;
	params.min = TextureFilter::LinearMipMapLinear;
	params.mag = TextureFilter::Linear;
	params.s = params.t = TextureWrap::Clamp;
	params.mipmaps = true;
	params.anisotropy = 16.0f;

	m_diffuseTextures.push_back( Loader::loadTexture2D( "Textures/grass.jpg", params ) );
}

void Planet::update( const Camera& camera )
{
	for( int i = 0; i < 6; i++ )
		m_frameVisibleNodes[ i ] = m_quadTrees[ i ].updateVisibleNodes( camera );
}

void Planet::renderPatches( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const glm::vec3& betaR, float betaM )
{
	m_shader.bind();
	m_shader.setUniform( "vp", camera.getMatrix() );
	m_shader.setUniform( "far", 1e+14f );
	m_shader.setUniform( "lightDirU", lightDir );
	m_shader.setUniform( "toneMapping", toneMapping );
	m_shader.setUniform( "gamma", gamma );
	m_shader.setUniform( "rayleighExtinctionAtSeaLevel", betaR );
	m_shader.setUniform( "mieExtinctionAtSeaLevel", glm::vec3( betaM ) );

	m_shader.setUniform( "radius", (float)m_radius );
	m_shader.setUniform( "cameraPos", camera.getPosition() );
	
	// 0.0 should be the planet center
	glm::mat4 world = glm::translate( glm::mat4(), glm::vec3( glm::dvec3( 0.0 ) - camera.getPosition() ) );
	m_shader.setUniform( "relativeWorld", world );

	m_shader.setUniform( "heightmap", 0 );
	m_shader.setUniform( "normalmap", 1 );
	m_shader.setUniform( "tangentmap", 2 );

	//m_shader.setUniform( "snowrock", 3 );

	int index = 3;

	for( const Texture2D& tex : m_diffuseTextures )
		tex.bind( index++ );

	for( unsigned int i = 0; i < m_atlases.size(); i++ )
		m_atlases[ i ].bind( i );

	for( int face = 0; face < 6; face++ )
	{
		for( const Node& node : m_frameVisibleNodes[ face ] )
		{
			const PlanetPatch& patch = m_patches[ &node ];

			m_shader.setUniform( "regionPosition", glm::vec2( patch.region ) / (float)AtlasesNumRegions );
			m_shader.setUniform( "regionSize", 1.0f / (float)AtlasesNumRegions );

			m_shader.setUniform( "average", (float)patch.position.y );

			m_shader.setUniform( "patchPosition", glm::vec2( node.position.x, -node.position.y ) );
			m_shader.setUniform( "patchSize", (float)( node.size ) );
			m_shader.setUniform( "faceNormal", Planet::FacesNormals[ face ] );
			m_shader.setUniform( "faceTangent", Planet::FacesTangents[ face ] );

			// if the curvature is more than 10 meters, we can draw the quad without curvature since it wont be visible
			// else we can curve it with the radius, because it will be so far away that precision errors won't matter
			if( m_radius - glm::length( getInterpolatedPatchCorner( patch, face ) ) < 1000.0 )
			{
				m_shader.setUniform( "interpolateCorners", true );
				m_shader.setUniform( "patchNormal", getPatchNormal( Planet::Points[ (int)PointType::Center ], patch, face ) );

				glm::dvec3 tangent = glm::normalize( getPatchCorner( Planet::Points[ (int)PointType::Right ], patch, face, false ) -
					getPatchCorner( Planet::Points[ (int)PointType::Left ], patch, face, false ) );

				m_shader.setUniform( "patchTangent", glm::vec3( tangent ) );

				std::vector< glm::vec3 > corners( 4 );

				for( int j = 0; j < 4; j++ )
				{
					glm::dvec3 corner = getPatchCorner( Planet::Points[ j ], patch, face, true ) - camera.getPosition();
					corners[ j ] = glm::vec3( corner );
				}

				m_shader.setUniform( "corners", corners );
			}
			else
			{
				m_shader.setUniform( "interpolateCorners", false );

				//std::vector< glm::vec3 > normals( 4 );

				/*for( int j = 0; j < 4; j++ )
				{
					normals[ j ] = glm::vec3( getPatchNormal( Planet::Points[ j ], patch, face ) );

					if( face == 0 )
						std::cout << node.depth << " " << normals[ j ].x << " " << normals[ j ].y << " " << normals[ j ].z << std::endl;
				}*/
				
				//m_shader.setUniform( "normals", normals );

				
			}

			m_patch.render( target );
		}
	}
}

glm::dvec3 Planet::getPatchNormal( const glm::dvec3& point, const PlanetPatch& patch, int face ) const
{
	glm::dvec3 normal = Planet::FacesNormals[ face ];
	glm::dvec3 tangent = Planet::FacesTangents[ face ];

	glm::dvec3 cubePos = patch.position + point * (double)( Planet::PatchSize / 2 ) * patch.scale;

	glm::dvec3 pos = normal * ( m_radius );
	pos += tangent * cubePos.x;
	pos += glm::normalize( glm::cross( normal, tangent ) ) * cubePos.z;

	return glm::normalize( pos );
}

glm::dvec3 Planet::getPatchCorner( const glm::dvec3& point, const PlanetPatch& patch, int face, bool addAverage ) const
{
	double height = m_radius;
	if( addAverage )
		height += patch.position.y;
	
	return getPatchNormal( point, patch, face ) * height;
}

glm::dvec3 Planet::getInterpolatedPatchCorner( const PlanetPatch& patch, int face ) const
{
	return glm::mix(
		glm::mix( getPatchCorner( Planet::Points[ (int)PointType::TopLeft ], patch, face, false ), getPatchCorner( Planet::Points[ (int)PointType::TopRight ], patch, face, false ), 0.5 ),
		glm::mix( getPatchCorner( Planet::Points[ (int)PointType::BottomLeft ], patch, face, false ), getPatchCorner( Planet::Points[ (int)PointType::BottomRight ], patch, face, false ), 0.5 ),
		0.5 );
}

void Planet::processTaskData( PlanetTaskData data, Node& node )
{
	node.isReady = true;
	node.setAABB( data.min, data.max );
	node.average = data.average;

	PlanetPatch& patch = m_patches[ &node ];
	patch.position = glm::dvec3( node.position.x, data.average, -node.position.y );
	patch.scale = glm::pow( 2.0, node.depth );

	m_atlases[ 0 ].update( patch.region, data.heightmap, PixelFormat::Alpha, PixelType::Float );
	m_atlases[ 1 ].update( patch.region, data.normalmap, PixelFormat::RGB, PixelType::Float );
	m_atlases[ 2 ].update( patch.region, data.tangentmap, PixelFormat::RGB, PixelType::Float );
}

void Planet::orderTask( Node& node, int id )
{
	requestTask( node, id );
	processTaskData( m_worker.waitTaskResults( &node ), node );
}

#include "FastNoise/FastNoise.h"

double ridged( double value )
{
	value = glm::clamp( value, 0.0, 1.0 );// * 0.5 + 0.5;
	//return 1.0 - glm::abs( value );
	return value;
}

template< class Noise >
double getPlanetHeight2( Noise& noise, const glm::dvec3& position )
{
	double x = position.x;
	double y = position.y;
	double z = position.z;

	double frequency = 0.00003;
	double value = ridged( noise.GetSimplex( x * frequency, y * frequency, z * frequency ) ) * 250000.0;

	frequency = 0.0003;
	value += ridged( noise.GetSimplex( x * frequency, y * frequency, z * frequency ) ) * 50000.0;

	frequency = 0.003;
	value += ridged( noise.GetSimplex( x * frequency, y * frequency, z * frequency ) ) * 5000.0;

	frequency = 0.03;
	value += ( noise.GetSimplex( x * frequency, y * frequency, z * frequency ) ) * 500.0;

	frequency = 0.3;
	value += ( noise.GetSimplex( x * frequency, y * frequency, z * frequency ) ) * 0.5;

	frequency = 3.0;
	value += ( noise.GetSimplex( x * frequency, y * frequency, z * frequency ) ) * 0.1;

	return value;

	/*double frequency2 = 0.0000003;
	double value2 = noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 1.0;

	frequency2 = 0.000003;
	value2 += noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 0.1;

	frequency2 = 0.00003;
	value2 += noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 0.01;

	frequency2 = 0.0003;
	value2 += noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 0.001;

	frequency2 = 0.003;
	value2 += noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 0.0001;

	frequency2 = 0.03;
	value2 += noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 0.00001;

	frequency2 = 0.3;
	value2 += noise.GetSimplex( x * frequency2 + 450125.0, y * frequency2 + 450125.0, z * frequency2 + 450125.0 ) * 0.000001;
	
	return value;//value2 * 500000.0;// + 50000.0 + 5000.0 + 500.0 + 50.0 + 5.0 );*/

	frequency = 0.00004;
	float n = noise.GetSimplex( x * frequency, y * frequency, z * frequency );
	n = n * 0.5 + 0.5;

	frequency = 0.00025;
	float n2 = noise.GetSimplex( x * frequency, y * frequency, z * frequency );
	n2 = n2 * 0.5 + 0.5;

	return ( n * 3.25 + n2 * 0.75 ) * 100000.0;
	//return n * 500000.0;

	//return n * 3.25 + n2 * 0.75;
}

glm::dvec3 cubeToSamplePosition( const glm::dvec2& position, double size, const glm::dvec3& normal, const glm::dvec3& tangent, double radius, int x, int y )
{
	glm::dvec2 cubePos = ( position - size / 2.0 ) + glm::dvec2( x, y ) / 32.0 * size;

	glm::dvec3 pos = normal * radius;
	pos += tangent * cubePos.x;
	pos += glm::normalize( glm::cross( normal, tangent ) ) * cubePos.y;

	return glm::normalize( pos ) * radius;
}

void Planet::requestTask( const Node& node, int id )
{
	double radius = m_radius;

	static std::function< PlanetTaskData( int, const glm::dvec2&, double, int, int, const glm::dvec3&, const glm::dvec3& ) > func =
		[ radius ]( int regionSize, const glm::dvec2& position, double size, int depth, int maxDepth, const glm::dvec3& normal, const glm::dvec3& tangent )
		{
			PlanetTaskData data;
			data.average = 0.0;

			FastNoise noise;

			std::vector< double > heightmap( regionSize * regionSize );

			for( int y = 0; y < regionSize; y++ )
			{
				for( int x = 0; x < regionSize; x++ )
				{
					//glm::dvec3 pos = cubeToSamplePosition( position, size, normal, tangent, radius, x, y );
					//heightmap[ y * regionSize + x ] = generateHeight( pos / 3000000.0 ) * 500000.0;//getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x, y ) );
					heightmap[ y * regionSize + x ] = getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x, y ) );

					data.average += heightmap[ y * regionSize + x ];
				}
			}

			data.average /= heightmap.size();

			auto minmax = std::minmax_element( heightmap.begin(), heightmap.end() );

			data.min = *minmax.first;
			data.max = *minmax.second;
			
			data.heightmap.resize( regionSize * regionSize, 0.0f );

			for( unsigned int i = 0; i < data.heightmap.size(); i++ )
				data.heightmap[ i ] = (float)( heightmap[ i ] - data.average );
			
			data.normalmap.resize( regionSize * regionSize );

			for( int y = 0; y < regionSize; y++ )
			{
				for( int x = 0; x < regionSize; x++ )
				{
					glm::dvec2 cubePos = ( position - size / 2.0 ) + glm::dvec2( x, y ) * size / 32.0;

					glm::dvec3 pos = normal * radius;
					pos += tangent * cubePos.x;
					pos += glm::normalize( glm::cross( normal, tangent ) ) * cubePos.y;

					pos = glm::normalize( pos ) * radius;

					double maxHeight = 250000.0 + 50000.0 + 5000.0 + 500.0 + 0.5 + 0.1;

					double top = getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x, y + 1 ) ) / ( maxHeight );
					double left = getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x - 1, y ) ) / ( maxHeight );
					double right = getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x + 1, y ) ) / ( maxHeight );
					double bottom = getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x, y - 1 ) ) / ( maxHeight );
					
					// UNDERSTAND THIS!!!!!!!
					double sx = left - right;
					double sy = 1.0 / ( 0.0025 * (double)( 1 << ( maxDepth - depth ) ) );
					double sz = top - bottom;

					//data.normalmap[ y * 33 + x ] = glm::normalize( cubeToSamplePosition( position, size, normal, tangent, radius, x, y ) );//glm::normalize( normal );
					data.normalmap[ y * 33 + x ] = glm::normalize( glm::vec3( sx, sz, sy ) );
					//data.normalmap[ y * 33 + x ] = glm::normalize( glm::vec3( 0.0f, 0.0f, 1.0f ) );

					/*double strength = 1.0 / ( 0.0125 * (double)( 1 << ( maxDepth - depth ) ) );
					double maxHeight = 500000.0;

					//using namespace std::placeholders;
					auto heightFunc = 
						[ position, size, normal, tangent, radius ]( int x, int y )
						{
							//std::bind( getPlanetHeight2, noise, cubeToSamplePosition( position, size, normal, tangent, radius, _1, _2 ) );
							//return getPlanetHeight2( noise, cubeToSamplePosition( position, size, normal, tangent, radius, x, y ) );
							return generateHeight( cubeToSamplePosition( position, size, normal, tangent, radius, x, y ) / 3000000.0 );
						};

					data.normalmap[ y * 33 + x ] = generateNormal( x, y, heightFunc, strength, maxHeight );*/
				}
			}

			data.tangentmap.resize( regionSize * regionSize );

			for( int y = 0; y < regionSize; y++ )
			{
				for( int x = 0; x < regionSize; x++ )
				{
					glm::dvec2 cubePos = ( position - size / 2.0 ) + glm::dvec2( x, y ) * size / 32.0;

					glm::dvec3 pos = normal * radius;
					pos += tangent * cubePos.x;
					pos += glm::normalize( glm::cross( normal, tangent ) ) * cubePos.y;

					pos = glm::normalize( pos ) * radius;

					cubePos = ( position - size / 2.0 ) + glm::dvec2( x + 1, y ) * size / 32.0;

					glm::dvec3 pos2 = normal * radius;
					pos2 += tangent * cubePos.x;
					pos2 += glm::normalize( glm::cross( normal, tangent ) ) * cubePos.y;

					data.tangentmap[ y * regionSize + x ] = glm::normalize( pos2 - pos );
				}
			}

			return data;
		};
	
	glm::dvec3 normal = Planet::FacesNormals[ id ];
	glm::dvec3 tangent = Planet::FacesTangents[ id ];
	
	m_worker.addTask( &node, std::bind( func, m_atlases[ 0 ].getRegionSize(), glm::dvec2( node.position.x, -node.position.y ), node.size, node.depth, getQuadTreeDepth(), normal, tangent ) );

	m_patches[ &node ].region = m_atlases[ 0 ].getNextAvailableRegion();
}

bool Planet::checkNodeStatus( Node& node )
{
	auto data = m_worker.getTaskResults( &node );
	if( !data )
		return false;
	
	processTaskData( *data, node );
	return true;
}

void Planet::aboutToBeDestroy( const Node& node )
{
	const PlanetPatch& patch = m_patches[ &node ];
	m_atlases[ 0 ].releaseRegion( patch.region );

	m_patches.erase( &node );
	m_worker.removeTask( &node );
}

void Planet::bindTexture( PlanetTexture type, int unit ) const
{
	m_atlases[ (int)type ].bind( unit );
}

int Planet::getQuadTreeDepth() const
{
	return glm::log2( m_radius ) - glm::log2( PatchSize / 2.0 );
}

double Planet::getAltitude( const Camera& camera ) const
{
	// should be replace with the planet center
	return glm::length( camera.getPosition() - glm::dvec3( 0.0 ) ) - m_radius;
}