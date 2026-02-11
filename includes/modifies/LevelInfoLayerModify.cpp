#pragma once

#include <Geode/Geode.hpp>
#include "../custom/PauseChecker.cpp"
#include "../custom/PlayPauseHandler.cpp"

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(SpotiLevelInfoLayer, LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        PauseChecker::checkPause();

        return true;
    };

    void onPlay(CCObject* sender) {
        LevelInfoLayer::onPlay(sender);

        PlayPauseHandler::pause();
    }
};