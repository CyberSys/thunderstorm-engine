#include "fs.h"
#include <ShlObj.h>

namespace fs
{
	path GetStashPath()
	{
		static path path;
		if (path.empty())
		{
			wchar_t* str = nullptr;
			SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_SIMPLE_IDLIST, nullptr, &str);
			path = str;
			path = path / L"My Games" / L"Thunderstorm";
			CoTaskMemFree(str);
		}
		return path;
	}

	path GetLogsPath()
	{
		return GetStashPath() / "Logs";
	}

	path GetSaveDataPath()
	{
		return GetStashPath() / "SaveData";
	}
}
