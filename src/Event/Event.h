#pragma once
#include <functional>
#include <list>
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
	class Conteiner final : public IConteiner<Ret, Args...>
	{
		using Method = typename ActualType<Ret(Class::*)(Args...)>::type;
		Class* mClassPtr;
		Method mMethod;

	public:

		Conteiner(Class* _Ptr, Method _Met) :
			mClassPtr(_Ptr),
			mMethod(_Met)
		{};

		Ret call(Args... args)
		{
			if constexpr (std::is_same<Ret, void>::value)
				(mClassPtr->*mMethod)(std::forward<Args>(args)...);
			else
				return (mClassPtr->*mMethod)(std::forward<Args>(args)...);
		}
	};

	template<class T>
	class Event 
	{
		static_assert("No... Pleas, use that: Event(<return_t(arguments_t...)>)");
	};

	template<class Ret, class... Args>
	class Event<Ret(Args...)>
	{
		using function_type = std::function<Ret(Args...)>;
		template<class ClassT>
		using method_type = typename ActualType<Ret(ClassT::*)(Args...)>::type;

		std::list<std::unique_ptr<IConteiner<Ret, Args...>>> mReciversM;
		std::list<function_type> mReciversF;
	public:
		Event() {}
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		template<class ClassT>
		void attach(ClassT* _Ptr, method_type<ClassT> _Met)
		{
			mReciversM.emplace_back(new Conteiner<ClassT, Ret, Args...>(_Ptr, _Met));
		}
		void attach(function_type&& _Fn)
		{
			mReciversF.emplace_back(std::forward<function_type>(_Fn));
		}
		size_t detach_all()
		{
			size_t result = mReciversM.size() + mReciversF.size();
			mReciversM.clear();
			mReciversF.clear();
			return result;
		}
		Ret operator()(Args... args)
		{
			if constexpr (std::is_same<Ret, void>::value)
			{
				for (auto& obj : mReciversM)
					obj->call(args...);
				for (auto& fn : mReciversF)
					fn(args...);
			}
			else
			{
				Ret result;
				for (auto& obj : mReciversM)
					result = obj->call(args...);
				for (auto& fn : mReciversF)
					result = fn(args...);
				return result;
			}
		}
		void swap(Event* _Event) noexcept
		{
			mReciversM.swap(_Event->mReciversM);
			mReciversF.swap(_Event->mReciversF);
		}
	};
}