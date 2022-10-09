#pragma once

namespace VT
{
	template <typename T>
	class Singelton
	{
	protected:
		Singelton() = default;

	public:
		virtual ~Singelton() {}

		static T& getInstance();
	};

	template<typename T>
	inline T& Singelton<T>::getInstance()
	{
		static T uniqueInstance;
		return uniqueInstance;
	}
}