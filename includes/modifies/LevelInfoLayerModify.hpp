#pragma once

#include "Geode/utils/async.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>
#include "../custom/PauseChecker.hpp"

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

        auto req = web::WebRequest();
        req.header("Authorization", "Bearer " + m_fields->m_accessToken);
        req.header("Content-Length", "0");
        req.header("Content-Type", "application/json");

        m_fields->m_listener.spawn(
            req.put(m_fields->m_apiEndpoint, Mod::get()),
            [](web::WebResponse res) {
                if (res.ok()) {
                    log::info("Spotify Paused");
                    return true;
                }

                return false;
            }
        );
    }
};