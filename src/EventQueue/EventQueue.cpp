#include "EventQueue.hpp"

#include <queue>
#include <mutex>
#include "..\Event\Event.hpp"

template<class T, size_t ReaderCount = 1>
class EventQueue
{
public:
	EventQueue() noexcept {};
	~EventQueue() noexcept {};

	void enqueue(const T& _Val)
	{
		mQueue.push(_Val);
	}
	void enqueue(T&& _Val)
	{
		mQueue.push(std::move(_Val));
	}

	T get_value() noexcept
	{
		return mQueue.front();
	}

	void pop() noexcept
	{
		if constexpr (ReaderCount == 1)
		{
			std::unique_lock<std::mutex> locker(mMutex);
			mQueue.pop();
		}
		else
		{
			mQueue.pop();
		}
	}

private:
	std::queue<T> mQueue;
	std::mutex mMutex;
};