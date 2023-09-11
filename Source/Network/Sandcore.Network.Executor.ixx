export module Sandcore.Network.Executor;

import std;

export namespace Sandcore {
	class Executor {
	public:
		using Task = std::function<void()>;

		Executor(std::size_t count = 8) : threads(count) {
			for (auto& thread : threads) {
				thread = std::thread(&Executor::work, this);
			}
		}

		~Executor() {
			{
				std::unique_lock lock(mutex);
				run = false;
			}
			condition.notify_all();

			for (auto& thread : threads) if (thread.joinable()) thread.join();
		}

		void add(Task task) {
			std::unique_lock lock(mutex);
			tasks.push(task);
			condition.notify_one();
		}
	private:
		void work() {
			while (run) {
				Task task;
				{
					std::unique_lock lock(mutex);
					condition.wait(lock, [this]() { return !tasks.empty() || !run; });
					if (!run && tasks.empty()) return;

					task = tasks.front();
					tasks.pop();
				}
				try {
					task();
				} catch (...) {
					add(task);
				}
			}
		}
		bool run = true;

		std::queue<Task> tasks;
		std::deque<std::thread> threads;

		std::mutex mutex;
		std::condition_variable condition;
	};
}