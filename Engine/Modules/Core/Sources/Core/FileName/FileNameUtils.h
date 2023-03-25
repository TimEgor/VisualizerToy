#pragma once

#include "FileName.h"

namespace VT::FileNameUtils
{
	void getFileName(const FileName& path, FileName& resultName);
	FileName getFileName(const FileName& path);

	void getFileDirPath(const FileName& path, FileName& resultPath);
	FileName getFileDirPath(const FileName& path);

	void getFileExtension(const FileName& path, FileName& resultExtension);
	FileName getFileExtension(const FileName& path);

	void removeExtension(FileName& name);
	FileName removeExtension(const FileName& name);
}