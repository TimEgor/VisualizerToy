#include "ManagedResourceSystemConverterArgs.h"

#include "DummyResourceSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

void VT_DUMMY_RS::ManagedResourceSystemConverterArgs::selfDestroy()
{
	reinterpret_cast<DummyResourceSystem*>(VT::EngineInstance::getInstance()->getEnvironment()->m_resourceSystem)->releaseResourceSystemConverterArgs(this);
}

VT::IResourceConverterArgs* VT_DUMMY_RS::ManagedResourceSystemConverterArgs::getArgs() const
{
	const uint8_t* handlerPtr = reinterpret_cast<const uint8_t*>(this);
	const VT::IResourceConverterArgs* argsPtr = reinterpret_cast<const VT::IResourceConverterArgs*>(handlerPtr + sizeof(ManagedResourceSystemConverterArgs));

	return const_cast<VT::IResourceConverterArgs*>(argsPtr);
}

bool VT_DUMMY_RS::ManagedResourceSystemConverterArgsContainer::init(size_t elementSize)
{
	return m_pool.init(elementSize + sizeof(ManagedResourceSystemConverterArgs), 128, 0);
}

void VT_DUMMY_RS::ManagedResourceSystemConverterArgsContainer::release()
{
	m_pool.release();
}

VT_DUMMY_RS::ManagedResourceSystemConverterArgs* VT_DUMMY_RS::ManagedResourceSystemConverterArgsContainer::allocateArgs()
{
	PoolElementInfo newArgsHandlerInfo = m_pool.addElementRaw();
	return new (newArgsHandlerInfo.m_elementPtr) ManagedResourceSystemConverterArgs(newArgsHandlerInfo.m_elementIndex);
}

void VT_DUMMY_RS::ManagedResourceSystemConverterArgsContainer::deallocateArgs(ManagedResourceSystemConverterArgs* args)
{
	assert(args);
	m_pool.removeElement(args->getID());
}

void VT_DUMMY_RS::ConverterArgsColllection::release()
{
	VT::UniqueLockGuard m_locker(m_mutex);
	m_containers = ArgsContainers();
}

void VT_DUMMY_RS::ConverterArgsColllection::addType(VT::ResourceConverterArgsType type,
	VT::IResourceSystemConverterArgsDestructor* destructor, size_t elementSize)
{
	VT::UniqueLockGuard m_locker(m_mutex);
	assert(destructor);

	auto& findTypeIter = m_containers.find(type);
	if (findTypeIter == m_containers.end())
	{
		ConverterArgsTypeInfo& info = m_containers[type];
		m_locker.unlock();

		info.m_container.init(elementSize);
		info.m_destructor = destructor;
	}
}

VT_DUMMY_RS::ManagedResourceSystemConverterArgs* VT_DUMMY_RS::ConverterArgsColllection::allocateNewConverterArgsHandler(VT::ResourceConverterArgsType type)
{
	VT::SharedLockGuard m_locker(m_mutex);

	auto& findTypeIter = m_containers.find(type);
	if (findTypeIter != m_containers.end())
	{
		m_locker.unlock();
		return findTypeIter->second.m_container.allocateArgs();
	}

	return nullptr;
}

void VT_DUMMY_RS::ConverterArgsColllection::deallocateNewConverterArgs(ManagedResourceSystemConverterArgs* argsHandler)
{
	assert(argsHandler);

	VT::IResourceConverterArgs* args = argsHandler->getArgs();

	assert(args);

	VT::ResourceConverterArgsType type = args->getType();

	VT::SharedLockGuard m_locker(m_mutex);

	auto& findTypeIter = m_containers.find(type);
	if (findTypeIter != m_containers.end())
	{
		m_locker.unlock();
		ConverterArgsTypeInfo& info = findTypeIter->second;
		info.m_destructor->destroy(args);
		info.m_container.deallocateArgs(argsHandler);
	}
}
