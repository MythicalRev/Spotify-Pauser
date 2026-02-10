#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
class $modify(SpotiLevelInfoLayer, LevelInfoLayer) {
    struct Fields {
        TaskHolder<web::WebResponse> m_listener;

        std::string m_apiEndpoint = "https://api.spotify.com/v1/me/player/pause";
    };

    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        return true;
    };

    

    void onPlay(CCObject* sender) {
        LevelInfoLayer::onPlay(sender);

        auto req = web::WebRequest();
        req.header("Authorization", "Bearer BQDOZ5y7borwDiuZ267x5fcHx08fgARg3_dnvQWlmtzuIKA9iTpjc1q6hFNqPqkCbfWf6nWcoYBZ3TSK0kDwYl3xrPOt10DoWt06Z6Pl32oyx_xmJLqBYJV9wpEjqHzrCWkyxh7z0lznOHycJg1f1PRQn4zXjGvFQx4ta0pTuVmxG8MyqD9L2j6_51GyyWqbjScU-bDeNXr3l5ne8JKY7PSKD_-E-WwrJgz1w6_ov495ciUfi3EEz3MTLeiCDBwSnyxdUZAxzYNdZUPNrrdPHXTeAZXwjeefk5xfwS1V-L1v0msAfiovKy7XVEyJOeFBpAs2lnJ5kYy0NoQE7a2HZxFp5EF_cLQxly6kxLNIpK8R0HvPeL6IHJRDwkwm7alAAbDU7nNCdLU");
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