#pragma once
#include "base/Config.h"
#include <string_view>

typedef struct FT_LibraryRec_* FT_Library;
typedef struct FT_StreamRec_* FT_Stream;
typedef struct FT_FaceRec_* FT_Face;
typedef struct FT_StrokerRec_* FT_Stroker;
typedef struct FT_BBox_ FT_BBox;

NS_OCF_BEGIN

struct FontFaceInfo {
    FT_Face face;
    long index;
    unsigned int currentGlyphIndex;

    std::string_view familyName;
    std::string_view path;
};

class IFontEngine {
public:
    virtual ~IFontEngine() = default;
    virtual FontFaceInfo* lookupFontFaceForCodepoint(char32_t codepoint) = 0;

};

NS_OCF_END