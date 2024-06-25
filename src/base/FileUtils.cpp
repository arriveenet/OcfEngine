#include "FileUtils.h"
#include <filesystem>

NS_OCF_BEGIN

namespace fs = std::filesystem;

constexpr const char* OCF_ASSETS_DIR = "assets";

FileUtils* FileUtils::s_sharedFileUtils = nullptr;
std::string FileUtils::s_exeDirectory;

namespace {
	std::string checkExePath()
	{
		return fs::current_path().generic_string();
	}

	std::string checkAssetsPath()
	{
		std::string result;

		fs::path currentPath = fs::current_path();
		fs::path rootPath = currentPath.root_path();

		while (currentPath != rootPath) {
			fs::path assetsPath = currentPath;
			assetsPath.append(OCF_ASSETS_DIR);
			if (fs::exists(assetsPath)) {
				result = assetsPath.string();
				break;
			}

			currentPath = currentPath.parent_path();
		}

		return result;
	}
}

FileUtils* FileUtils::getInstance()
{
	if (s_sharedFileUtils == nullptr) {
		s_sharedFileUtils = new FileUtils;
		if (!s_sharedFileUtils->init()) {
			delete s_sharedFileUtils;
			s_sharedFileUtils = nullptr;
		}
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

void FileUtils::addSearchPath(const std::string& path, bool front)
{
}

const std::vector<std::string>& FileUtils::getSearchPath() const
{
	return m_searchPathArray;
}

std::string FileUtils::getAssetsPath() const
{
	return m_defaultAssetsRootPath;
}

std::string FileUtils::fullPathForFilename(const std::string& filename) const
{
	if (filename.empty()) {
		return "";
	}

	const fs::path p(filename);
	if (p.is_absolute()) {
		return filename;
	}

	std::string fullpath;
	for (const auto& searchIter : m_searchPathArray) {
		fullpath = this->getPathForFilename(filename, searchIter);

		if (!fullpath.empty()) {
			return fullpath;
		}
	}

	return std::string();
}

FileUtils::FileUtils()
{
}

bool FileUtils::init()
{
	if (s_exeDirectory.empty()) {
		s_exeDirectory = checkExePath();
	}

	m_searchPathArray.emplace_back(s_exeDirectory);

	m_defaultAssetsRootPath = checkAssetsPath();
	if (!m_defaultAssetsRootPath.empty()) {
		m_searchPathArray.emplace_back(m_defaultAssetsRootPath);
		m_searchPathArray.emplace_back(m_defaultAssetsRootPath + "\\fonts");
		m_searchPathArray.emplace_back(m_defaultAssetsRootPath + "\\shaders");
		m_searchPathArray.emplace_back(m_defaultAssetsRootPath + "\\textures");
	}

	return true;
}

std::string FileUtils::getPathForFilename(const std::string& filename, const std::string& searchPath) const
{
	fs::path path(searchPath);
	path.append(filename);

	std::string result = fs::exists(path) ? path.generic_string() : "";

	return result;
}

NS_OCF_END
