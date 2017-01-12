#ifndef __PLANET_HPP__
#define __PLANET_HPP__

#include "QuadTree.hpp"
#include "TerrainWorker.hpp"
#include "TextureAtlas.hpp"
#include "Mesh.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/Texture.hpp"
#include <map>
#include <vector>

class Camera;
class RenderTarget;

class PlanetTaskData
{
public:
	std::vector< float > heightmap;
	std::vector< glm::vec3 > normalmap;
	std::vector< glm::vec3 > tangentmap;

	double average;
	double min, max;
};

class PlanetPatch
{
public:
	glm::dvec3 position;
	double scale;

	glm::ivec2 region;
};

enum class PlanetTexture
{
	HeightMap,
	NormalMap,
	TangentMap,
	Max
};

using PlanetPatchKey = const Node*;

class Planet : public NodeCallbacks
{
public:
	Planet( double diameter );

	void update( const Camera& camera );

	void renderPatches( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const glm::vec3& betaR, float betaM );

	void bindTexture( PlanetTexture type, int unit ) const;
	double getAltitude( const Camera& camera ) const;

private:
	int getQuadTreeDepth() const;

	glm::dvec3 getPatchNormal( const glm::dvec3& point, const PlanetPatch& patch, int face ) const;
	glm::dvec3 getPatchCorner( const glm::dvec3& point, const PlanetPatch& patch, int face, bool addAverage ) const;
	glm::dvec3 getInterpolatedPatchCorner( const PlanetPatch& patch, int face ) const;

	virtual void orderTask( Node& node, int id = 0 );
	virtual void requestTask( const Node& node, int id = 0 );
	virtual bool checkNodeStatus( Node& node );
	virtual void aboutToBeDestroy( const Node& node );

	void processTaskData( PlanetTaskData data, Node& node );

private:
	double m_radius;
	std::vector< QuadTree > m_quadTrees;

	std::map< PlanetPatchKey, PlanetPatch > m_patches;
	TerrainWorker< PlanetPatchKey, PlanetTaskData > m_worker;

	std::vector< TextureAtlas > m_atlases;

	Mesh m_patch;
	Shader m_shader;

	std::vector< Texture2D > m_diffuseTextures;

	std::vector< ConstNodeRef > m_frameVisibleNodes[ 6 ];

private:
	static const int PatchSize = 32;
	static const int AtlasesNumRegions = 64;

	static glm::vec3 FacesNormals[ 6 ], FacesTangents[ 6 ];
	static glm::dvec3 Points[];
};
#endif