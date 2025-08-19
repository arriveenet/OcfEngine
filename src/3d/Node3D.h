#pragma once
#include "base/Node.h"

namespace ocf {

class Node3D : public Node {
public:
    Node3D();
    ~Node3D();

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    void setScale(const glm::vec3& scale);

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;

protected:
    glm::vec3 m_position; // Position in 3D space
    glm::vec3 m_rotation; // Rotation in degrees (pitch, yaw, roll)
    glm::vec3 m_scale;    // Scale factors for x, y, z axes

    glm::mat4 m_transform; // Transformation matrix combining position, rotation, and scale
};

} // namespace ocf