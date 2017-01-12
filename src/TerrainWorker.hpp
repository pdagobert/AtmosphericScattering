#ifndef __TERRAIN_WORKER_HPP__
#define __TERRAIN_WORKER_HPP__

#include "external/threadpool.hpp"
#include <map>
#include <exception>
#include <chrono>
#include <algorithm>
#include <experimental/optional>
#include <iostream>

template< class TaskReturnType >
class RunningTask
{
public:
	bool consumed;
	std::future< TaskReturnType > future;
	std::unique_ptr< std::packaged_task< TaskReturnType() > > task;
};

template< class TaskIdType, class TaskReturnType >
class TerrainWorker
{
public:
	TerrainWorker() :
		m_pool( 4 )
	{

	}

	void addTask( TaskIdType id, std::function< TaskReturnType() > func )
	{
		RunningTask< TaskReturnType >& task = m_runningTasks[ id ];
		task.task = std::unique_ptr< std::packaged_task< TaskReturnType() > >( new std::packaged_task< TaskReturnType() >( func ) );
		task.future = m_pool.add_task( *task.task );
		task.consumed = false;
	}

	void removeTask( TaskIdType id )
	{
		auto it = m_runningTasks.find( id );
		if( it == m_runningTasks.end() )
			throw std::runtime_error( "no task for this id" );
		
		// if the task hasn't been consumed, then it is probably still beeing processed by the queue
		// we move the task in a vector to avoid problems until it finishes
		if( !it->second.consumed )
			m_invalidTasks.push_back( std::move( it->second ) );
		
		m_runningTasks.erase( it ); 
	}

	std::experimental::optional< TaskReturnType > getTaskResults( TaskIdType id )
	{
		auto it = m_runningTasks.find( id );
		if( it == m_runningTasks.end() )
			throw std::runtime_error( "no task for this id" );
		
		if( isReady( it->second ) )
			return it->second.future.get();
		else
			return {};
	}

	TaskReturnType waitTaskResults( TaskIdType id )
	{
		auto it = m_runningTasks.find( id );
		if( it == m_runningTasks.end() )
			throw std::runtime_error( "no task for this id" );
		
		it->second.future.wait();
		return it->second.future.get();
	}

	void removeInvalidTasks()
	{
		m_invalidTasks.erase( std::remove_if( m_invalidTasks.begin(), m_invalidTasks.end(),
			[]( const RunningTask< TaskReturnType >& task )
			{
				return isReady( task );
			} ) );
	}

private:
	static bool isReady( const RunningTask< TaskReturnType >& task )
	{
		return ( task.future.wait_for( std::chrono::milliseconds( 0 ) ) == std::future_status::ready );
	}

private:
	workqueue::threadpool m_pool;

	std::map< TaskIdType, RunningTask< TaskReturnType > > m_runningTasks;
	std::vector< RunningTask< TaskReturnType > > m_invalidTasks;
};
#endif