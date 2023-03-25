#pragma once

#include <string>
#include <unordered_map>

namespace VT
{
	class StringSubstitutionProcessor final
	{
		using ResultStringType = std::string;

		inline static constexpr const char* BEGIN_EXP_STR = "${";
		inline static constexpr const char* END_EXP_STR = "}";
		inline static constexpr size_t BEGIN_EXP_STR_SIZE = std::char_traits<char>::length(BEGIN_EXP_STR);
		inline static constexpr size_t END_EXP_STR_SIZE = std::char_traits<char>::length(END_EXP_STR);

	public:
		using ReplacementMap = std::unordered_map<uint32_t, std::string>;

	private:
		const ReplacementMap& m_replacementMap;
		ResultStringType m_resultString;

		size_t processSubstring(size_t beginPos, size_t endPos);
		void replaceSubstring(size_t beginPos, size_t endPos);

	public:
		StringSubstitutionProcessor(const char* baseStr, const ReplacementMap& replacementMap)
			: m_replacementMap(replacementMap), m_resultString(baseStr) {}

		ResultStringType process();
	};

	inline std::string stringSubstitute(const char* baseStr, const StringSubstitutionProcessor::ReplacementMap& replacementMap)
	{
		StringSubstitutionProcessor processor(baseStr, replacementMap);
		return processor.process();
	}
}
