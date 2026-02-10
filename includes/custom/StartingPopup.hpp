#include "Geode/loader/Log.hpp"
#include "Geode/loader/Mod.hpp"
#include "Geode/ui/TextInput.hpp"
#include "Geode/utils/JsonValidation.hpp"
#include <Geode/utils/async.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/base64.hpp>
#include <matjson.hpp>

using namespace cocos2d;

class StartingPopup : public geode::Popup {
private:
    geode::TextInput* m_clId;
    geode::TextInput* m_clSec;
    geode::TextInput* m_refToken;
    geode::async::TaskHolder<geode::utils::web::WebResponse> m_listener;

protected:
    bool init() {
        if (!Popup::init(240.f, 250.f))
            return false;

        this->setTitle("WAIT!");

        this->m_closeBtn->setVisible(false);

        auto label = CCLabelBMFont::create("To use this mod you MUST\nsetup the spotify API", "bigFont.fnt");
        label->setScale(0.35f);
        label->setAlignment(kCCTextAlignmentCenter);

        m_clId = geode::TextInput::create(200.f, "Client ID", "chatFont.fnt");
        m_clId->setID("clId"_spr);
        m_clId->setCommonFilter(geode::CommonFilter::Any);
        m_clSec = geode::TextInput::create(200.f, "Client Secret", "chatFont.fnt");
        m_clSec->setID("clSec"_spr);
        m_clSec->setCommonFilter(geode::CommonFilter::Any);
        m_refToken = geode::TextInput::create(200.f, "Refresh Token", "chatFont.fnt");
        m_refToken->setID("refToken"_spr);
        m_refToken->setCommonFilter(geode::CommonFilter::Any);

        auto helpBox = geode::MDTextArea::create("Stuck? Click [here](https://google.com) to view a setup guide.", CCSize(200.f, 20.f));
        helpBox->getScrollLayer()->m_disableMovement = true;

        auto saveBtn = CCMenuItemSpriteExtra::create(ButtonSprite::create("Save"), this, menu_selector(StartingPopup::savePressed));

        auto btnMenu = CCMenu::create();
        btnMenu->addChild(saveBtn);

        m_mainLayer->addChildAtPosition(label, geode::Anchor::Top, CCPoint(0, -45.0f));
        m_mainLayer->addChildAtPosition(m_clId, geode::Anchor::Top, CCPoint(0, -85.0f));
        m_mainLayer->addChildAtPosition(m_clSec, geode::Anchor::Top, CCPoint(0, -125.0f));
        m_mainLayer->addChildAtPosition(m_refToken, geode::Anchor::Top, CCPoint(0, -165.0f));
        m_mainLayer->addChildAtPosition(helpBox, geode::Anchor::Top, CCPoint(0, -195.0f));
        m_mainLayer->addChildAtPosition(btnMenu, geode::Anchor::Top, CCPoint(0, -225.0f));

        return true;
    }

    void savePressed(CCObject* sender) {
        geode::log::debug("input sent");

        std::string clientID = m_clId->getString();
        std::string clientSecret = m_clSec->getString();
        std::string refreshToken = m_refToken->getString();

        std::string rawAuthToken = clientID + ":" + clientSecret;

        std::string authToken = geode::utils::base64::encode( rawAuthToken, geode::utils::base64::Base64Variant::Normal);

        auto req = geode::utils::web::WebRequest();

        req.header("Authorization", "Basic " + authToken);
        req.header("Content-Type", "application/x-www-form-urlencoded");

        std::string body = "grant_type=refresh_token&refresh_token=" + refreshToken;

        req.bodyString(body);

        m_listener.spawn(
            req.post("https://accounts.spotify.com/api/token"),
            [this, clientID, clientSecret, refreshToken, sender](geode::utils::web::WebResponse res) {
                if (res.ok()) {
                    geode::Mod::get()->setSettingValue("clid", clientID);
                    geode::Mod::get()->setSettingValue("clsec", clientSecret);
                    geode::Mod::get()->setSettingValue("reftoken", refreshToken);
                    geode::Mod::get()->setSettingValue("accesstoken", res.json().unwrap()["access_token"].asString().unwrap());

                    geode::Mod::get()->setSavedValue<bool>("first-setup-done", true);

                    this->onClose(sender);
                }
            }
        );

        return;
    }

public:
    static StartingPopup* create() {
        auto ret = new StartingPopup();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};
