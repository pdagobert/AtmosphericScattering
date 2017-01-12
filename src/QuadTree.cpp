#include "QuadTree.hpp"
#include <queue>

Node::Node( int depth, double size, const glm::dvec2& position ) :
	depth( depth ),
	size( size ),
	position( position ),
	isReady( false )
{

}

void Node::createChildren()
{
	children.reserve( 4 );

	int childDepth = depth - 1;
	double childSize = size / 2.0;
	double childRadius = childSize / 2.0;

	children.push_back( Node( childDepth, childSize, position + glm::dvec2( childRadius, childRadius ) ) );
	children.push_back( Node( childDepth, childSize, position + glm::dvec2( childRadius, -childRadius ) ) );
	children.push_back( Node( childDepth, childSize, position + glm::dvec2( -childRadius, childRadius ) ) );
	children.push_back( Node( childDepth, childSize, position + glm::dvec2( -childRadius, -childRadius ) ) );
}

void Node::setAABB( double minHeight, double maxHeight )
{
	double radius = size / 2.0;

	glm::dvec3 min( position.x - radius, minHeight, -position.y - radius );
	glm::dvec3 max( position.x + radius, maxHeight, -position.y + radius );

	aabb = AABB( min, max );
}

QuadTree::QuadTree( double radius, int maxDepth, NodeCallbacks& owner, int id ) :
	m_root( new Node( maxDepth, radius * 2.0, glm::dvec2( 0.0 ) ) ),
	m_owner( owner ),
	m_id( id )
{

}

void QuadTree::setSelectNodeFunction( std::function< bool( const Node&, const Camera& ) > selectNode )
{
	m_selectNode = selectNode;
}

std::vector< ConstNodeRef > QuadTree::updateVisibleNodes( const Camera& camera )
{
	/*if( !m_selectNode( *m_root, camera ) )
		return {};*/
	
	std::queue< NodeRef > nodes;
	nodes.push( *m_root );

	// build root if needed
	if( !m_root->isReady )
		m_owner.orderTask( *m_root, m_id );

	std::vector< ConstNodeRef > visibleNodes;

	while( !nodes.empty() )
	{
		Node& current = nodes.front();
		if( current.depth == 0 )
		{
			visibleNodes.push_back( current );
			nodes.pop();

			continue;
		}

		if( current.children.empty() )
		{
			current.createChildren();

			for( Node& child : current.children )
				m_owner.requestTask( child, m_id );
		}

		bool ready = true;

		for( Node& child : current.children )
		{
			if( !child.isReady )
			{
				if( !m_owner.checkNodeStatus( child ) )
				{
					ready = false;
					break;
				}
			}
		}

		if( !ready )
		{
			visibleNodes.push_back( current );
			nodes.pop();
			continue;
		}

		bool oneChildVisible = false;

		for( Node& child : current.children )
		{
			if( current.depth > ( m_root->depth - 2 ) || m_selectNode( child, camera ) )
			{
				nodes.push( child );
				oneChildVisible = true;
			}
			else
			{
				visibleNodes.push_back( child );

				releaseNodes( child );
				child.children.clear();
			}
		}

		if( !oneChildVisible )
		{
			visibleNodes.pop_back();
			visibleNodes.pop_back();
			visibleNodes.pop_back();
			visibleNodes.pop_back();	

			visibleNodes.push_back( current );

			for( Node& grandchild : current.children )
			{
				releaseNodes( grandchild );
				grandchild.children.clear();
			}
		}

		nodes.pop();
	}

	return visibleNodes;
}

void QuadTree::releaseNodes( const Node& node )
{
	for( const Node& child : node.children )
	{
		m_owner.aboutToBeDestroy( child );
		releaseNodes( child );
	}
}