#include "XmlDocument.h"

bool VT::XmlDocument::loadFromBuffer(const void* data, size_t dataSize)
{
	return m_xmlDocument.load_buffer(data, dataSize);
}

bool VT::XmlDocument::loadFromString(const char* str)
{
	return m_xmlDocument.load_string(str);
}

VT::XmlNode VT::XmlDocument::getRootNode() const
{
	return m_xmlDocument;
}