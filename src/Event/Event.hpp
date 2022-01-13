#pragma once
#include <functional>
#include <list>
#include <vector>
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

		ObjConteiner(Class* _Ptr, Method _Met) :
			mClassPtr(_Ptr),
			mMethod(_Met)
		{};

		Ret call(Args... args)
		{
			if constexpr (std::is_same<Ret, void>::value)
				(mClassPtr->*mMethod)(args...);
			else
				return (mClassPtr->*mMethod)(args...);
		}
	};

	template<class Ret, class... Args>
	class FuncConteiner final : public IConteiner<Ret, Args...>
	{
		using func_t = std::function<Ret(Args...)>;
		std::function<Ret(Args...)> mFunction;

	public:

		FuncConteiner(func_t& _Fn) : mFunction(_Fn) {};
		FuncConteiner(func_t&& _Fn) : mFunction(std::move(_Fn)) {};

		Ret call(Args... args)
		{
			if constexpr (std::is_same<Ret, void>::value)
				mFunction(std::forward<Args>(args)...);
			else
				return mFunction(std::forward<Args>(args)...);
		}
	};

	template<class T>
	class Event
	{
		static_assert("No... Pleas, use that: Event(<return_t(arguments_t...)>)");
	};

	template<class Ret, class... Args>
	class Event<Ret(Args...)> final : public std::list<std::unique_ptr<IConteiner<Ret, Args...>>>
	{
	public:
		using base = std::list<std::unique_ptr<IConteiner<Ret, Args...>>>;
		using u_ptr_type = std::unique_ptr<IConteiner<Ret, Args...>>;
		using function_type = std::function<Ret(Args...)>;
		using func_conteiner_type = typename FuncConteiner<Ret, Args...>;
		
		template<class ClassT>
		using method_type = typename ActualType<Ret(ClassT::*)(Args...)>::type;
		
		template<class ClassT>
		using obj_conteiner_type = ObjConteiner<ClassT, Ret, Args...>;

		Event() {}
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;
		Event(Event&&) = delete;
		Event& operator=(Event&&) = delete;

		template<class ClassT>
		[[nodiscard]] base::iterator attach(ClassT* _Ptr, method_type<ClassT> _Met)
		{
			return base::insert(base::end(), u_ptr_type(new obj_conteiner_type<ClassT>(_Ptr, _Met)));
		}

		[[nodiscard]] base::iterator attach(function_type& _Fn)
		{
			return base::insert(base::end(), u_ptr_type(new func_conteiner_type(_Fn)));
		}

		[[nodiscard]] base::iterator attach(function_type&& _Fn)
		{
			return base::insert(base::end(), u_ptr_type(new func_conteiner_type(std::move(_Fn))));
		}

		[[nodiscard]] size_t detach_all()
		{
			size_t result = base::size();
			base::clear();
			return result;
		}

		auto operator()(Args... args)
		{
			if constexpr (std::is_same<Ret, void>::value)
			{
				for (auto& elem : *this)
					elem->call(args...);
			}
			else
			{
				std::vector<Ret> results;
				for (auto& elem : *this)
					results.emplace_back(elem->call(args...));
				return results;
			}
		}
	};
}