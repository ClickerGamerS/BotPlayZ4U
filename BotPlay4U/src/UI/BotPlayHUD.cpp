#include "BotPlayHUD.hpp"
#include <algorithm>
#include <cmath>

using namespace cocos2d;

BotPlayHUD* BotPlayHUD::create() {
    auto ret = new BotPlayHUD();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BotPlayHUD::init() {
    if (!CCNode::init()) return false;

    constexpr float width = 154.f;
    constexpr float height = 38.f;
    this->setContentSize({width, height});
    this->setAnchorPoint({0.5f, 0.5f});

    m_background = extension::CCScale9Sprite::create("square02b_001.png");
    if (!m_background) return false;
    m_background->setContentSize({width, height});
    m_background->setPosition({width * 0.5f, height * 0.5f});
    m_background->setOpacity(225);
    this->addChild(m_background);

    m_botLabel = CCLabelBMFont::create("BOT OFF", "bigFont.fnt");
    m_settingsLabel = CCLabelBMFont::create("SET", "bigFont.fnt");
    if (!m_botLabel || !m_settingsLabel) return false;

    m_botLabel->setScale(0.42f);
    m_botLabel->setPosition({width * 0.29f, height * 0.5f});
    m_settingsLabel->setScale(0.42f);
    m_settingsLabel->setPosition({width * 0.81f, height * 0.5f});
    this->addChild(m_botLabel);
    this->addChild(m_settingsLabel);
    refreshLabel();
    this->schedule(schedule_selector(BotPlayHUD::pollState), 0.2f);
    return true;
}

void BotPlayHUD::pollState(float) {
    refreshLabel();
}

void BotPlayHUD::refreshLabel() {
    if (m_botLabel) {
        if (Autoclicker::g_safeModeEnabled && Autoclicker::g_safeModeBlocked) {
            m_botLabel->setString("SAFE STOP");
        } else {
            m_botLabel->setString(Autoclicker::g_autoclickerEnabled ? "BOT ON" : "BOT OFF");
        }
    }
    if (m_background) {
        if (Autoclicker::g_safeModeEnabled && Autoclicker::g_safeModeBlocked) m_background->setColor({155, 45, 40});
        else if (Autoclicker::g_autoclickerEnabled) m_background->setColor({30, 105, 170});
        else m_background->setColor({55, 55, 60});
    }
}

void BotPlayHUD::onEnter() {
    CCNode::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -130, true);
}

void BotPlayHUD::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCNode::onExit();
}

bool BotPlayHUD::ccTouchBegan(CCTouch* touch, CCEvent*) {
    if (!touch || !m_background) return false;
    auto local = this->convertToNodeSpace(touch->getLocation());
    if (!m_background->boundingBox().containsPoint(local)) return false;

    m_touchStart = touch->getLocation();
    m_initialPosition = this->getPosition();
    m_dragged = false;
    return true;
}

void BotPlayHUD::ccTouchMoved(CCTouch* touch, CCEvent*) {
    if (!touch || !this->getParent()) return;
    auto now = touch->getLocation();
    auto dx = now.x - m_touchStart.x;
    auto dy = now.y - m_touchStart.y;
    if (std::abs(dx) > 6.f || std::abs(dy) > 6.f) m_dragged = true;
    if (!m_dragged) return;

    auto parentSize = this->getParent()->getContentSize();
    auto x = m_initialPosition.x + dx;
    auto y = m_initialPosition.y + dy;
    x = std::max(80.f, std::min(x, parentSize.width - 80.f));
    y = std::max(22.f, std::min(y, parentSize.height - 22.f));
    this->setPosition({x, y});
}

void BotPlayHUD::ccTouchEnded(CCTouch* touch, CCEvent*) {
    if (!touch || m_dragged) return;
    auto local = this->convertToNodeSpace(touch->getLocation());
    if (local.x >= this->getContentSize().width * 0.60f) {
        if (auto popup = AutoclickerPopup::create()) popup->show();
        return;
    }
    Autoclicker::g_autoclickerEnabled = !Autoclicker::g_autoclickerEnabled;
    if (!Autoclicker::g_autoclickerEnabled) Autoclicker::g_safeModeBlocked = false;
    refreshLabel();
}
