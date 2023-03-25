#include "XmlNode.h"

bool VT::XmlNodeIterator::operator==(const XmlNodeIterator& iter) const
{
	return m_xmlNodeIterator == iter.m_xmlNodeIterator;
}

bool VT::XmlNodeIterator::operator!=(const XmlNodeIterator& iter) const
{
	return m_xmlNodeIterator != iter.m_xmlNodeIterator;
}

VT::XmlNode VT::XmlNodeIterator::operator*() const
{
	return m_xmlNodeIterator.operator*();
}

VT::XmlNodeIterator& VT::XmlNodeIterator::operator++()
{
	m_xmlNodeIterator = m_xmlNodeIterator.operator++();
	return *this;
}

VT::XmlNodeIterator VT::XmlNodeIterator::operator++(int)
{
	const XmlNodeIterator tmp = *this;
	m_xmlNodeIterator = m_xmlNodeIterator.operator++();
	return tmp;
}

VT::XmlNodeIterator& VT::XmlNodeIterator::operator--()
{
	m_xmlNodeIterator = m_xmlNodeIterator.operator--();
	return *this;
}

VT::XmlNodeIterator VT::XmlNodeIterator::operator--(int)
{
	const XmlNodeIterator tmp = *this;
	m_xmlNodeIterator = m_xmlNodeIterator.operator--();
	return tmp;
}

bool VT::XmlNamedNodeIterator::operator==(const XmlNamedNodeIterator& iter) const
{
	return m_xmlNodeIterator == iter.m_xmlNodeIterator;
}

bool VT::XmlNamedNodeIterator::operator!=(const XmlNamedNodeIterator& iter) const
{
	return m_xmlNodeIterator != iter.m_xmlNodeIterator;
}

VT::XmlNode VT::XmlNamedNodeIterator::operator*() const
{
	return m_xmlNodeIterator.operator*();
}

VT::XmlNamedNodeIterator& VT::XmlNamedNodeIterator::operator++()
{
	m_xmlNodeIterator = m_xmlNodeIterator.operator++();
	return *this;
}

VT::XmlNamedNodeIterator VT::XmlNamedNodeIterator::operator++(int)
{
	const XmlNamedNodeIterator tmp = *this;
	m_xmlNodeIterator = m_xmlNodeIterator.operator++();
	return tmp;
}

VT::XmlNamedNodeIterator& VT::XmlNamedNodeIterator::operator--()
{
	m_xmlNodeIterator = m_xmlNodeIterator.operator--();
	return *this;
}

VT::XmlNamedNodeIterator VT::XmlNamedNodeIterator::operator--(int)
{
	const XmlNamedNodeIterator tmp = *this;
	m_xmlNodeIterator = m_xmlNodeIterator.operator--();
	return tmp;
}

VT::XmlNode VT::XmlNode::getChild(const char* name) const
{
	return m_xmlNode.child(name);
}

VT::XmlNodeRange<VT::XmlNodeIterator> VT::XmlNode::getAllChildren() const
{
	pugi::xml_object_range<pugi::xml_node_iterator> xmlRange = m_xmlNode.children();
	return XmlNodeRange{ XmlNodeIterator(xmlRange.begin()), XmlNodeIterator(xmlRange.end()) };
}

VT::XmlNodeRange<VT::XmlNamedNodeIterator> VT::XmlNode::getChildren(const char* name) const
{
	pugi::xml_object_range<pugi::xml_named_node_iterator> xmlRange = m_xmlNode.children(name);
	return XmlNodeRange{ XmlNamedNodeIterator(xmlRange.begin()), XmlNamedNodeIterator(xmlRange.end()) };
}

VT::XmlNode VT::XmlNode::getFirstChild() const
{
	return m_xmlNode.first_child();
}

VT::XmlNode VT::XmlNode::getLastChild() const
{
	return m_xmlNode.last_child();
}

VT::XmlNode VT::XmlNode::getNextSibling() const
{
	return m_xmlNode.next_sibling();
}

VT::XmlNode VT::XmlNode::getPrevSibling() const
{
	return m_xmlNode.previous_sibling();
}

const char* VT::XmlNode::getName() const
{
	return m_xmlNode.name();
}

bool VT::XmlNode::setName(const char* name)
{
	return m_xmlNode.set_name(name);
}

VT::XmlValue VT::XmlNode::getValue() const
{
	return m_xmlNode.text();
}
