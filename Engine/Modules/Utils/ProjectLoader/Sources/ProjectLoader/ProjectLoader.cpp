#include "ProjectLoader.h"

#include "Core/FileName/FileNameUtils.h"
#include "Core/Output.h"
#include "Core/String/Format.h"
#include "Core/String/StringSubstitution.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"
#include "FileSystem/IFileSystem.h"

#include "ResourceSystem/IResourceSystem.h"
#include "GameSystem/IGameSystem.h"

#include "XmlParser/XmlDocument.h"
#include "XmlParser/ValueParserBase.h"

bool VT::ProjectLoader::load(const FileName& path)
{
	if (path)
	{
		const FileName extension = FileNameUtils::getFileExtension(path);

		if (extension == VT_PROJECT_FILE_FULL_EXT_STR)
		{
			return loadProjectInternal(path);
		}
		if (extension == VT_GAME_MODULE_FILE_FULL_EXT_STR)
		{
			return loadModuleInternal(path);
		}
	}

	return false;
}

bool VT::ProjectLoader::loadProject(const FileName& projectPath)
{
	VT_CHECK_RETURN_FALSE(!projectPath || FileNameUtils::getFileExtension(projectPath) != VT_PROJECT_FILE_FULL_EXT_STR);
	return loadProjectInternal(projectPath);
}

bool VT::ProjectLoader::loadModule(const FileName& modulePath)
{
	VT_CHECK_RETURN_FALSE(!modulePath || FileNameUtils::getFileExtension(modulePath) != VT_GAME_MODULE_FILE_FULL_EXT_STR);
	return loadModuleInternal(modulePath);
}

bool VT::ProjectLoader::loadProjectInternal(const FileName& projectPath)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IFileSystem* fileSystem = environment->m_resourceSystem->getBaseFileSystem();

	size_t projetDataSize = 0;
	void* projectData = fileSystem->readResourceData(projectPath, projetDataSize);

	VT_CHECK_RETURN_FALSE(projectData);

	XmlDocument projectDoc;
	if (!projectDoc.loadFromBuffer(projectData, projetDataSize))
	{
		VT_SAFE_DESTROY_ARRAY(projectData);
		return false;
	}

	VT_SAFE_DESTROY_ARRAY(projectData);

	XmlNode projectDocRootNode = projectDoc.getRootNode();
	XmlNode projectRootNode = projectDocRootNode.getChild("Project");
	VT_CHECK_RETURN_FALSE(projectRootNode);
	XmlNode modulesNode = projectRootNode.getChild("Modules");
	VT_CHECK_RETURN_FALSE(modulesNode);

	IGameSystem* gameSystem = environment->m_gameSystem;

	auto moduleNodesRange = modulesNode.getChildren("Module");
	VT_CHECK_RETURN_FALSE(!moduleNodesRange.empty());

	StringSubstitutionProcessor::ReplacementMap replacementMap =
	{
		{ crc32("VT_PLATFORM"), VT_PLATFORM_NAME },
		{ crc32("VT_CONFIG"), VT_CONFIG_NAME },
		{ crc32("VT_DYNAMIC_LIB_EXT"), VT_DYNAMIC_LIB_FULL_EXT_NAME },

		{ crc32("VT_PROJECT_PATH"), FileNameUtils::getFileDirPath(projectPath).c_str() }
	};

	for (const auto& moduleNode : moduleNodesRange)
	{
		FileName modulePath = stringSubstitute(moduleNode.getValue().getRaw(), replacementMap);
		if (!modulePath)
		{
			return false;
		}

		if (!gameSystem->loadGameModule(modulePath))
		{
			VT_ENV_CONSOLE_OUTPUT(stringFormat("Project loading: module \'%s\' hasn't been loaded.", modulePath.c_str()).c_str());
			return false;
		}
	}

	return true;
}

bool VT::ProjectLoader::loadModuleInternal(const FileName& modulePath)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	return environment->m_gameSystem->loadGameModule(modulePath);
}