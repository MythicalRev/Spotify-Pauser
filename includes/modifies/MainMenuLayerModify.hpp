#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../custom/StartingPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
class $modify(SpotiMainMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        return true;
    }

    void firstNetworkTest() {
        MenuLayer::firstNetworkTest();

        if (!Mod::get()->getSavedValue<bool>("first-setup-done")) {
            StartingPopup::create()->show();
        }
    }
};