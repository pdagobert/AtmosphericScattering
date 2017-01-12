#ifndef __QUAD_TREE_HPP__
#define __QUAD_TREE_HPP__

#include "Collision/AABB.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <memory>
#include <glm/glm.hpp>

class Camera;

class Node
{
public:
	Node( int depth, double size, const glm::dvec2& position );

	void createChildren();
	void setAABB( double minHeight, double maxHeight );

public:
	int depth;
	double size;
	glm::dvec2 position;

	double average;

	std::vector< Node > children;
	
	bool isReady;
	AABB aabb;
};

using NodeRef = std::reference_wrapper< Node >;
using ConstNodeRef = std::reference_wrapper< const Node >;

class NodeCallbacks
{
public:
	virtual void orderTask( Node& node, int id = 0 ) = 0;
	virtual void requestTask( const Node& node, int id = 0 ) = 0;

	virtual bool checkNodeStatus( Node& node ) = 0;

	virtual void aboutToBeDestroy( const Node& node ) = 0;
};

class QuadTree
{
public:
	QuadTree( double radius, int maxDepth, NodeCallbacks& owner, int id = 0 );

	void setSelectNodeFunction( std::function< bool( const Node&, const Camera& ) > selectNode );

	std::vector< ConstNodeRef > updateVisibleNodes( const Camera& camera );

private:
	void releaseNodes( const Node& node );

private:
	std::unique_ptr< Node > m_root;

	std::function< bool( const Node&, const Camera& ) > m_selectNode;

	NodeCallbacks& m_owner;
	int m_id;
};
#endif