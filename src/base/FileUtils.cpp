#include "FileUtils.h"
#include <filesystem>

OCF_BEGIN

FileUtils* FileUtils::s_sharedFileUtils = nullptr;

FileUtils* FileUtils::getInstance()
{
	if (s_sharedFileUtils == nullptr) {
		s_sharedFileUtils = new FileUtils;
	}

	return s_sharedFileUtils;
}

void FileUtils::destroyInstance()
{
	delete s_sharedFileUtils;
	s_sharedFileUtils = nullptr;
}

FileUtils::~FileUtils()
{
}

std::string FileUtils::getFullPath(const std::string& filename) const
{
	std::filesystem::path fullPaht = std::filesystem::absolute(filename);

	return fullPaht.string();
}

std::string FileUtils::getParentFullPath(const std::string& filename) const
{
	std::filesystem::path parentPath = std::filesystem::path(filename).parent_path();

	return std::filesystem::absolute(parentPath).string();
}

std::string FileUtils::getAssetsPath() const
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	std::filesystem::path assetsPath = currentPath.parent_path();
	assetsPath.append("assets");

	return assetsPath.string();
}

FileUtils::FileUtils()
{
}

OCF_END
