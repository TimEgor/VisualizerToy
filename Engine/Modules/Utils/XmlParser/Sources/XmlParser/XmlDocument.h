#pragma once

#include "XmlNode.h"

namespace VT
{
	class XmlDocument final
	{
	private:
		pugi::xml_document m_xmlDocument;

	public:
		XmlDocument() = default;
		XmlDocument(pugi::xml_document&& pugiXmlDoc) :
			m_xmlDocument(std::move(pugiXmlDoc)) {}
		
		bool loadFromBuffer(const void* data, size_t dataSize);
		bool loadFromString(const char* str);

		XmlNode getRootNode() const;
	};
}