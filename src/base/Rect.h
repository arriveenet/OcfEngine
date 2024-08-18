#pragma once
#include <glm/glm.hpp>
#include "base/Macros.h"

NS_OCF_BEGIN

class Rect {
public:
    glm::vec2 m_position;
    glm::vec2 m_size;

public:
    Rect();
    Rect(float x, float y, float widht, float height);
    Rect(const glm::vec2& position, const glm::vec2& size);

    Rect& operator= (const Rect& other);

    void setRect(float x, float y, float widht, float height);

    float getMaxX() const;
    float getMaxY() const;
    
    float getMinX() const;
    float getMinY() const;

    bool equals(const Rect& rect) const;

    bool intersect(const glm::vec2& point) const;

    bool intersect(const Rect& rect) const;
};

NS_OCF_END
