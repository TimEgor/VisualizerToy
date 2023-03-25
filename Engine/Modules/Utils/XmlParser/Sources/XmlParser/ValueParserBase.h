#pragma once

#include "XmlParser/ValueParserInterface.h"

#include "Core/FileName/FileName.h"

namespace VT
{
	namespace XmlParserInterface
	{
		template<>
		inline void getValue(const XmlNode& node, bool& val)
		{
			val = node.getValue().get<bool>();
		}

		template<>
		inline void getValue(const XmlNode& node, unsigned& val)
		{
			val = node.getValue().get<unsigned>();
		}

		template<>
		inline void getValue(const XmlNode& node, int& val)
		{
			val = node.getValue().get<int>();
		}

		template<>
		inline void getValue(const XmlNode& node, float& val)
		{
			val = node.getValue().get<float>();
		}

		template<>
		inline void getValue(const XmlNode& node, double& val)
		{
			val = node.getValue().get<double>();
		}

		template <>
		inline void getValue(const XmlNode& node, char** val)
		{
			strcpy(*val, node.getValue().get<const char*>());
		}

		template <>
		inline void getValue(const XmlNode& node, FileName& val)
		{
			val = node.getValue().get<const char*>();
		}

		template <>
		inline void setValue(XmlNode& node, const bool& val)
		{
			node.getValue().set(val);
		}

		template <>
		inline void setValue(XmlNode& node, const unsigned& val)
		{
			node.getValue().set(val);
		}

		template <>
		inline void setValue(XmlNode& node, const int& val)
		{
			node.getValue().set(val);
		}

		template <>
		inline void setValue(XmlNode& node, const float& val)
		{
			node.getValue().set(val);
		}

		template <>
		inline void setValue(XmlNode& node, const double& val)
		{
			node.getValue().set(val);
		}

		template <>
		inline void setValue(XmlNode& node, const char* val)
		{
			node.getValue().set(val);
		}

		template <>
		inline void setValue(XmlNode& node, const FileName& val)
		{
			node.getValue().set(val.c_str());
		}
	}
}