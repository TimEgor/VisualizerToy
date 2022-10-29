#pragma once

#include "volk.h"

#include <cassert>

namespace VT_VK
{
	inline bool checkVkResult(VkResult result)
	{
		return result == VK_SUCCESS;
	}

	inline void checkVkResultAssert(VkResult result)
	{
		assert(checkVkResult(result));
	}

	inline bool checkVkResultReturnAssert(VkResult result)
	{
		const bool chkResult = checkVkResult(result);
		assert(chkResult);

		return chkResult;
	}
}