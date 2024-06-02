#pragma once
#include "Component.h"
#include "base/GameObject.h"
#include "input/Input.h"
#include <glm/glm.hpp>
#include <vector>

OCF_BEGIN

class Game;
class Renderer;

/**
 * @brief ノードクラス
 */
class Node : public GameObject {
public:
	/**
	 * @brief エンティティの状態
	 */
	enum State {
		Active,	//!< 生存中
		Dead	//!< 死亡
	};

	enum {
		FLAGS_TRANSFORM_DIRTY		= (1 << 0),
		FLAGS_CONTENT_SIZE_DIRTY	= (1 << 1),
		FLAGS_RENDER_AS_3D			= (1 << 3),

		FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
	};
	/** コンストラクタ */
	Node();
	/** デストラクタ */
	virtual ~Node();

	virtual bool init();

	/**
	 * @brief 入力処理
	 * @param inputState 入力状態
	 */
	virtual void processInput(const InputState& inputState);

	virtual void update(float deltaTime);
	virtual void updateNode(float deltaTime);
	virtual void updateComponents(float deltaTime);
	virtual void updateNodes(float deltaTime);
	virtual void updateTransform();

	virtual void draw(Renderer* renderer, const glm::mat4& transform);

	void setState(State state) { m_state = state; }
	State getState() const { return m_state; }

	virtual void setPosition(const glm::vec3& position);
	virtual void setPosition(const glm::vec2& position);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(float x, float y);
	virtual glm::vec3 getPosition() const;

	virtual void setSize(const glm::vec2& size);
	virtual void setSize(float width, float height);
	virtual glm::vec2 getSize() const;

	virtual void setAnchorPoint(const glm::vec2& point);
	virtual const glm::vec2& getAnchorPoint() const;

	virtual void setRotation(float rotation);
	virtual float getRotation() const;

	virtual void setScale(float scale);
	virtual void setScaleX(float scaleX);
	virtual void setScaleY(float scaleY);
	virtual void setScaleZ(float scaleZ);
	virtual float getScale() const;

	/**
	 * @brief 子ノードを追加
	 * @param pNode 追加する子ノード
	 */
	virtual void addChild(Node* pNode);
	/**
	 * @brief 子ノードを削除
	 * @param pNode 削除する子ノード
	 */
	virtual void removeChild(Node* pNode);
	/**
	 * @brief 子ノードの数を取得
	 * @return 子ノードの数
	 */
	virtual size_t getChildCount() const;
	/**
	 * @brief 親ノードを設定
	 * @param pNode 設定する親ノード
	 */
	virtual void setParent(Node* pNode);
	/**
	 * @brief 親ノードを取得
	 * @return pNode 親ノード
	 */
	virtual Node* getParent() { return m_pParent; }

	/**
	 * @brief カメラのマスクを取得
	 * @return カメラのマスク
	 */
	uint16_t getCameraMask() const { return m_cameraMask; }
	/**
	 * @brief カメラのマスクを設定
	 * @param uint16_t カメラのマスク
	 * @param applyChildren 子ノードにもカメラマスクをせっていするかどうか
	 */
	virtual void setCameraMask(uint16_t mask, bool applyChildren = true);

	void addComponent(Component* pComponent);
	void removeComponent(Component* pComponent);

	const glm::mat4& getNodeToParentTransform();

	virtual void visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags);

protected:
	bool isVisitableByVisitingCamera() const;
	glm::mat4 transform(const glm::mat4& parentTransform);
	uint32_t processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag);
	
protected:
	State m_state;			//!< ノードの状態

	Node* m_pParent;		//!< 親ノード
	Game* m_pGame;			//!< シングルトンのゲームクラス

	uint16_t m_cameraMask;	//!< カメラマスク

	glm::vec3 m_position;	//!< ノードの位置
	glm::vec2 m_size;		//!< コンテンツサイズ

	glm::vec2 m_anchorPointInPoints;	//!< ポイントのアンカーポイント
	glm::vec2 m_anchorPoint;			//!< 正規化されたアンカーポイント

	float m_rotation;		//!< Z軸の回転角度
	
	float m_scaleX;			//!< X軸の倍率
	float m_scaleY;			//!< Y軸の倍率
	float m_scaleZ;			//!< Z軸の倍率

	glm::mat4 m_transform;			//!< 変換行列
	glm::mat4 m_modelVewTransform;	//!< モデルビュー行列

	bool m_ignoreAnchorPointForPosition;	//!< ノードの基準点がvec2(0,0)の場合はtrue、それ以外の場合はfalse
	bool m_transformDirty;					//!< 変換のダーティーフラグ
	bool m_transformUpdated;				//!< 変換の更新のダーティーフラグ
	bool m_contentSizeDirty;				//!< コンテンツサイズのダーティーフラグ

	std::vector<Node*> m_children;			//!< 子ノードの配列
	std::vector<Component*> m_components;	//!< コンポーネント
};

OCF_END
