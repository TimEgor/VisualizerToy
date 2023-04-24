#pragma once

#include <functional>

namespace VT
{
	class BaseOptionValue
	{
	private:
		const char* m_argStr;

	public:
		BaseOptionValue(const char* argStr)
			: m_argStr(argStr) {}
		virtual ~BaseOptionValue() {}

		virtual void parse(const char* valStr) = 0;
	};

	template <typename OptionValType>
	class OptionValue final : public BaseOptionValue
	{
	public:
		using SetterType = std::function<void(const char*, OptionValType&)>;

	private:
		OptionValType& m_value;
		SetterType m_setter = nullptr;

	public:
		OptionValue(const char* argStr, OptionValType& value, SetterType setter = nullptr)
			: BaseOptionValue(argStr), m_value(value), m_setter(setter) {}

		virtual void parse(const char* valStr) override
		{
			if (m_setter)
			{
				m_setter(valStr, m_value);
			}
			else
			{
				ArgParserInterface::parseValue(valStr, m_value);
			}
		}
	};
}
