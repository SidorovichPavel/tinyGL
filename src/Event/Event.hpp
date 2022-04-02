#pragma once
#include <functional>
#include <list>
#include <vector>
#include <array>
#include <iterator>
#include <memory>

namespace tgl
{
	namespace detail
	{
		template<class T>
		struct ActualType
		{
			using type = T;
		};

		template<class Ret, class... Args>
		class IConteiner
		{
		public:
			IConteiner() {};
			virtual ~IConteiner() {};
			virtual Ret call(Args...) = 0;
		};



		template<class Class, class Ret, class... Args>
		class ObjConteiner final : public IConteiner<Ret, Args...>
		{
			using Method = typename ActualType<Ret(Class::*)(Args...)>::type;
			Class* mClassPtr;
			Method mMethod;

		public:

			ObjConteiner(Class* _Ptr, Method _Met) noexcept
				:
				mClassPtr(_Ptr),
				mMethod(_Met)
			{};

			Ret call(Args... args)
			{
				if constexpr (std::is_same_v<Ret, void>)
					(mClassPtr->*mMethod)(args...);
				else
					return (mClassPtr->*mMethod)(args...);
			}
		};

		template<class Ret, class... Args>
		class FuncConteiner final : public IConteiner<Ret, Args...>
		{
			using func_t = ActualType<Ret(__cdecl*)(Args...)>::type;
			func_t mFunction;

		public:

			/*pleas, see youre call type*/
			FuncConteiner(func_t _Fn)
				:
				mFunction(_Fn)
			{};

			Ret call(Args... args)
			{
				if constexpr (std::is_same_v<Ret, void>)
					mFunction(std::forward<Args>(args)...);
				else
					return mFunction(std::forward<Args>(args)...);
			}
		};

	}

	template<class T>
	class Event
	{
	public:
		static_assert("No... Pleas, use that: Event<return_t(arguments_t...)>");
	};

	class id_type_info {};

	template<>
	class Event<id_type_info>
	{
	public:
		static_assert("No... Pleas, use that: Event<return_t(arguments_t...)>");
		using id_type = void*;
	};

	template<class Ret, class... Params>
	class Event<Ret(Params...)>
	{
	public:
		using IConteiner = detail::IConteiner<Ret, Params...>;
		using function_type = detail::ActualType<Ret(*)(Params...)>::type;
		using func_conteiner_type = detail::FuncConteiner<Ret, Params...>; //sizeof(func_conteiner_type) == 8;

		template<class ClassT>
		using method_type = detail::ActualType<Ret(ClassT::*)(Params...)>::type;

		template<class ClassT>
		using obj_conteiner_type = detail::ObjConteiner<ClassT, Ret, Params...>; //sizeof(obj_conteiner_type) == 12;

		using id_type = void*;

		Event()
		{
			zero_data();
		}

		~Event()
		{
			detach_all();
		}

		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		template<class ClassT>
		[[nodiscard]]
		id_type attach(ClassT* _Ptr, method_type<ClassT> _Met)
		{
			auto place = get_place();
			if (is_valid(place))
				new(place) obj_conteiner_type<ClassT>(_Ptr, _Met);

			return place;
		}

		[[nodiscard]]
		id_type attach(function_type _Fn)
		{
			auto place = get_place();
			if (is_valid(place))
				new(place) func_conteiner_type(_Fn);

			return place;
		}

		void detach(id_type _Id)
		{
			auto place = static_cast<uint8_t*>(_Id);
			if (is_valid(place))
			{
				auto obj = static_cast<IConteiner*>(_Id);
				obj->~IConteiner();
				std::fill(place, place + mObjSize, 0ui8);
			}
		}

		void detach_all()
		{
			auto [begin, end] = get_data_range();
			for (auto place = begin; place != end; place += mObjSize)
				if (*place)
				{
					auto obj = reinterpret_cast<IConteiner*>(place);
					obj->~IConteiner();
				}
			zero_data();
		}

		template<class... Args>
		auto operator()(Args&&... args)
		{
			if constexpr (std::is_same_v<Ret, void>)
			{
				auto [begin, end] = get_data_range();
				for (auto place = begin; place != end; place += mObjSize)
				{
					auto test_place = reinterpret_cast<size_t*>(place);
					if (!*test_place)
						continue;

					auto obj = reinterpret_cast<IConteiner*>(place);
					obj->call(std::forward<Args>(args)...);
				}
			}
			else
			{
				std::vector<Ret> results;
				auto [begin, end] = get_data_range();
				for (auto place = begin; place != end; place += mObjSize)
				{
					auto test_place = reinterpret_cast<size_t*>(place);
					if (!*test_place)
						continue;

					auto obj = reinterpret_cast<IConteiner*>(place);
					results.push_back(obj->call(std::forward<Args>(args)...));

				}
				return results;
			}
		}

		void swap(Event& _Other) noexcept
		{
			std::swap(mInvokeData, _Other.mInvokeData);
		}

	private:

		uint8_t* get_place() noexcept
		{
			auto [begin, end] = get_data_range();
			for (auto place = begin; place != end; place += mObjSize)
				if (!*place)
					return place;

			return end;
		}

		std::pair<uint8_t*, uint8_t*> get_data_range() noexcept
		{
			return { mInvokeData.data(), mInvokeData.data() + mInvokeData.size() };
		}

		bool is_valid(uint8_t* _Place) noexcept
		{
			auto [begin, end] = get_data_range();
			return _Place >= begin && _Place < end;
		}

		void zero_data() noexcept
		{
			std::fill(mInvokeData.begin(), mInvokeData.end(), 0ui8);
		}

		static constexpr auto mObjSize = sizeof(size_t) * 3;	// hack. i think about it
		static constexpr auto mMaxObjNumber = 5;

		std::array<uint8_t, (mMaxObjNumber* mObjSize)> mInvokeData;
	};
}