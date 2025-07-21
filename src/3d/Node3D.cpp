#include "Node3D.h"

NS_OCF_BEGIN

Node3D::Node3D()
{
    setName("Node3D");
}

Node3D::~Node3D()
{
}

void Node3D::setPosition(const glm::vec3& position)
{
    m_position = position;
}

void Node3D::setRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
}

void Node3D::setScale(const glm::vec3& scale)
{
    m_scale = scale;
}

glm::vec3 Node3D::getPosition() const
{
    return m_position;
}

glm::vec3 Node3D::getRotation() const
{
    return m_rotation;
}

glm::vec3 Node3D::getScale() const
{
    return m_scale;
}

NS_OCF_END
