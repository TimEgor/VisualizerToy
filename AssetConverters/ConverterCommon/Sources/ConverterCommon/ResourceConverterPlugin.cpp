#include "ResourceConverterPlugin.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "ResourceSystem/IResourceSystem.h"

void VT_RC::ResourceConverterPluginBase::onLoaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "ResourceConverterBase::onLoaded() : Engine environment is invalid.");
		return;
	}

	VT::IResourceSystem* resSystem = environment->m_resourceSystem;
	if (!resSystem)
	{
		assert(false && "ResourceConverterBase::onLoaded() : Engine environment resourceSystem hasn't been initialized yet.");
		return;
	}

	VT::IFileResourceConverter* resConverter = createConverter();
	if (!resConverter)
	{
		assert(false && "ResourceConverterBase::onLoaded() : Converter hasn't been created.");
		return;
	}

	resSystem->addResourceConverter(resConverter);
}

void VT_RC::ResourceConverterPluginBase::onUnloaded()
{
	VT::EngineEnvironment* environment = VT::EngineInstance::getInstance()->getEnvironment();
	if (!environment)
	{
		assert(false && "ResourceConverterBase::onUnloaded() : Engine environment is invalid.");
		return;
	}

	VT::IResourceSystem* resSystem = environment->m_resourceSystem;
	if (!resSystem)
	{
		assert(false && "ResourceConverterBase::onUnloaded() : Engine environment resourceSystem is invalid.");
		return;
	}

	resSystem->removeResourceConverter(getConverterType());
}
