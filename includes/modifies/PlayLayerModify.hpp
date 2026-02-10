#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

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
            auto req = web::WebRequest();
            req.header("Authorization", "Bearer " + m_fields->m_accessToken);
            req.header("Content-Length", "0");
            req.header("Content-Type", "application/json");

            m_fields->m_listener.spawn(
                req.put(m_fields->m_apiEndpoint, Mod::get()),
                [](web::WebResponse res) {
                    if (res.ok()) {
                        log::info("Spotify Resumed");
                        return true;
                    }

                    return false;
                }
            );
        }
    }
};