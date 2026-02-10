#pragma once

#include <Geode/Geode.hpp>
#include "../custom/PlayPauseHandler.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(SpotiPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        
            Mod::get()->setSavedValue<bool>("skip-pause-check", true);

        return true;
    }

    void onQuit() {
        PlayLayer::onQuit();

        if (!Mod::get()->getSavedValue<bool>("spotify-paused-before-play")) {
            PlayPauseHandler::play();
        }
    }
};