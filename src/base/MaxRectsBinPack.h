#pragma once
#include "base/RectBinPack.h"

NS_OCF_BEGIN

class MaxRectsBinPack : public RectBinPack {
public:
    enum class FreeRectChoiceHeuristic {
        RectBestsShortSideFit,
        RectBestLongSideFit,
        RectBestAreaFit,
        RectBottomLeftRule,
        RectContactPointRule
    };


    MaxRectsBinPack();
    virtual ~MaxRectsBinPack();

    void init(float width, float height) override;

    void insert(std::vector<glm::vec2> rects, std::vector<Rect>& dest) override;

    Rect insert(float width, float height) override;

    Rect scoreRect(float width, float height, float score1, float score2) const;

    void setHeuristic(FreeRectChoiceHeuristic heuristic) { m_heuristic = heuristic; }
    FreeRectChoiceHeuristic getHeuristic() const { return m_heuristic; }

private:
    Rect findPositionForShortSideFit(float width, float height,
                                     float& bestShortSideFit, float& bestLongSideFit) const;

    Rect findPositionForLongSideFit(float width, float height,
                                    float& bestShortSideFit, float& bestLongSideFit) const;

    Rect findPositionForAreaFit(float width, float height,
                                float& bestAreaFit, float& bestShortSideFit) const;

    Rect findPositionForNewNodeBottomLeft(float width, float height,
                                          float& bestY, float& bestX) const;

    Rect findPositionForContactPoint(float width, float height, float bestContactScore) const;

    float commonIntervalLength(float i1start, float i1end, float i2start, float i2end) const;

    float contactPointScoreNode(float x, float y, float width, float height) const;

    void placeRect(const Rect& node);

    bool splitFreeNode(const Rect& freeNode, const Rect& usedNode);

    void insertNewFreeRects(const Rect& newFreeRect);

    void pruneFreeList();

private:
    size_t m_newFreeRectsLastSize;
    std::vector<Rect> m_newFreeRects;
    std::vector<Rect> m_freeRects;
    std::vector<Rect> m_usedRects;
    FreeRectChoiceHeuristic m_heuristic;
};

NS_OCF_END