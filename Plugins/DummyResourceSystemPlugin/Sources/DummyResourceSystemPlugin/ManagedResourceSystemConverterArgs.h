#pragma once

#include "ResourceSystem/ResourceSystemConverterArgs.h"
#include "ResourceConverter/IResourceConverterArgs.h"

#include "ObjectPool/ThreadIndexObjectPoolAllocator.h"

#include "unordered_map"

namespace VT_DUMMY_RS
{
	using ManagedResourceSystemConverterArgsID = uint32_t;
	constexpr ManagedResourceSystemConverterArgsID InvalidManagedResourceConverterArgsID = 0;

	class ManagedResourceSystemConverterArgs final : public VT::ResourceSystemConverterArgs
	{
	private:
		ManagedResourceSystemConverterArgsID m_id = InvalidManagedResourceConverterArgsID;

		virtual void selfDestroy() override;

	public:
		ManagedResourceSystemConverterArgs(ManagedResourceSystemConverterArgsID id)
			: m_id(id) {}

		virtual VT::IResourceConverterArgs* getArgs() const override;
		ManagedResourceSystemConverterArgsID getID() const { return m_id; }
	};

	class ManagedResourceSystemConverterArgsContainer final
	{
		using PoolType = VT::ThreadIndexObjectPoolAllocator<ManagedResourceSystemConverterArgsID>;
		using PoolElementInfo = typename PoolType::NewElementInfo;

	private:
		VT::ThreadIndexObjectPoolAllocator<uint32_t> m_pool;

	public:
		ManagedResourceSystemConverterArgsContainer() = default;

		bool init(size_t elementSize);
		void release();

		ManagedResourceSystemConverterArgs* allocateArgs();
		void deallocateArgs(ManagedResourceSystemConverterArgs* args);
	};

	class ConverterArgsColllection final
	{
		struct ConverterArgsTypeInfo final
		{
			ManagedResourceSystemConverterArgsContainer m_container;
			VT::IResourceSystemConverterArgsDestructor* m_destructor = nullptr;
		};

		using ArgsContainers = std::unordered_map<VT::ResourceConverterArgsType, ConverterArgsTypeInfo>;

	private:
		ArgsContainers m_containers;
		VT::SharedMutex m_mutex;

	public:
		ConverterArgsColllection() = default;
		~ConverterArgsColllection() { release(); }

		void release();

		void addType(VT::ResourceConverterArgsType type, VT::IResourceSystemConverterArgsDestructor* destructor, size_t elementSize);
		ManagedResourceSystemConverterArgs* allocateNewConverterArgsHandler(VT::ResourceConverterArgsType type);
		void deallocateNewConverterArgs(ManagedResourceSystemConverterArgs* argsHandler);
	};
}
