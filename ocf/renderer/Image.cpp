#include "Image.h"
#include <fstream>
#include <string.h>
#include <memory>

extern "C" {
#include <png.h>
#include <jpeglib.h>
} // extern "C"

#include "base/FileUtils.h"

namespace ocf {

#pragma pack(push, 1)
struct BitmapFileHeader {
    uint16_t bfType;       // 0x4D42
    uint32_t bfSize;       // ファイルサイズ
    uint16_t bfReserved1;  // 予約領域
    uint16_t bfReserved2;  // 予約領域
    uint32_t bfOffBits;    // データオフセット
};

struct BitmapInfoHeader {
    uint32_t biSize;            // ヘッダサイズ
    int32_t biWidth;            // 幅
    int32_t biHeight;           // 高さ
    uint16_t biPlanes;          // プレーン数
    uint16_t biBitCount;        // ビット数
    uint32_t biCompression;     // 圧縮形式
    uint32_t biSizeImage;       // 画像サイズ
    int32_t biXPelsPerMeter;    // 水平解像度
    int32_t biYPelsPerMeter;    // 垂直解像度
    uint32_t biClrUsed;         // 使用色数
    uint32_t biClrImportant;    // 重要色数
};
#pragma pack(pop)

namespace {
    struct ImageSource {
        const unsigned char* pData;
        size_t size;
        int offset;
    };

    void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        ImageSource* pSource = static_cast<ImageSource*>(png_get_io_ptr(png_ptr));

        if ((pSource->offset + length) <= pSource->size) {
            memcpy(data, pSource->pData + pSource->offset, length);
            pSource->offset += static_cast<int>(length);
        } else {
            png_error(png_ptr, "pngReadCallback failed");
        }
    }

    void pngWriteCallback(png_structp png_ptr, png_bytep data, size_t length)
    {
        if (png_ptr == nullptr) {
            return;
        }

        auto fileStream = static_cast<std::ofstream*>(png_get_io_ptr(png_ptr));

        fileStream->write(reinterpret_cast<char*>(data), length);

        if (fileStream->fail()) {
            png_error(png_ptr, "Write Error");
        }
    }
}

Image::Image()
    : m_pData(nullptr)
    , m_dataSize(0)
    , m_width(0)
    , m_height(0)
    , m_fileType(Format::UNKNOWN)
    , m_pixelFormat(PixelFormat::NONE)
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

bool Image::initWithRawData(const uint8_t* pData, size_t dataSize,
                            int width, int height, PixelFormat format)
{
    bool result = false;
    do {
        OCF_BREAK_IF(width == 0 || height == 0);

        m_width = width;
        m_height = height;
        m_pixelFormat = format;

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
        result = initWithPngData(pData, dataSize);
        break;
    case Format::JPEG:
        result = initWithJpegData(pData, dataSize);
        break;
    default:
        break;
    }

    return result;
}

bool Image::save(std::string_view filename)
{
    std::string fileExtension = FileUtils::getExtension(filename);
    if (fileExtension == ".png") {
        return savePng(filename);
    }

    return false;
}


Image::Format Image::detectFormat(const unsigned char* pData, size_t dataSize)
{
    if (isBmp(pData, dataSize)) {
        return Format::BMP;
    } 
    else if (isPng(pData, dataSize)) {
        return Format::PNG;
    }
    else if (isJpeg(pData, dataSize)) {
        return Format::JPEG;
    } else
    {
        return Format::UNKNOWN;
    }
}

bool Image::initWithBmpData(const unsigned char* pData, size_t dataSize)
{
    bool result = false;
    unsigned char* pDataPtr = const_cast<unsigned char*>(pData);

    do {
        BitmapFileHeader fileHeader = {};
        memcpy(&fileHeader, pDataPtr, sizeof(BitmapFileHeader));
        pDataPtr += sizeof(BitmapFileHeader);

        // BMPデータか判定
        if (fileHeader.bfType != 0x4D42) {
            OCF_ASSERT("BMP format is not supported");
            break;
        }

        // データサイズが正しいか判定
        if (fileHeader.bfSize != dataSize) {
            OCF_ASSERT("BMP data size is not correct");
            break;
        }

        BitmapInfoHeader infoHeader = {};
        memcpy(&infoHeader, pDataPtr, sizeof(BitmapInfoHeader));
        pDataPtr += sizeof(BitmapInfoHeader);

        // 24bit以外はサポートしない
        if (infoHeader.biBitCount != 24) {
            OCF_ASSERT("BMP format is not supported");
            break;
        }

        m_width = infoHeader.biWidth;
        m_height = std::abs(infoHeader.biHeight);
        m_pixelFormat = PixelFormat::RGB;

        const int rowSize = m_width * (infoHeader.biBitCount / 8);
        const int padding = (4 - (rowSize % 4)) % 4;

        m_pData = new unsigned char[m_height * rowSize];

        pDataPtr = const_cast<unsigned char*>(pData + fileHeader.bfOffBits);

        for (int y = 0; y < m_height; y++) {
            // BMPは下から上に描画されるので、Y座標を反転する
            unsigned char* pRow = m_pData + (m_height - 1 - y) * rowSize;
            memcpy(pRow, pDataPtr, rowSize);

            // BMPはBGR形式なので、RGBに変換する
            for (int x = 0; x < m_width; x++) {
                if (x * 3 + 2 < rowSize) {
                    std::swap(pRow[x * 3 + 0], pRow[x * 3 + 2]);
                }
            }

            // パディング分だけポインタを進める
            pDataPtr += rowSize + padding;
        }

        result = true;
    } while (false);

    return result;
}

bool Image::initWithPngData(const unsigned char* pData, size_t dataSize)
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
        const png_uint_32 colorType = png_get_color_type(png_ptr, info_ptr);

        // ピクセルフォーマットを設定
        switch (colorType) {
        case PNG_COLOR_TYPE_GRAY:
            m_pixelFormat = PixelFormat::GRAY;
            break;
        case PNG_COLOR_TYPE_RGB:
            m_pixelFormat = PixelFormat::RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            m_pixelFormat = PixelFormat::RGBA;
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

bool Image::initWithJpegData(const unsigned char* pData, size_t dataSize)
{
    jpeg_decompress_struct cinfo{};

    jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1] = {0};
    uint32_t location = 0;

    bool result = false;
    do {
        cinfo.err = jpeg_std_error(&jerr);

        jpeg_create_decompress(&cinfo);

        jpeg_mem_src(&cinfo, pData, dataSize);

        jpeg_read_header(&cinfo, TRUE);

        if (cinfo.jpeg_color_space == JCS_GRAYSCALE) {
            m_pixelFormat = PixelFormat::GRAY;
        }
        else {
            cinfo.out_color_space = JCS_RGB;
            m_pixelFormat = PixelFormat::RGB;
        }

        jpeg_start_decompress(&cinfo);

        m_width = cinfo.output_width;
        m_height = cinfo.output_height;

        m_dataSize = cinfo.output_width * cinfo.output_height * cinfo.output_components;
        m_pData = new unsigned char[m_dataSize];
        OCF_BREAK_IF(m_pData == nullptr);

        while (cinfo.output_scanline < cinfo.output_height) {
            row_pointer[0] = m_pData + location;
            location += cinfo.output_width * cinfo.output_components;
            jpeg_read_scanlines(&cinfo, row_pointer, 1);
        }

        jpeg_destroy_decompress(&cinfo);

        result = true;
    } while (false);

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

bool Image::isJpeg(const unsigned char* pData, size_t dataSize)
{
    if (dataSize <= 4)
        return false;

    static const unsigned char JPEG_SIGNATURE[] = {0xFF, 0xD8};

    return memcmp(pData, JPEG_SIGNATURE, 2) == 0;
}

bool Image::savePng(std::string_view filename)
{
    bool result = false;

    do {
        std::ofstream outStream(filename.data(), std::ios::binary);
        OCF_BREAK_IF(!outStream);

        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        OCF_BREAK_IF(png_ptr == nullptr);

        png_infop info_ptr = png_create_info_struct(png_ptr);
        OCF_BREAK_IF(info_ptr == nullptr);

        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }

        png_set_write_fn(png_ptr, &outStream, pngWriteCallback, nullptr);

        png_set_IHDR(png_ptr, info_ptr, m_width, m_height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);

        png_set_packing(png_ptr);

        std::unique_ptr<png_bytep[]> row_pointers(new png_bytep[m_height]);

        if (row_pointers == nullptr) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            break;
        }

        for (int i = 0; i < m_height; i++) {
            row_pointers[i] = m_pData + i * m_width * 3;
        }

        png_write_image(png_ptr, row_pointers.get());

        png_write_end(png_ptr, info_ptr);

        png_destroy_write_struct(&png_ptr, &info_ptr);

        outStream.close();

        result = true;

    } while (false);

    return result;
}

} // namespace ocf
