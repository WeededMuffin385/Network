export module Sandcore.Network.ThreadSafeQueue;

import std;

export namespace Sandcore {
	template <typename T>
	class ThreadSafeQueue {
	public:
		T& front() {
			std::scoped_lock lock(mutex);
			return queue.front();
		}
		void pop() {
			std::scoped_lock lock(mutex);
			queue.pop();
		}

		void push(T value) {
			std::scoped_lock lock(mutex);
			queue.push(value);
		}

		bool empty() {
			std::scoped_lock lock(mutex);
			return queue.empty();
		}

		std::size_t size() {
			std::scoped_lock lock(mutex);
			return queue.size();
		}

		std::mutex mutex;
	private:
		std::queue<T> queue;
	};
}