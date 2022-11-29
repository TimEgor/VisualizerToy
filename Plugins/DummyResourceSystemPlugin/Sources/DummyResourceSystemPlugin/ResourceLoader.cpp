#include "ResourceLoader.h"

#include "Core/UtilitiesMacros.h"
#include "DummyResourceSystem.h"

#include <filesystem>
#include <fstream>

bool VT_DUMMY_RS::ResourceLoader::init(DummyResourceSystem* resourceSystem)
{
    m_resourceSystem = resourceSystem;
    return true;
}

void VT_DUMMY_RS::ResourceLoader::release()
{
    m_resourceSystem = nullptr;
}

void VT_DUMMY_RS::ResourceLoader::loadResource(const VT::FileName& name, VT::ResourceDataReference dataRef)
{
    ManagedResourceData* data = dataRef.getObjectCast<ManagedResourceData>();
    assert(data);

    std::filesystem::path fsPath(name.c_str());
    if (!std::filesystem::exists(fsPath))
    {
        return;
    }

    size_t resourceSize = static_cast<size_t>(std::filesystem::file_size(fsPath));

    if (resourceSize == 0)
    {
        return;
    }

    void* resourceData = new uint8_t[resourceSize];

    std::fstream stream(name.c_str(), std::ios_base::in | std::ios_base::binary);
    if (!stream)
    {
        VT_SAFE_DESTROY_ARRAY(resourceData);
        return;
    }

    stream.read(reinterpret_cast<char*>(resourceData), resourceSize);
    stream.close();

    data->setData(resourceData, resourceSize);
    data->setState(VT::ResourceState::LOADED);
}

void VT_DUMMY_RS::ResourceLoader::loadResourceAsync(const VT::FileName& name, VT::ResourceDataReference dataRef)
{
    loadResource(name, dataRef);

    if (m_resourceSystem)
    {
        m_resourceSystem->onResourceLoaded(name.hash(), dataRef);
    }
}
