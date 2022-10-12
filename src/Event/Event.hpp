#pragma once
#include <iterator>
#include <memory>
#include <vector>
#include <map>

#include <function.hpp>

#include "..\Logger\Logger.hpp"

namespace tgl
{
	template<class>
	class Event
	{
	public:
		using id_type = int64_t;
		static_assert("No... Pleas, use that: Event<return_t(arguments_t...)>");
	};

	template<class... ArgTypes>
	class Event<void(ArgTypes...)>
	{
	public:

		using callable_type = fnw::function<void(ArgTypes...)>;
		using id_type = int64_t;

		Event()
			:
			mIDCounter(0)
		{		}

		~Event()
		{		}

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		[[nodiscard]]
		id_type attach(callable_type&& _CallBack)
		{
			auto res = mIDCounter++;

			mStorage.insert({ res, std::move(_CallBack) });

			return res;
		}

		template<class Class, class Method>
		[[nodiscard]]
		id_type attach(Class _Object, Method _Method)
		{
			auto res = mIDCounter++;

			mStorage.insert({ res, callable_type(_Object, _Method) });

			return res;
		}

		void detach(id_type _Id)
		{
			mStorage.erase(_Id);
		}

		void detach_all()
		{
			mStorage.clear();
		}

		auto operator()(ArgTypes... _Args)
		{
			std::vector<int64_t> except_ids;

			for (auto& callback : mStorage)
			{
				try
				{
					callback.second(_Args...);
				}
				catch (const std::exception& e)
				{
					except_ids.push_back(callback.first);
					logger << e.what();
				}
			}

			if (!except_ids.empty())
			{
				for (auto& var : except_ids)
					mStorage.erase(var);
				logger << flush;
			}
		}

		void swap(Event& _Other) noexcept
		{
			//std::swap(mInvokeData, _Other.mInvokeData);
		}

	private:

		int64_t mIDCounter;
		std::map<id_type, callable_type> mStorage;
	};
}