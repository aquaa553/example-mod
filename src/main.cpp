#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(FrameCounterPlayLayer, PlayLayer) {
	struct Fields {
		CCLabelBMFont* m_frameLabel = nullptr;
	};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

		auto label = CCLabelBMFont::create("Frame: 0", "bigFont.fnt");
		label->setAnchorPoint({0.f, 1.f});

		auto winSize = CCDirector::get()->getWinSize();
		label->setPosition({5.f, winSize.height - 5.f});
		label->setID("frame-counter-label"_spr);

		m_uiLayer->addChild(label, 100);
		m_fields->m_frameLabel = label;

		this->applySettings();
		return true;
	}

	void applySettings() {
		auto label = m_fields->m_frameLabel;
		if (!label) return;

		auto mod = Mod::get();
		label->setVisible(mod->getSettingValue<bool>("show-counter"));
		label->setOpacity(static_cast<GLubyte>(mod->getSettingValue<int64_t>("opacity")));
		label->setScale(static_cast<float>(mod->getSettingValue<double>("scale")));
	}

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);

		auto label = m_fields->m_frameLabel;
		if (!label || !label->isVisible()) return;

		// m_currentProgress is the game's physics step counter for the
		// current attempt, so it resets on respawn.
		label->setString(fmt::format("Frame: {}", m_gameState.m_currentProgress).c_str());
	}

	void resetLevel() {
		PlayLayer::resetLevel();
		this->applySettings(); // pick up any setting changes between attempts
	}
};
