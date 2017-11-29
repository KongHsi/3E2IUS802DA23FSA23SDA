#ifndef DYNAMIC_QUEUE_H
#define DYNAMIC_QUEUE_H

#include <deque>
#include <condition_variable>
#include <mutex>
#include "Task.h"

class DynamicQueue{
	std::deque<Task*> buff_;
	std::mutex mutex_;
	std::condition_variable cv_;

public:
	DynamicQueue() :
		buff_(), mutex_(), cv_() {}

	void add(Task* task) {
		{
			std::unique_lock<decltype(mutex_)> lock(mutex_);
			buff_.push_back(task);
			cv_.notify_all();
		}
	}

	Task* get() {
		Task* task;
		{
			std::unique_lock<decltype(mutex_)> lock(mutex_);
			while (buff_.size() == 0)
				cv_.wait(lock);
			task = buff_.front();
			buff_.pop_front();
		}
		return task;
	}
};

#endif
