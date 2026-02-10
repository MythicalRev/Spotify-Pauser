#pragma once

#include "Geode/utils/async.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include "../custom/PauseChecker.hpp"
#include "../custom/PlayPauseHandler.hpp"

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(SpotiLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        TaskHolder<web::WebResponse> m_listener;

        std::string m_apiEndpoint = "https://api.spotify.com/v1/me/player/pause";
        std::string m_accessToken = Mod::get()->getSettingValue<std::string>("accesstoken");
    };

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