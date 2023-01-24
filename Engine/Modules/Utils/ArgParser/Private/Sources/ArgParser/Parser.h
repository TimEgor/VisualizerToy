#pragma once

#include "OptionalValue.h"
#include "ValueParserBase.h"

#include <unordered_map>

namespace VT
{
	class ArgParser final
	{
		using OptionContainer = std::unordered_map<std::string, BaseOptionValue*>;

	private:
		OptionContainer m_options;

	public:
		ArgParser() = default;
		~ArgParser() { clear(); }

		template <typename OptionType>
		void addOptionValue(const char* optionName, OptionType& value, typename OptionValue<OptionType>::SetterType setter = nullptr)
		{
			m_options.insert(std::make_pair(optionName, new OptionValue<OptionType>(optionName, value, setter)));
		}

		void parse(int argc, char** argv);
		void clear();
	};
}