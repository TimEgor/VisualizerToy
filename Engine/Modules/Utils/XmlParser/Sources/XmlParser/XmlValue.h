#pragma once

#include "pugixml.hpp"

namespace VT
{
	class XmlValue final
	{
	private:
		pugi::xml_text m_xmlText;

	public:
		XmlValue() = default;
		XmlValue(pugi::xml_text pugiXmlText)
			: m_xmlText(pugiXmlText) {}

		template<typename T>
		T get() const;

		const char* getRaw() const { return m_xmlText.get(); }

		template<typename T>
		void set(T val);
	};

	template <typename T>
	T XmlValue::get() const
	{
		static_assert(false && "Inalid XML Value type");
		return T();
	}

	template <>
	inline bool XmlValue::get() const
	{
		return m_xmlText.as_bool();
	}

	template <>
	inline unsigned XmlValue::get() const
	{
		return m_xmlText.as_uint();
	}

	template <>
	inline int XmlValue::get() const
	{
		return m_xmlText.as_int();
	}

	template <>
	inline float XmlValue::get() const
	{
		return m_xmlText.as_float();
	}

	template <>
	inline double XmlValue::get() const
	{
		return m_xmlText.as_double();
	}

	template <>
	inline const char* XmlValue::get() const
	{
		return m_xmlText.as_string();
	}

	template <typename T>
	void XmlValue::set(T val)
	{
		m_xmlText.set(val);
	}
}
