#pragma once

namespace VT
{
#pragma region CONST_REFERENCE_IMPL
	template<typename T>
	void CountableConstObjetcReference<T>::releaseReference()
	{
		if (m_object)
		{
			const_cast<T*>(m_object)->releaseReferenceObject();
			m_object = nullptr;
		}
	}

	template <typename T>
	void CountableConstObjetcReference<T>::assign(const T* object)
	{
		releaseReference();

		if (object)
		{
			m_object = reinterpret_cast<T*>(const_cast<T*>(object)->getReferenceObject());
		}
	}

	template<typename T>
	CountableConstObjetcReference<T>::CountableConstObjetcReference(const T* objectRef)
	{
		if (objectRef)
		{
			m_object = reinterpret_cast<T*>(const_cast<T*>(objectRef)->getReferenceObject());
		}
	}

	template<typename T>
	CountableConstObjetcReference<T>::CountableConstObjetcReference(const CountableConstObjetcReference<T>& reference)
	{
		if (reference.m_object)
		{
			m_object = reinterpret_cast<T*>(const_cast<T*>(reference.m_object)->getReferenceObject());
		}
	}

	template<typename T>
	CountableConstObjetcReference<T>::CountableConstObjetcReference(CountableConstObjetcReference<T>&& reference)
	{
		if (reference.m_object)
		{
			m_object = reference.m_object;
			reference.m_object = nullptr;
		}
	}

	template<typename T>
	CountableConstObjetcReference<T>::CountableConstObjetcReference(const CountableObjetcReference<T>& reference)
	{
		if (reference.m_object)
		{
			m_object = reinterpret_cast<T*>(const_cast<T*>(reference.m_object)->getReferenceObject());
		}
	}

	template<typename T>
	CountableConstObjetcReference<T>& CountableConstObjetcReference<T>::operator=(const T* objectRef)
	{
		assign(objectRef);
		return *this;
	}

	template<typename T>
	CountableConstObjetcReference<T>& CountableConstObjetcReference<T>::operator=(const CountableConstObjetcReference<T>& reference)
	{
		assign(reference.m_object);
		return *this;
	}

	template<typename T>
	CountableConstObjetcReference<T>& CountableConstObjetcReference<T>::operator=(CountableConstObjetcReference<T>&& reference)
	{
		assign(reference.m_object);
		return *this;
	}

	template<typename T>
	bool CountableConstObjetcReference<T>::operator==(const CountableConstObjetcReference<T>& reference) const
	{
		return m_object == reference.m_object;
	}

	template<typename T>
	bool CountableConstObjetcReference<T>::operator!=(const CountableConstObjetcReference<T>& reference) const
	{
		return m_object != reference.m_object;
	}

	template <typename T1, typename T2>
	bool operator<(CountableConstObjetcReference<T1> ref1, CountableConstObjetcReference<T2> ref2)
	{
		return ref1->getObject() < ref2->getObject();
	}

	template <typename T1, typename T2>
	bool operator>(CountableConstObjetcReference<T1> ref1, CountableConstObjetcReference<T2> ref2)
	{
		return ref1->getObject() > ref2->getObject();
	}
#pragma endregion

#pragma region REFERENCE_IMPL
	template <typename T>
	CountableObjetcReference<T>& CountableObjetcReference<T>::operator=(T* object)
	{
		CountableConstObjetcReference<T>::assign(const_cast<const T*>(object));
		return *this;
	}

	template <typename T>
	CountableObjetcReference<T>& CountableObjetcReference<T>::operator=(const CountableObjetcReference<T>& reference)
	{
		CountableConstObjetcReference<T>::assign(const_cast<const T*>(reference.m_object));
		return *this;
	}

	template <typename T>
	CountableObjetcReference<T>& CountableObjetcReference<T>::operator=(CountableObjetcReference<T>&& reference)
	{
		CountableConstObjetcReference<T>::assign(const_cast<const T*>(reference.m_object));
		return *this;
	}
#pragma endregion
}