#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>

using namespace geode::prelude;

class PlayPauseHandler {
private:
    std::string m_accessToken = Mod::get()->getSettingValue<std::string>("accesstoken");
protected:
    bool pausePB() {
        auto req = web::WebRequest();
        req.header("Authorization", "Bearer " + m_accessToken);
        req.header("Content-Length", "0");
        req.header("Content-Type", "application/json");

        async::spawn(
            req.put("https://api.spotify.com/v1/me/player/pause", Mod::get()),
            [](web::WebResponse res) {
                if (res.ok()) {
                    log::info("Spotify Paused");
                    return true;
                }

                return false;
            }
        );

        return false;
    }

    bool playPB() {
        auto req = web::WebRequest();
        req.header("Authorization", "Bearer " + m_accessToken);
        req.header("Content-Length", "0");
        req.header("Content-Type", "application/json");

        async::spawn(
            req.put("https://api.spotify.com/v1/me/player/play", Mod::get()),
            [](web::WebResponse res) {
                if (res.ok()) {
                    log::info("Spotify Resumed");
                    return true;
                }

                return false;
            }
        );

        return false;
    }
public:
    static PlayPauseHandler* pause() {
        auto ret = new PlayPauseHandler();
        if (ret->pausePB()) {
            return ret;
        }

        delete ret;
        return nullptr;
    }

    static PlayPauseHandler* play() {
        auto ret = new PlayPauseHandler();
        if (ret->playPB()) {
            return ret;
        }

        delete ret;
        return nullptr;
    }
};