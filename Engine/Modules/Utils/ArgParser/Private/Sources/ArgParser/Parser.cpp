#include "Parser.h"

#include <string_view>

void VT::ArgParser::parse(int argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string_view arg(argv[i]);
		std::string_view argVal(arg);
		std::string_view argName(arg);

		std::size_t valSeparator = arg.find('=');
		if (valSeparator != arg.npos && valSeparator < arg.size() - 1)
		{
			argVal = std::string_view(arg.data() + valSeparator + 1);
			argName = std::string_view(arg.data(), valSeparator);
		}

		auto findIter = m_options.find(std::string(argName, 0, argName.size()));
		if (findIter != m_options.end())
		{
			findIter->second->parse(argVal.data());
		}
	}
}

void VT::ArgParser::clear()
{
	for (auto& options : m_options)
	{
		delete options.second;
	}

	m_options = OptionContainer();
}
