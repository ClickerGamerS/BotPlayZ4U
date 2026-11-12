#pragma once

#include <Geode/Geode.hpp>
#include "AutoclickerPopup.hpp"
#include "../other/variables.hpp"

class BotPlayHUD : public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate {
protected:
    cocos2d::extension::CCScale9Sprite* m_background = nullptr;
    cocos2d::CCLabelBMFont* m_botLabel = nullptr;
    cocos2d::CCLabelBMFont* m_settingsLabel = nullptr;
    cocos2d::CCPoint m_touchStart = {0.f, 0.f};
    cocos2d::CCPoint m_initialPosition = {0.f, 0.f};
    bool m_dragged = false;

    bool init() override;
    void refreshLabel();
    void pollState(float dt);

public:
    static BotPlayHUD* create();
    void onEnter() override;
    void onExit() override;
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) override;
};
