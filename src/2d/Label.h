#pragma once
#include <string>
#include <vector>
#include "2d/Node2D.h"
#include "base/Types.h"
#include "base/Macros.h"
#include "renderer/VertexArray.h"
#include "renderer/QuadCommand.h"

NS_OCF_BEGIN

class FontAtlas;
class DrawShape;
class Font;

class Label : public Node2D {
public:
    enum class LabelType {
        BMFONT,
        TTF,
    };

    static Label* create(const std::string& text);
    static Label* createWithBMFont(std::string_view bmFontPath, std::string_view text);
    static Label* createWithTTF(std::string_view ttfPath, std::string_view text, int fontSize);

    Label();
    virtual ~Label();

    bool init();
    bool initWithBMFont(std::string_view bmFontPath);
    bool initWithTTF(std::string_view ttfPath, int fontSize);

    Font* getFont() const { return m_font; }

    void setString(std::string_view text);
    std::string getString() const { return m_text; }
    void setTextColor(const glm::vec3& textColor);
    void setTextColor(unsigned char r, unsigned char g, unsigned char b);
    const glm::vec3& getTextColor() const { return m_textColor; }

    void update(float deltaTime) override;

    void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
    void updateContent();
    void updateQuads();
    void updateBatchCommands();

    void setProgram(Program* program);

    struct BatchCommand {
        QuadCommand quadCommand;
        std::vector<QuadV3fC3fT2f> quads;
        std::vector<unsigned short> indices;
        Texture2D* texture;

        BatchCommand();
        void clear();
        void insertQuad(const QuadV3fC3fT2f& quad);
    };

protected:
    Font* m_font;
    LabelType m_labelType;
    std::string m_text;
    std::u32string m_utf32Text;
    glm::vec3 m_textColor;
    bool m_isDirty;
    FontAtlas* m_fontAtlas = nullptr;
    std::vector<BatchCommand> m_batchCommands;
#if OCF_LABEL_DEBUG_DRAW
    DrawShape* m_pDebugDrawShape = nullptr;
#endif
};

NS_OCF_END
