#include "StringSubstitution.h"

#include "Core/HashFunctions/CRC32.h"

size_t VT::StringSubstitution::processSubstring(size_t beginPos, size_t endPos)
{
	const std::string_view substring(&m_resultString[beginPos], endPos - beginPos);
	size_t entrancePos = m_resultString.npos;

	size_t endLimPos = endPos - END_EXP_STR_SIZE + 1;
	for (size_t iterPos = beginPos; iterPos < endLimPos; ++iterPos)
	{
		const char* currentStr = &m_resultString[iterPos];
		if (strncmp(currentStr, BEGIN_EXP_STR, BEGIN_EXP_STR_SIZE) == 0)
		{
			entrancePos = iterPos;

			const size_t initialStringLength = m_resultString.size();
			iterPos = processSubstring(iterPos + BEGIN_EXP_STR_SIZE, endPos);

			const size_t updatedStringSize = m_resultString.size();
			if (updatedStringSize != initialStringLength)
			{
				const size_t deltaSize = updatedStringSize - initialStringLength;

				endPos += deltaSize;
				endLimPos += deltaSize;
			}

			currentStr = &m_resultString[iterPos];
		}

		if (strncmp(currentStr, END_EXP_STR, END_EXP_STR_SIZE) == 0)
		{
			if (entrancePos == m_resultString.npos)
			{
				return iterPos;
			}

			size_t initialStringLength = m_resultString.size();
			replaceSubstring(entrancePos, iterPos + END_EXP_STR_SIZE - 1);

			size_t updatedStringSize = m_resultString.size();
			if (updatedStringSize != initialStringLength)
			{
				const size_t deltaSize = updatedStringSize - initialStringLength;

				endPos += deltaSize;
				endLimPos += deltaSize;
				iterPos += deltaSize;
			}

			initialStringLength = m_resultString.size();
			iterPos = processSubstring(entrancePos, iterPos + 1);
			
			updatedStringSize = m_resultString.size();
			if (updatedStringSize != initialStringLength)
			{
				const size_t deltaSize = updatedStringSize - initialStringLength;

				endPos += deltaSize;
				endLimPos += deltaSize;
			}

			entrancePos = m_resultString.npos;
		}
	}

	return endLimPos - 1;
}

void VT::StringSubstitution::replaceSubstring(size_t beginPos, size_t endPos)
{
	if (beginPos >= endPos)
	{
		return;
	}

	const size_t substringBeginPos = beginPos + BEGIN_EXP_STR_SIZE;
	const size_t substringEndPos = endPos - END_EXP_STR_SIZE;

	const uint32_t substringHash = crc32(&m_resultString[substringBeginPos], substringEndPos - substringBeginPos + 1);
	auto findIter = m_replacementMap.find(substringHash);
	if (findIter == m_replacementMap.end())
	{
		return;
	}

	m_resultString.replace(beginPos, endPos - beginPos + 1, findIter->second);
}

VT::StringSubstitution::ResultStringType VT::StringSubstitution::process()
{
	if (m_resultString.empty() || m_replacementMap.empty())
	{
		return ResultStringType();
	}

	processSubstring(0, m_resultString.size());

	return m_resultString;
}
