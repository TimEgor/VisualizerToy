#pragma once

#include "XmlNode.h"

namespace VT
{
	namespace XmlParserInterface
	{
		template <typename T>
		void getValue(const XmlNode& node, T& val)
		{
			static_assert(false && "Invalid type for XML parser");
		}

		template <typename T>
		void getValue(const XmlNode& node, T* val)
		{
			static_assert(false && "Invalid type for XML parser");
		}

		template <typename T>
		void setValue(XmlNode& node, const T& val)
		{
			static_assert(false && "Invalid type for XML parser");
		}

		template <typename T>
		void setValue(XmlNode& node, const T* val)
		{
			static_assert(false && "Invalid type for XML parser");
		}
	}
}
