#pragma once
#include"net_common.h" 

namespace olc
{
	namespace net
	{
		template<typename T>
		class tsqueue
		{
		public: 
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			~tsqueue()
			{
				clear();
			}

		public:
			// Return and maintains item at front of Queue
			const T& front()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.front();
			}

			// Return and maintains item at back of Queue
			const T& front()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.back();
			}

			// Add item to the back of the Queue
			void push_back(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_back(std::move(item));
			}

			// Add item to the front of the Queue
			void push_back(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.emplace_front(std::move(item));
			}

			// Returns true if Queue has no items
			bool empty()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.empty();
			}

			// Returns number of items in Queue
			size_t count()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.size();
			}

			// Erase all items in Queue
			void clear()
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.clear();
			}

			// Removes and returns item from front of Q
			T pop_front()
			{
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deqQueue.front());
				deqQueue.pop_front();
				return t;
			}

			// Removes and returns item from back Q
			T pop_front()
			{
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deqQueue.back();
				deqQueue.pop_back();
				return t;
			}

		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
		};
	}
}