#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../other/variables.hpp"
#include "../UI/BotPlayHUD.hpp"



class $modify(AutoclickerGJBGLHook, GJBaseGameLayer) {
    struct Fields {
        size_t currentIdx[2] = {0, 0};
        size_t currentInputDelta[2] = {0, 0};
        size_t currentRepeats[2] = {0, 0};
        size_t lastDelta[2] = {0, 0};
        bool isRelease[2] = {false, false}; 
    };

    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        // Safe Mode freezes automated inputs once the finish guard has engaged.
        if (!Autoclicker::g_autoclickerEnabled ||
            (Autoclicker::g_safeModeEnabled && Autoclicker::g_safeModeBlocked)) {
            return GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
        }
        
        auto f = m_fields.self();
        //geode::log::info("delta1p: {}, actDel: {}, repeat1p {}, repeatAc {}, currentidx1p {}", m_fields->currentInputDelta[0], Autoclicker::g_inputs[0][f->currentIdx[0]].ticks[f->isRelease[0]], f->currentRepeats[0], Autoclicker::g_inputs[0][f->currentIdx[0]].repeatTimes, f->currentIdx[0]);
        //yes the line above makes the game crash if the vector is empty

        for(size_t i = 0; i < 2; i++) {

            if (Autoclicker::g_inputs[i].empty()) continue;

            if(f->currentIdx[i] >= Autoclicker::g_inputs[i].size()) f->currentIdx[i] = 0;
            if(f->currentRepeats[i] >= Autoclicker::g_inputs[i][f->currentIdx[i]].repeatTimes) f->currentRepeats[i] = 0;
            

            while(f->currentInputDelta[i] == f->lastDelta[i]) {

                f->lastDelta[i] = Autoclicker::g_inputs[i][f->currentIdx[i]].ticks[f->isRelease[i]];
                // geode::log::info("lastdelta:{}, isRel: {}", f->lastDelta[i], f->isRelease[i]);

                f->currentInputDelta[i] = 0;
                if(f->isRelease[i]) f->currentRepeats[i]++; //hold && release

                f->isRelease[i] = !f->isRelease[i]; //switch between release and hold because
                GJBaseGameLayer::handleButton(f->isRelease[i], 1, !i); //first is player 1 (!i)

                if(f->currentRepeats[i] >= Autoclicker::g_inputs[i][f->currentIdx[i]].repeatTimes) {
                    f->currentRepeats[i] = 0;
                    f->currentIdx[i]++;
                    if(f->currentIdx[i] >= Autoclicker::g_inputs[i].size()) f->currentIdx[i] = 0;
                }
            }
            f->currentInputDelta[i]++;

        }
        
        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    }
    
};

void checkDelta() {
    auto gjbgl = GJBaseGameLayer::get();
    if(gjbgl) {
        for(size_t i = 0; i < 2; i++) {
            bool smallerDelta = false;
            size_t deltaGjbgl = static_cast<AutoclickerGJBGLHook*>(gjbgl)->m_fields->currentInputDelta[i];
            for(const auto& delta : Autoclicker::g_inputs[i]) {

                if(delta.ticks[0] >= deltaGjbgl || delta.ticks[1] >= deltaGjbgl) {
                    smallerDelta = true;
                    break;
                }

            }
            if(!smallerDelta) static_cast<AutoclickerGJBGLHook*>(gjbgl)->m_fields->currentInputDelta[i] = 0;
        }
    }
}

class $modify(AutoclickerPLHook, PlayLayer) {
    struct Fields {
        cocos2d::CCNode* safeModeOverlay = nullptr;
        BotPlayHUD* hud = nullptr;
        bool hitboxesApplied = false;
    };

    void removeSafeModeOverlay() {
        if (m_fields->safeModeOverlay) {
            m_fields->safeModeOverlay->removeFromParentAndCleanup(true);
            m_fields->safeModeOverlay = nullptr;
        }
    }

    void showSafeModeOverlay() {
        if (m_fields->safeModeOverlay) {
            m_fields->safeModeOverlay->setVisible(true);
            return;
        }

        auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
        auto overlay = cocos2d::CCNode::create();
        overlay->setID("botplay4u-safe-mode-overlay");
        auto barrier = cocos2d::CCDrawNode::create();
        barrier->drawSegment(
            {size.width * 0.91f, size.height * 0.12f},
            {size.width * 0.91f, size.height * 0.88f},
            4.0f,
            cocos2d::ccColor4F(1.0f, 0.18f, 0.12f, 0.95f)
        );

        auto label = cocos2d::CCLabelBMFont::create("SAFE MODE: FINISH BLOCKED", "bigFont.fnt");
        if (label) {
            label->setScale(0.5f);
            label->setPosition({size.width / 2.0f, size.height * 0.88f});
            overlay->addChild(label);
        }
        overlay->addChild(barrier);
        this->addChild(overlay, 9999);
        m_fields->safeModeOverlay = overlay;
    }

    void setupHasCompleted() {
        Autoclicker::g_safeModeBlocked = false;
        checkDelta();
        PlayLayer::setupHasCompleted();
        if (!m_fields->hitboxesApplied) {
            if (Autoclicker::g_hitboxesEnabled) this->toggleDebugDraw();
            m_fields->hitboxesApplied = true;
        }
        if (!m_fields->hud) {
            auto size = cocos2d::CCDirector::sharedDirector()->getWinSize();
            m_fields->hud = BotPlayHUD::create();
            if (m_fields->hud) {
                m_fields->hud->setPosition({96.f, size.height - 34.f});
                this->addChild(m_fields->hud, 10000);
            }
        }
    }

    void resumeAndRestart(bool fromStart) {
        Autoclicker::g_safeModeBlocked = false;
        removeSafeModeOverlay();
        checkDelta();
        PlayLayer::resumeAndRestart(fromStart);
    }

    void resume() {
        checkDelta();
        PlayLayer::resume();
    }

    float getCurrentPercent() {
        // Keep the reported/saved progress at zero during Safe Mode bot runs.
        if (Autoclicker::g_safeModeEnabled &&
            (Autoclicker::g_autoclickerEnabled || Autoclicker::g_safeModeBlocked)) {
            return 0.0f;
        }
        return PlayLayer::getCurrentPercent();
    }


    void checkForEnd() {
        // Use the original percent calculation here, before our display/save mask.
        auto realPercent = PlayLayer::getCurrentPercent();
        if (Autoclicker::g_safeModeEnabled && Autoclicker::g_autoclickerEnabled && realPercent >= 99.0f) {
            Autoclicker::g_safeModeBlocked = true;
            showSafeModeOverlay();
            this->pauseGame(false);
            return;
        }
        if (Autoclicker::g_safeModeEnabled && Autoclicker::g_safeModeBlocked) return;
        PlayLayer::checkForEnd();
    }
};
