#pragma once
#include <cstdint>
#include <algorithm>
#include <vector>
#include <glm/glm.hpp>
#include "2d/Component.h"
#include "base/GameObject.h"
#include "input/Input.h"

NS_OCF_BEGIN

class Game;
class Renderer;
class Scene;

/**
 * @brief �m�[�h�N���X
 */
class Node : public GameObject {
public:
	enum {
		FLAGS_TRANSFORM_DIRTY		= (1 << 0),
		FLAGS_CONTENT_SIZE_DIRTY	= (1 << 1),
		FLAGS_RENDER_AS_3D			= (1 << 3),

		FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
	};
	/** �f�t�H���g�R���X�g���N�^ */
	Node();
	/** �f�X�g���N�^ */
	virtual ~Node();

	virtual bool init();

	/**
	 * @brief ���͏���
	 * @param inputState ���͏��
	 */
	virtual void processInput(const InputState& inputState);

	virtual void update(float deltaTime);
	virtual void updateNode(float deltaTime);
	virtual void updateComponents(float deltaTime);
	virtual void updateNodes(float deltaTime);
	virtual void updateTransform();

	virtual void draw(Renderer* renderer, const glm::mat4& transform);

	virtual void setPosition(const glm::vec3& position);
	virtual void setPosition(const glm::vec2& position);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(float x, float y);
	virtual glm::vec3 getPosition() const;

	virtual void setSize(const glm::vec2& size);
	virtual void setSize(float width, float height);
	virtual glm::vec2 getSize() const;

	virtual bool hitTest(const glm::vec2& worldPoint) const;

	virtual void setAnchorPoint(const glm::vec2& point);
	virtual const glm::vec2& getAnchorPoint() const;

	virtual void setRotation(float rotation);
	virtual float getRotation() const;

	virtual void setScale(float scale);
	virtual void setScaleX(float scaleX);
	virtual void setScaleY(float scaleY);
	virtual void setScaleZ(float scaleZ);
	virtual float getScale() const;

	virtual void setGlobalZOrder(float globalZorder);
	virtual float getGlobalZOrder() const { return m_globalZOrder; }

	/**
	 * @brief �q�m�[�h��ǉ�
	 * @param pNode �ǉ�����q�m�[�h
	 */
	virtual void addChild(Node* pNode);
	/**
	 * @brief �q�m�[�h���폜
	 * @param pNode �폜����q�m�[�h
	 */
	virtual void removeChild(Node* pNode);
	/**
	 * @brief �q�m�[�h�̐����擾
	 * @return �q�m�[�h�̐�
	 */
	virtual size_t getChildCount() const;
	/**
	 * @brief �e�m�[�h��ݒ�
	 * @param pNode �ݒ肷��e�m�[�h
	 */
	virtual void setParent(Node* pNode);
	/**
	 * @brief �e�m�[�h���擾
	 * @return pNode �e�m�[�h
	 */
	virtual Node* getParent() { return m_pParent; }

	/**
	 * @brief �q�m�[�h�����ׂă\�[�g����
	 */
	virtual void sortAllChildren();

	/**
	 * @brief �\�[�g�̃w���p�֐�
	 * @param nodes �m�[�h�̔z��
	 */
	template<typename T>
	inline static void sortNodes(std::vector<T*>& nodes)
	{
		static_assert(std::is_base_of<Node, T>::value, "Node::sortNodes: Only accept derived of Node!");
		std::sort(std::begin(nodes), std::end(nodes),
			[](T* n1, T* n2) { return (n1->m_localZOrder < n1->m_localZOrder); });
	}

	/**
	 * @brief �J�����̃}�X�N���擾
	 * @return �J�����̃}�X�N
	 */
	uint16_t getCameraMask() const { return m_cameraMask; }
	/**
	 * @brief �J�����̃}�X�N��ݒ�
	 * @param uint16_t �J�����̃}�X�N
	 * @param applyChildren �q�m�[�h�ɂ��J�����}�X�N��ݒ肷�邩�ǂ���
	 */
	virtual void setCameraMask(uint16_t mask, bool applyChildren = true);

	void addComponent(Component* pComponent);
	void removeComponent(Component* pComponent);

	virtual const glm::mat4& getNodeToParentTransform() const;
	virtual glm::mat4 getNodeToParentTransform(Node* ancestor) const;
	virtual glm::mat4 getNodeToWorldTransform() const;
	virtual glm::mat4 getWorldToNodeTransform() const;

	glm::vec2 convertToNodeSpace(const glm::vec2& worldPoint) const;
	glm::vec2 convertToWorldSpace(const glm::vec2& nodePoint) const;

	virtual void visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags);

	virtual Scene* getScene() const;

protected:
	bool isVisitableByVisitingCamera() const;
	glm::mat4 transform(const glm::mat4& parentTransform);
	uint32_t processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag);
	
protected:
	Node* m_pParent;		//!< �e�m�[�h
	Game* m_pGame;			//!< �V���O���g���̃Q�[���N���X

	uint16_t m_cameraMask;	//!< �J�����}�X�N

	glm::vec3 m_position;	//!< �m�[�h�̈ʒu
	glm::vec2 m_size;		//!< �R���e���c�T�C�Y

	glm::vec2 m_anchorPointInPoints;	//!< �|�C���g�̃A���J�[�|�C���g
	glm::vec2 m_anchorPoint;			//!< ���K�����ꂽ�A���J�[�|�C���g

	float m_rotation;		//!< Z���̉�]�p�x
	
	float m_scaleX;			//!< X���̔{��
	float m_scaleY;			//!< Y���̔{��
	float m_scaleZ;			//!< Z���̔{��

	mutable glm::mat4 m_transform;			//!< �ϊ��s��
	mutable glm::mat4 m_modelVewTransform;	//!< ���f���r���[�s��

	std::int32_t m_localZOrder;	//!< ���[�J��Z�I�[�_�[
	float m_globalZOrder;		//!< �O���[�o��Z�I�[�_�[

	bool m_ignoreAnchorPointForPosition;	//!< �m�[�h�̊�_��vec2(0,0)�̏ꍇ��true�A����ȊO�̏ꍇ��false
	mutable bool m_transformDirty;			//!< �ϊ��̃_�[�e�B�[�t���O
	bool m_transformUpdated;				//!< �ϊ��̍X�V�̃_�[�e�B�[�t���O
	bool m_contentSizeDirty;				//!< �R���e���c�T�C�Y�̃_�[�e�B�[�t���O

	std::vector<Node*> m_children;			//!< �q�m�[�h�̔z��
	std::vector<Component*> m_components;	//!< �R���|�[�l���g
};

NS_OCF_END
