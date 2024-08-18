#include "Image.h"
#include <fstream>
#include <Windows.h>

#include <libpng16/png.h>

NS_OCF_BEGIN

namespace {
    struct ImageSource {
        const unsigned char* pData;
        size_t size;
        int offset;
    };

    void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        ImageSource* pSource = (ImageSource*)png_get_io_ptr(png_ptr);

        if ((pSource->offset + length) <= pSource->size) {
            memcpy(data, pSource->pData + pSource->offset, length);
            pSource->offset += static_cast<int>(length);
        } else {
            png_error(png_ptr, "pngReadCallback failed");
        }
    }
}

Image::Image()
    : m_pData(nullptr)
    , m_dataSize(0)
    , m_width(0)
    , m_height(0)
    , m_fileType(Format::UNKNOWN)
    , m_pixcelFormat(PixelFormat::NONE)
{
}

Image::~Image()
{
    if (m_pData) {
        delete[] m_pData;
        m_pData = nullptr;
    }

    m_width = 0;
    m_height = 0;
}

bool Image::initWithRowData(const uint8_t* pData, size_t dataSize, int width, int height)
{
    bool result = false;
    do {
        OCF_BREAK_IF(width == 0 || height == 0);

        m_width = width;
        m_height = height;
        m_pixcelFormat = PixelFormat::RGBA;

        m_dataSize = dataSize;
        m_pData = new uint8_t[dataSize];
        OCF_BREAK_IF(pData == nullptr);
        memcpy(m_pData, pData, m_dataSize);

        result = true;
    } while (0);

    return result;
}

bool Image::loadImageFile(const std::string& path)
{
    m_filePath = path;

    std::ifstream ifs;
    ifs.open(path, std::ios_base::binary);

    // ファイルオープン失敗
    if (!ifs) {
        return false;
    }

    // ファイルサイズ取得
    ifs.seekg(0, std::ios_base::end);
    size_t size = ifs.tellg();

    // ファイル位置を先頭に戻す
    ifs.seekg(0, std::ios_base::beg);

    // データ読み込み
    unsigned char* pData = new unsigned char[size];
    ifs.read(reinterpret_cast<char*>(pData), size);

    ifs.close();

    // データから画像データを読み込む
    loadImageData(pData, size);

    // メモリ解放
    delete[] pData;

    return true;
}

bool Image::loadImageData(const unsigned char* pData, size_t dataSize)
{
    bool result = false;

    m_fileType = detectFormat(pData, dataSize);

    switch (m_fileType) {
    case Format::BMP:
        result = initWithBmpData(pData, dataSize);
        break;
    case Format::PNG:
        result = initWidhtPngData(pData, dataSize);
    default:
        break;
    }

    return result;
}


Image::Format Image::detectFormat(const unsigned char* pData, size_t dataSize)
{
    if (isBmp(pData, dataSize)) {
        return Format::BMP;
    } else if (isPng(pData, dataSize)) {
        return Format::PNG;
    } else {
        return Format::UNKNOWN;
    }
}

bool Image::initWithBmpData(const unsigned char* pData, size_t dataSize)
{
    return false;
}

bool Image::initWidhtPngData(const unsigned char* pData, size_t dataSize)
{
    bool result = false;
    static constexpr int PNG_SIG_SIZE = 8;

    png_byte header[PNG_SIG_SIZE] = { 0 };
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    do {
        if (dataSize < 8) break;

        // PNGデータか判定
        memcpy(header, pData, PNG_SIG_SIZE);
        if (png_sig_cmp(header, 0, PNG_SIG_SIZE) != 0) break;

        // png_ptrを初期化
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (png_ptr == nullptr) break;

        // info_ptrを初期化
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == nullptr) break;

        setjmp(png_jmpbuf(png_ptr));

        // コールバック関数を設定
        ImageSource imageSource = {};
        imageSource.pData = pData;
        imageSource.size = dataSize;
        imageSource.offset = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

        // PNGのファイル情報を読み込む
        png_read_info(png_ptr, info_ptr);

        m_width = png_get_image_width(png_ptr, info_ptr);
        m_height = png_get_image_height(png_ptr, info_ptr);
        const png_byte bitDepth = png_get_bit_depth(png_ptr, info_ptr);
        const png_uint_32 colorType = png_get_color_type(png_ptr, info_ptr);

        // ピクセルフォーマットを設定
        switch (colorType) {
        case PNG_COLOR_TYPE_GRAY:
            m_pixcelFormat = PixelFormat::GRAY;
            break;
        case PNG_COLOR_TYPE_RGB:
            m_pixcelFormat = PixelFormat::RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            m_pixcelFormat = PixelFormat::RGBA;
            break;
        default:
            break;
        }

        // PNGデータよよ読み込み
        png_size_t rowbytes;
        png_bytep* row_pointers = new png_bytep[m_height];

        rowbytes = png_get_rowbytes(png_ptr, info_ptr);

        const size_t dataLen = rowbytes * m_height;
        m_pData = new unsigned char[dataLen * sizeof(unsigned char)];

        for (int i = 0; i < m_height; i++) {
            row_pointers[i] = m_pData + i * rowbytes;
        }
        png_read_image(png_ptr, row_pointers);

        png_read_end(png_ptr, nullptr);

        if (row_pointers != nullptr) {
            delete[] row_pointers;
        }

        result = true;
    } while (false);

    if (png_ptr != nullptr) {
        png_destroy_read_struct(&png_ptr, (info_ptr != nullptr) ? &info_ptr : nullptr, nullptr);
    }

    return result;
}

bool Image::isBmp(const unsigned char* pData, size_t dataSize)
{
    if (dataSize <= 4)
        return false;

    static unsigned char BMP_FILETYPE[] = { 0x42, 0x4D };

    return memcmp(pData, BMP_FILETYPE, 2) == 0;
}

bool Image::isPng(const unsigned char* pData, size_t dataSize)
{
    if (dataSize <= 8)
        return false;

    static const unsigned char PNG_SIGNATURE[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

    return memcmp(PNG_SIGNATURE, pData, sizeof(PNG_SIGNATURE)) == 0;
}

NS_OCF_END
