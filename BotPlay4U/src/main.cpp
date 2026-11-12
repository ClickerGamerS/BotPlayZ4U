#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "UI/AutoclickerPopup.hpp"
#include "other/variables.hpp"

$on_mod(Loaded) {
    Autoclicker::g_safeModeEnabled =
        geode::Mod::get()->getSavedValue<bool>("safe-mode", true);
}
class $modify(PLHook, PauseLayer) {

	void customSetup() {
		
		PauseLayer::customSetup();

		
		auto btn = CCMenuItemSpriteExtra::create(geode::CircleButtonSprite::createWithSpriteFrameName("gjHand_01_001.png"), this, menu_selector(PLHook::onMyButton));

		auto rightMenu = this->getChildByID("right-button-menu");
		if(!rightMenu) return;

		rightMenu->addChild(btn);
		rightMenu->updateLayout();
		
	}
	
	void onMyButton(CCObject*) {
		if (auto popup = AutoclickerPopup::create()) popup->show();
	}
};
