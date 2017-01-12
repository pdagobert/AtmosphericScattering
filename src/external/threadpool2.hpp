/*
 * This file is licensed under the zlib/libpng license, included in this
 * distribution in the file COPYING.
 */

#include <future>
#include <thread>
#include <deque>
#include <vector>
#include <utility>
#include <chrono>
#include <functional>
#include <type_traits>

#ifndef WORKQUEUE_threadpool_hpp
#define WORKQUEUE_threadpool_hpp

namespace workqueue {

class threadpool {
	std::vector<std::thread> pool;
	bool stop;

	std::mutex access;
	std::condition_variable cond;
	std::deque<std::function<void()>> tasks;

public:
	explicit threadpool(int nr = 1) : stop(false) {
		while(nr-->0) {
			add_worker();
		}
	}
	~threadpool() {
		stop = true;
		for(std::thread &t : pool) {
			t.join();
		}
		pool.clear();
	}

	template<class Rt>
	auto add_task(std::packaged_task<Rt(int, double, double, double, int, int)>& pt, int n, double x, double y, double size, int depth, int maxDepth ) -> std::future<Rt> {
		std::unique_lock<std::mutex> lock(access);

		auto ret = pt.get_future();
		tasks.push_back([&pt, n, x, y, size, depth, maxDepth]{pt(n, x, y, size, depth, maxDepth);});

		cond.notify_one();

		return ret;
	}

	/*template< class T >
	std::future< T > add_task( std::âckaged_task< Rt( int ) >& pt, int n )
	{
		std::unique_lock< std::mutex > lock( access );

		auto ret = pt.get_future();
		tasks.push_back( [&pt])
		return ret;
	}*/

private:
	void add_worker() {
		std::thread t([this]() {
			while(!stop) {
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(access);
					if(tasks.empty()) {
						cond.wait_for(lock, std::chrono::duration<int, std::milli>(5));
						continue;
					}
					task = std::move(tasks.front());
					tasks.pop_front();
				}
				task();
			}
		});
		pool.push_back(std::move(t));
	}
};

}

#endif//WORKQUEUE_threadpool_hpp

// vim: syntax=cpp11
