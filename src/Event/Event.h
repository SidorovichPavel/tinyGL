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
		virtual Ret call(Args...) { return Ret(); };
	};

	template<class Class, class Ret, class... Args>
	class Conteiner final : public IConteiner<Ret, Args...>
	{
		using Method = typename ActualType<Ret(Class::*)(Args...)>::type;
	public:
		Class* mClassPtr;
		Method mMethod;

		Conteiner(Class* _Ptr, Method _Met) :
			mClassPtr(_Ptr),
			mMethod(_Met)
		{};

		Ret call(Args... args)
		{
			return (mClassPtr->*mMethod)(std::forward<Args...>(args)...);
		}
	};

	template<class T>
	class Event {};

	template<class Ret, class... Args>
	class Event<Ret(Args...)>
	{
		std::list<std::unique_ptr<IConteiner<Ret, Args...>>> mReciversM;
		std::list<std::function<Ret(Args...)>> mReciversF;
	public:
		Event() {}

		template<class ClassT>
		void attach(ClassT* _Ptr, typename ActualType<Ret(ClassT::*)(Args...)>::type _Met)
		{
			mReciversM.emplace_back(new Conteiner<ClassT, Ret, Args...>(_Ptr, _Met));
		}
		void attach(std::function<Ret(Args...)> _Fn)
		{
			mReciversF.push_back(_Fn);
		}

		Ret operator()(Args... args)
		{
			Ret result{};
			for (auto& obj : mReciversM)
				result = obj->call(std::forward<Args>(args)...);
			for (auto& fn : mReciversF)
				result = fn(std::forward<Args>(args)...);
			return result;
		}
	};

	//void specialisation
	template<class... Args>
	class IConteiner<void, Args...>
	{
	public:
		IConteiner() {};
		virtual void call(Args...) {};
	};

	template<class Class, class... Args>
	class Conteiner<Class, void, Args...> final : public IConteiner<void, Args...>
	{
		using Method = typename ActualType<void(Class::*)(Args...)>::type;
	public:
		Class* mClassPtr;
		Method mMethod;

		Conteiner(Class* _Ptr, Method _Met) :
			mClassPtr(_Ptr),
			mMethod(_Met)
		{};

		void call(Args... args)
		{
			(mClassPtr->*mMethod)(std::forward<Args...>(args)...);
		}
	};

	template<class... Args>
	class Event<void(Args...)>
	{
		std::list<std::unique_ptr<IConteiner<void, Args...>>> mReciversM;
		std::list<std::function<void(Args...)>> mReciversF;
	public:
		Event() {}

		template<class ClassT>
		void attach(ClassT* _Ptr, typename ActualType<void(ClassT::*)(Args...)>::type _Met)
		{
			mReciversM.emplace_back(new Conteiner<ClassT, void, Args...>(_Ptr, _Met));
		}
		void attach(std::function<void(Args...)> _Fn)
		{
			mReciversF.push_back(_Fn);
		}

		void operator()(Args... args)
		{
			for (auto& obj : mReciversM)
				obj->call(std::forward<Args>(args)...);
			for (auto& fn : mReciversF)
				fn(std::forward<Args>(args)...);
		}
	};
}