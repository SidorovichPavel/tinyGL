#pragma once
#include <functional>
#include <list>
#include <vector>
#include <array>
#include <iterator>
#include <memory>

namespace tgl
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
		using id_type = int;
	};

	template<class Ret, class... Args>
	class Event<Ret(Args...)>
	{
	public:
		using IConteiner = IConteiner<Ret, Args...>;
		using function_type = ActualType<Ret(*)(Args...)>::type;
		using func_conteiner_type = FuncConteiner<Ret, Args...>; //sizeof(func_conteiner_type) == 8;

		template<class ClassT>
		using method_type = ActualType<Ret(ClassT::*)(Args...)>::type;

		template<class ClassT>
		using obj_conteiner_type = ObjConteiner<ClassT, Ret, Args...>; //sizeof(obj_conteiner_type) == 12;

		using id_type = int;

		Event() {}
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		template<class ClassT>
		[[nodiscard]]
		id_type attach(ClassT* _Ptr, method_type<ClassT> _Met)
		{
			auto [id, place] = get_place();
			if (place == nullptr)
				return id;
			
			new(place) obj_conteiner_type<ClassT>(_Ptr, _Met);
			return id;
		}

		[[nodiscard]]
		id_type attach(function_type _Fn)
		{
			auto [id, place] = get_place();
			if (place == nullptr)
				return id;

			new(place) func_conteiner_type(_Fn);
			return id;
		}

		void detach(id_type _Id)
		{
			mInvokeMask ^= _Id;
		}

		void detach_all()
		{
			mInvokeMask = 0;
		}

		auto operator()(Args... args)
		{
			if constexpr (std::is_same_v<Ret, void>)
			{
				for (auto idx = 0; idx < mMaxObjNumber; idx++)
				{
					int mask = 1 << idx;
					if (mask & mInvokeMask)
					{
						auto elem = reinterpret_cast<IConteiner*>(mInvokeData.data() + idx * mObjSize);
						elem->call(args...);
					}
				}
			}
			else
			{
				std::vector<Ret> results;
				for (auto idx = 0; idx < mMaxObjNumber; idx++)
				{
					int mask = 1 << idx;
					if (mask & mInvokeMask)
					{
						auto elem = reinterpret_cast<IConteiner*>(mInvokeData.data() + idx * mObjSize);
						results.push_back(elem->call(args...));
					}
				}
				return results;
			}
		}

		void swap(Event& _Other) noexcept
		{
			std::swap(mInvokeMask, _Other.mInvokeMask);
			std::swap(mInvokeData, _Other.mInvokeData);
		}

	private:

		std::pair<int, uint8_t*> get_place() noexcept
		{
			for (int idx = 0; idx < mMaxObjNumber; idx++)
			{
				int mask = 1 << idx;
				if (!(mask & mInvokeMask))
				{
					mInvokeMask |= mask;
					return { mask, mInvokeData.data() + idx * mObjSize };
				}
			}
			return { -1, nullptr };
		}

		int mInvokeMask;
		static constexpr auto mObjSize = sizeof(size_t) * 3;
		static constexpr auto mMaxObjNumber = 5;
		static constexpr auto mMaxMaskValue = 1 << mMaxObjNumber;
		std::array<uint8_t, (mMaxObjNumber* mObjSize)> mInvokeData;
	};
}