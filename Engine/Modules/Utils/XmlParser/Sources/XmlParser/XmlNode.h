#pragma once

#include "XmlValue.h"

namespace VT
{
	class XmlNode;

	class XmlNodeIterator final
	{
	private:
		pugi::xml_node_iterator m_xmlNodeIterator;

	public:
		XmlNodeIterator(pugi::xml_node_iterator iter)
			: m_xmlNodeIterator(iter) {}

		bool operator==(const XmlNodeIterator& iter) const;
		bool operator!=(const XmlNodeIterator& iter) const;

		XmlNode operator*() const;

		XmlNodeIterator& operator++();
		XmlNodeIterator operator++(int);

		XmlNodeIterator& operator--();
		XmlNodeIterator operator--(int);
	};

	class XmlNamedNodeIterator final
	{
	private:
		pugi::xml_named_node_iterator m_xmlNodeIterator;

	public:
		XmlNamedNodeIterator(pugi::xml_named_node_iterator iter)
			: m_xmlNodeIterator(iter) {}

		bool operator==(const XmlNamedNodeIterator& iter) const;
		bool operator!=(const XmlNamedNodeIterator& iter) const;

		XmlNode operator*() const;

		XmlNamedNodeIterator& operator++();
		XmlNamedNodeIterator operator++(int);

		XmlNamedNodeIterator& operator--();
		XmlNamedNodeIterator operator--(int);
	};

	template<typename Iter>
	class XmlNodeRange final
	{
	private:
		Iter m_begin;
		Iter m_end;

	public:
		XmlNodeRange(Iter begin, Iter end)
		: m_begin(begin), m_end(end) {}

		Iter begin() const { return m_begin; }
		Iter end() const { return m_end; }

		bool empty() const { return m_begin == m_end; }
	};

	class XmlNode final
	{
	private:
		pugi::xml_node m_xmlNode;

	public:
		XmlNode() = default;
		XmlNode(pugi::xml_node pugiXmlNode)
			: m_xmlNode(pugiXmlNode) {}

		operator bool() const { return m_xmlNode; }

		XmlNode getChild(const char* name) const;

		XmlNodeRange<XmlNodeIterator> getAllChildren() const;
		XmlNodeRange<XmlNamedNodeIterator> getChildren(const char* name) const;

		XmlNode getFirstChild() const;
		XmlNode getLastChild() const;

		XmlNode getNextSibling() const;
		XmlNode getPrevSibling() const;

		const char* getName() const;
		bool setName(const char* name);
		
		XmlValue getValue() const;
	};
}
