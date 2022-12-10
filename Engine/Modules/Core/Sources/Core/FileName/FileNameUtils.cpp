#include "FileNameUtils.h"

#include <filesystem>

bool VT::FileNameUtils::exist(const FileName& name)
{
    return std::filesystem::exists(name.c_str());
}


void VT::FileNameUtils::getFileName(const FileName& path, FileName& resultName)
{
    std::filesystem::path tmpPath(path.c_str());
    resultName = tmpPath.filename().string();
}

VT::FileName VT::FileNameUtils::getFileName(const FileName & path)
{
    std::filesystem::path tmpPath(path.c_str());
    return tmpPath.filename().string();
}

void VT::FileNameUtils::getFileDirPath(const FileName& path, FileName& resultPath)
{
    std::filesystem::path tmpPath(path.c_str());
    resultPath = tmpPath.parent_path().string();
}

VT::FileName VT::FileNameUtils::getFileDirPath(const FileName & path)
{
    std::filesystem::path tmpPath(path.c_str());
    return tmpPath.parent_path().string();
}

void VT::FileNameUtils::removeExtension(FileName& name)
{
    std::filesystem::path tmpPath(name.c_str());
    name = tmpPath.replace_extension().string();
}

VT::FileName VT::FileNameUtils::removeExtension(const FileName& name)
{
    std::filesystem::path tmpPath(name.c_str());
    return tmpPath.replace_extension().string();
}

void VT::FileNameUtils::createDir(const FileName& path)
{
    std::filesystem::create_directories(path.c_str());
}