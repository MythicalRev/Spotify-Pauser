#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "../custom/PlayPauseHandler.hpp"

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>
class $modify(SpotiPlayLayer, PlayLayer) {
    struct Fields {
        TaskHolder<web::WebResponse> m_listener;

        std::string m_apiEndpoint = "https://api.spotify.com/v1/me/player/play";
        std::string m_accessToken = Mod::get()->getSettingValue<std::string>("accesstoken");
    };

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