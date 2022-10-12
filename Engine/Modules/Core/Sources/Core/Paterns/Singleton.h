#pragma once

namespace VT
{
	template <typename T>
	class Singleton
	{
	protected:
		Singleton() = default;

	public:
		virtual ~Singleton() {}

		static T& getInstance();
	};

	template<typename T>
	inline T& Singleton<T>::getInstance()
	{
		static T uniqueInstance;
		return uniqueInstance;
	}
}