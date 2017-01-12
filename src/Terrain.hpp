#ifndef __TERRAIN_HPP__
#define __TERRAIN_HPP__

#include "QuadTree.hpp"
#include "TerrainWorker.hpp"
#include "TextureAtlas.hpp"
#include "Mesh.hpp"
#include "OpenGL/Shader.hpp"
#include <map>
#include <vector>
#include <glm/glm.hpp>

class RenderTarget;
class Camera;

class TaskData
{
public:
	std::vector< float > heightmap;
	std::vector< glm::vec3 > normalmap;

	double average;
	double min, max;
};

class TerrainPatch
{
public:
	glm::dvec3 position;
	double scale;

	glm::ivec2 region;
};

enum class TerrainTexture
{
	HeightMap,
	Max
};

using PatchKey = const Node*;

class Terrain : public NodeCallbacks
{
public:
	Terrain( double diameter );

	void update( const Camera& camera );

	void renderPatches( RenderTarget& target, const Camera& camera );

	void bindTexture( TerrainTexture type, int unit ) const;

private:
	int getQuadTreeDepth() const;

	void processTaskData( TaskData data, Node& node );

	virtual void orderTask( Node& node, int id = 0 );
	virtual void requestTask( const Node& node, int id = 0 );
	virtual bool checkNodeStatus( Node& node );
	virtual void aboutToBeDestroy( const Node& node );

private:
	double m_radius;
	QuadTree m_quadTree;

	std::map< PatchKey, TerrainPatch > m_patches;
	TerrainWorker< PatchKey, TaskData > m_worker;

	std::vector< TextureAtlas > m_atlases;

	Mesh m_patch;
	Shader m_shader;

	std::vector< ConstNodeRef > m_frameVisibleNodes;

private:
	static const int PatchSize = 32;
	static const int AtlasesNumRegions = 64;
};
#endif