#pragma once
#include <iterator>
#include <memory>
#include <vector>
#include <map>
#include <list>

#include "..\Utility\function.hpp"

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
		using storage_type = std::map<id_type, callable_type>;

		Event()
			:
			id_counter_(0)
		{		}

		~Event()
		{		}

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		[[nodiscard]]
		id_type attach(callable_type&& callback)
		{
			auto res = id_counter_++;

			storage_.insert({ res, std::move(callback) });

			return res;
		}

		template<class Class, class Method>
		[[nodiscard]]
		id_type attach(Class object, Method method)
		{
			auto res = id_counter_++;

			storage_.insert({ res, callable_type(object, method) });

			return res;
		}

		void detach(id_type _Id)
		{
			storage_.erase(_Id);
		}

		void detach_all()
		{
			storage_.clear();
		}

		auto operator()(ArgTypes... _Args)
		{
			std::vector<int64_t> except_ids;

			for (auto& callback : storage_)
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
					storage_.erase(var);
				logger << flush;
			}
		}

		void swap(Event& _Other) noexcept
		{
			//std::swap(mInvokeData, _Other.mInvokeData);
		}

	private:

		int64_t id_counter_;
		storage_type storage_;
	};
}