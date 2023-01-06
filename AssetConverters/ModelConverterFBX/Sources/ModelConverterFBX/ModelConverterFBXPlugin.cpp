#include "ModelConverterFBXPlugin.h"

VT_PLUGIN(VT_MODEL_RC::ModelConverterFBXPlugin)

VT::IFileResourceConverter* VT_MODEL_RC::ModelConverterFBXPlugin::createConverter()
{
	return new ModelConverterFBX();
}