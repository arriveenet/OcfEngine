#pragma once
#include <string>
#include "GameObject.h"
#include "base/types.h"

OCF_BEGIN

class Image : public GameObject {
public:
	enum class Format {
		PNG,
		BMP,
		UNKNOWN
	};

	Image();
	virtual ~Image();

	bool loadImageFile(const std::string& path);
	bool loadImageData(const unsigned char* pData, size_t dataSize);

	unsigned char* getData() { return m_pData; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	Format getFileType()const { return m_fileType; }
	PixelFormat getPixelFormat()const { return m_pixcelFormat; }
	std::string getFilePath() const { m_filePath; }

protected:
	Format detectFormat(const unsigned char* pData, size_t dataSize);
	bool initWithBmpData(const unsigned char* pData, size_t dataSize);
	bool initWidhtPngData(const unsigned char* pData, size_t dataSize);

	bool isBmp(const unsigned char* pData, size_t dataSize);
	bool isPng(const unsigned char* pData, size_t dataSize);

protected:
	unsigned char* m_pData;
	int m_width;
	int m_height;
	Format m_fileType;
	PixelFormat m_pixcelFormat;
	std::string m_filePath;
};

OCF_END
