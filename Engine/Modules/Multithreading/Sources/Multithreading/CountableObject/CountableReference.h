#pragma once

#include "CountableObjectBase.h"

#include <type_traits>

namespace VT
{
	template<typename T>
	class CountableObjetcReference;

	template <typename T>
	class CountableConstObjetcReference
	{
		static_assert(std::is_base_of<CountableObjectBase, T>::value);

	public:
		using ObjectType = T;

	protected:
		const T* m_object = nullptr;

		void releaseReference();
		void assign(const T* object);

	public:
		CountableConstObjetcReference() = default;
		CountableConstObjetcReference(nullptr_t) : m_object(nullptr) {}
		CountableConstObjetcReference(T* object) : CountableConstObjetcReference(const_cast<const T*>(object)) {}
		CountableConstObjetcReference(const T* object);
		CountableConstObjetcReference(const CountableConstObjetcReference<T>& reference);
		CountableConstObjetcReference(CountableConstObjetcReference<T>&& reference);
		CountableConstObjetcReference(const CountableObjetcReference<T>& reference);
		virtual ~CountableConstObjetcReference() { releaseReference(); }

		inline CountableConstObjetcReference& operator=(T* object) { return operator=(const_cast<const T*>(object)); }
		inline CountableConstObjetcReference& operator=(const T* object);
		inline CountableConstObjetcReference& operator=(const CountableConstObjetcReference<T>& reference);
		inline CountableConstObjetcReference& operator=(CountableConstObjetcReference<T>&& reference);
		inline bool operator==(const CountableConstObjetcReference<T>& reference) const;
		inline bool operator!=(const CountableConstObjetcReference<T>& reference) const;
		inline const T* operator->() const { return m_object; }
		inline const T& operator*() const { return *m_object; }

		inline operator bool() const { return m_object; }
		inline bool isNull() const { return m_object == nullptr; }

		inline const T* getObject() const { return m_object; }
		inline const T& getObjectRef() const { return *m_object; }

		template <typename CastType>
		inline const CastType* getObjectCast() const { return reinterpret_cast<CastType*>(m_object); }

		void release() { releaseReference(); }
	};

	template <typename T>
	class CountableObjetcReference : public CountableConstObjetcReference<T>
	{
	public:
		CountableObjetcReference() = default;
		CountableObjetcReference(T* object)
			: CountableConstObjetcReference<T>(object) {}
		CountableObjetcReference(const CountableObjetcReference<T>& reference)
			: CountableConstObjetcReference<T>(reference) {}
		CountableObjetcReference(CountableObjetcReference<T>&& reference)
			: CountableConstObjetcReference<T>(std::move(reference)) {}

		inline CountableObjetcReference& operator=(T* object);
		inline CountableObjetcReference& operator=(const CountableObjetcReference<T>& reference);
		inline CountableObjetcReference& operator=(CountableObjetcReference<T>&& reference);
		inline T* operator->() const { return getObject(); }

		inline T& operator*() const { return getObjectRef(); }

		inline T* getObject() const { return const_cast<T*>(m_object); }
		inline T& getObjectRef() const { return *getObject(); }

		template <typename CastType>
		inline CastType* getObjectCast() const { return reinterpret_cast<CastType*>(getObject()); }
	};
}

#define COUNTABLE_REFERENCES_DECLARATION_BY_NAME(TYPE, NAME)				\
	using NAME##Reference = VT::CountableObjetcReference<TYPE>;				\
	using NAME##ConstReference = VT::CountableConstObjetcReference<TYPE>;

#define COUNTABLE_REFERENCES_DECLARATION(TYPE) COUNTABLE_REFERENCES_DECLARATION_BY_NAME(TYPE, TYPE)

#include "CountableReference.inl"