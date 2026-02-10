#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/async.hpp>

using namespace geode::prelude;

class PauseChecker {
private:
    std::string m_accessToken = Mod::get()->getSettingValue<std::string>("accesstoken");
protected:
    bool checkForPause() {
        if (Mod::get()->getSavedValue<bool>("skip-pause-check")) {
            Mod::get()->setSavedValue<bool>("skip-pause-check", false);
            } else {
            auto req = web::WebRequest();
            req.header("Authorization", "Bearer " + m_accessToken);
            req.header("Content-Length", "0");
            req.header("Content-Type", "application/json");

            async::spawn(
                req.get("https://api.spotify.com/v1/me/player"),
                [](web::WebResponse res) {
                    if (res.ok()) {
                        if (res.json().unwrap()["is_playing"].asBool().unwrap()) {
                            Mod::get()->setSavedValue<bool>("spotify-paused-before-play", false);
                        } else {
                            Mod::get()->setSavedValue<bool>("spotify-paused-before-play", true);
                        }

                        return true;
                    } else {
                        return false;
                    }
                }
            );
        }

        return false;
    }
public:
    static PauseChecker* checkPause() {
        auto ret = new PauseChecker();
        if (ret->checkForPause()) {
            return ret;
        }

        delete ret;
        return nullptr;
    }
};