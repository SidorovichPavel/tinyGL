#pragma once

/*
* RayeS
* class - callable object wraper
* version 1.4.0
*/

#include <type_traits>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace fnw
{
	namespace detail
	{

		template<class ReturnType, class... ArgTypes>
		class function_base
		{
		public:
			function_base() = default;
			virtual ~function_base() = default;

			virtual ReturnType invoke(ArgTypes&&... args) = 0;
		};

		template<class FunctionType, class ReturnType, class... ArgTypes>
		class func_impl : public function_base<ReturnType, ArgTypes...>
		{
		public:
			func_impl(FunctionType _Function)
				:
				mFunction(_Function)
			{}

			~func_impl() = default;

			ReturnType invoke(ArgTypes&&... _Args) override
			{
				return mFunction(std::forward<ArgTypes>(_Args)...);
			};
			

		private:
			FunctionType mFunction;
		};

		template<class Class, class Method, class ReturnType, class... ArgTypes>
		class method_impl : public function_base<ReturnType, ArgTypes...>
		{
		public:
			method_impl(Class _Obj, Method _Met)
				:
				mObject(_Obj),
				mMethod(_Met)
			{}

			~method_impl() = default;

			ReturnType invoke(ArgTypes&&... _Args) override
			{
				if constexpr (std::is_pointer_v<Class>)
					return (mObject->*mMethod)(std::forward<ArgTypes>(_Args)...);
				else
					return (mObject.*mMethod)(std::forward<ArgTypes>(_Args)...);
			}


		private:
			Class mObject;
			Method mMethod;
		};

	}
	class fnw_error : public std::runtime_error
	{
	public:
		fnw_error(const char* _What)
			:
			runtime_error(_What)
		{}
	};

	template<class>
	class function {};

	template<class ReturnType, class... ArgTypes>
	class function<ReturnType(ArgTypes...)>
	{
		using invoker_ptr = detail::function_base<ReturnType, ArgTypes...>*;
		using invoker_t = detail::function_base<ReturnType, ArgTypes...>;

		enum class StoreMod : uint8_t
		{
			Free = 0,
			Pointer = 1,
			Object = 2
		};

	public:
		function(nullptr_t _Ptr = nullptr)
			:
			mStorage{0}
		{}

		function(const function&) = delete;
		function& operator=(const function&) = delete;

		function(function&& _Other) noexcept
		{
			swap(_Other);
		}

		function& operator=(function&& _Right) noexcept
		{
			swap(_Right);
			return *this;
		}

		template<class FunctionType>
		function(FunctionType _Function)
			:
			mStorage{ 0 }
		{
			using func_impl_t = detail::func_impl<FunctionType, ReturnType, ArgTypes...>;

			if constexpr (std::is_invocable_v<FunctionType, ArgTypes...>)
				store<func_impl_t>(_Function);
		}

		template<class Class, class Method>
		function(Class _Object, Method _Method)
			:
			mStorage{ 0 }
		{
			using method_impl_t = detail::method_impl<Class, Method, ReturnType, ArgTypes...>;

			if constexpr (std::is_invocable_v<Method, Class, ArgTypes...>)
				store<method_impl_t>(_Object, _Method);
		}

		~function()
		{
			free_storage();
		}

		template<class... ParamTypes>
		ReturnType operator()(ParamTypes&&... _Params)
		{
			switch (static_cast<StoreMod>(mStorage[flags_block]))
			{
			case StoreMod::Free:
				throw fnw_error("Function don't have callable object");
			case StoreMod::Object:
				return reinterpret_cast<invoker_ptr>(mStorage)->invoke(std::forward<ArgTypes>(static_cast<ArgTypes&&>(_Params))...);
			case StoreMod::Pointer:
				return (*reinterpret_cast<invoker_ptr*>(mStorage))->invoke(std::forward<ArgTypes>(static_cast<ArgTypes&&>(_Params))...);
			}

			throw fnw_error("The function store has been corrupted");
		}

	private:

		void swap(function& _Other) noexcept
		{
			uint8_t buffer[storage_size + 1];
			std::copy(std::begin(mStorage), std::end(mStorage), std::begin(buffer));
			std::copy(std::begin(_Other.mStorage), std::end(_Other.mStorage), std::begin(mStorage));
			std::copy(std::begin(buffer), std::end(buffer), std::begin(_Other.mStorage));
		}

		template<class Conteiner, class... Args>
		void store(Args&&... _Args)
		{
			if constexpr (sizeof(Conteiner) > storage_size)
			{
				invoker_ptr ptr = new Conteiner(std::forward<Args>(_Args)...);
				new (mStorage) invoker_ptr(ptr);
				mStorage[flags_block] = static_cast<uint8_t>(StoreMod::Pointer);
			}
			else
			{
				new (mStorage) Conteiner(std::forward<Args>(_Args)...);
				mStorage[flags_block] = static_cast<uint8_t>(StoreMod::Object);
			}
		}

		void free_storage() noexcept
		{
			switch (static_cast<StoreMod>(mStorage[flags_block]))
			{
			case StoreMod::Free:
				break;
			case StoreMod::Object:
				reinterpret_cast<invoker_ptr>(mStorage)->~function_base();
				break;
			case StoreMod::Pointer:
				delete *reinterpret_cast<invoker_ptr*>(mStorage);
				break;
			}

			std::fill(std::begin(mStorage), std::end(mStorage), 0ui8);
		}

		static constexpr size_t block_size = 8;//bytes
		static constexpr size_t storage_size = block_size * 4;//fix x86 release
		static constexpr size_t flags_block = storage_size;
		uint8_t mStorage[storage_size + 1];
	};



}