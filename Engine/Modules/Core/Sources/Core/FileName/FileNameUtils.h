#pragma once

#include "FileName.h"

namespace VT::FileNameUtils
{
	bool exist(const FileName& name);

	void getFileName(const FileName& path, FileName& resultName);
	FileName getFileName(const FileName& path);

	void getFileDirPath(const FileName& path, FileName& resultPath);
	FileName getFileDirPath(const FileName& path);

	void removeExtension(FileName& name);
	FileName removeExtension(const FileName& name);

	void createDir(const FileName& path);
}