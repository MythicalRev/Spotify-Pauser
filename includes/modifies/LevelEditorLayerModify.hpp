#pragma once

#include <Geode/Geode.hpp>
#include "../custom/PauseChecker.hpp"
#include "../custom/PlayPauseHandler.hpp"

using namespace geode::prelude;

#include <Geode/modify/LevelEditorLayer.hpp>
class $modify(SpotiLevelEditorLayer, LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) return false;
        
        PauseChecker::checkPause();

        return true;
    }

    void onPlaytest() {
        LevelEditorLayer::onPlaytest();

        PlayPauseHandler::pause();
    }

    void onStopPlaytest() {
        LevelEditorLayer::onStopPlaytest();

        if (!Mod::get()->getSavedValue<bool>("spotify-paused-before-play")) {
            PlayPauseHandler::play();
        }
    }
};