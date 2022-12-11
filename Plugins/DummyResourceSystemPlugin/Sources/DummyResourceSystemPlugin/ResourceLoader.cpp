#include "ResourceLoader.h"

#include "Core/UtilitiesMacros.h"
#include "Core/String/WildcardMatch/WildcardMatch.h"
#include "Core/HashFunctions/StdHashCombine.h"
#include "Core/String/Format.h"

#include "DummyResourceSystem.h"

#include "NativeFileSystem/NativeFileSystem.h"


constexpr char* ConvertedResourceDir = "CONVERTED/";

bool VT_DUMMY_RS::ResourceLoader::init(DummyResourceSystem* resourceSystem)
{
    m_resourceSystem = resourceSystem;
    m_fileSystem = new VT::NativeFileSystem();

    VT_CHECK_INITIALIZATION(m_resourceSystem, m_fileSystem);

    return true;
}

void VT_DUMMY_RS::ResourceLoader::release()
{
    m_resourceSystem = nullptr;
    VT_SAFE_DESTROY_WITH_RELEASING(m_fileSystem);
}

void VT_DUMMY_RS::ResourceLoader::readResourceData(const VT::FileName& name, VT::ResourceDataReference dataRef)
{
    assert(m_fileSystem);

    size_t dataSize = 0;
    void* data = m_fileSystem->readResourceBinaryData(name, dataSize);

    ManagedResourceData* managedResData = dataRef.getObjectCast<ManagedResourceData>();
    managedResData->setData(data, dataSize);
    managedResData->setState(VT::ResourceState::LOADED);
}

VT::IFileResourceConverter* VT_DUMMY_RS::ResourceLoader::chooseConvert(const VT::FileName& name)
{
    for (const auto& converter : m_converters)
    {
        if (VT::wildcardMatch(converter.second->getFileExtensionPattern(), name.c_str()))
        {
            return converter.second;
        }
    }

    return nullptr;
}

void VT_DUMMY_RS::ResourceLoader::loadResource(const VT::FileName& name, Request request)
{
    VT::IFileResourceConverter* converter = chooseConvert(name);
    if (converter)
    {
        size_t dataSize = 0;
        void* data = nullptr;

        const VT::FileName targetName = name + converter->getTargetFileExtension();

        VT::FileNameID id = 0;
        VT::hashCombine(id, targetName.hash());
        VT::hashCombine(id, request.m_args ? request.m_args->getArgs()->getHash() : 0);
        VT::hashCombine(id, converter->getType());
        VT::hashCombine(id, converter->getVersion());

        const VT::FileName convertedName = ConvertedResourceDir + VT::stringFormat("%x", id);

        if (!m_fileSystem->exist(convertedName))
        {
            converter->convert(*m_fileSystem, name, &data, dataSize,
                *m_fileSystem, convertedName, request.m_args->getArgs());

            ManagedResourceData* managedResData = request.m_data.getObjectCast<ManagedResourceData>();
            managedResData->setData(data, dataSize);
            managedResData->setState(VT::ResourceState::LOADED);
        }
        else
        {
            readResourceData(convertedName, request.m_data);
        }

        return;
    }

    readResourceData(name, request.m_data);
}

void VT_DUMMY_RS::ResourceLoader::loadResourceAsync(const VT::FileName& name, Request request)
{
    loadResource(name, request);

    if (m_resourceSystem)
    {
        m_resourceSystem->onResourceLoaded(name.hash(), request.m_data);
    }
}

void VT_DUMMY_RS::ResourceLoader::addResourceConverter(VT::IFileResourceConverter* converter)
{
    assert(converter);
    m_converters.insert(std::make_pair(converter->getType(), converter));
}

void VT_DUMMY_RS::ResourceLoader::removeResourceConverter(VT::ResourceConverterType converterType)
{
    auto findConverterIter = m_converters.find(converterType);
    if (findConverterIter != m_converters.end())
    {
        delete findConverterIter->second;
        m_converters.erase(findConverterIter);
    }
}
