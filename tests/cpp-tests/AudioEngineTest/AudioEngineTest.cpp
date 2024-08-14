#include "AudioEngineTest.h"

USING_NS_OCF;
using namespace ocf::ui;

AudioEngineTest::AudioEngineTest()
{
}

AudioEngineTest::~AudioEngineTest()
{
}

bool AudioEngineTest::init()
{
    if (!TestCase::init()) {
        return false;
    }

	auto button1 = Button::create("ButtonNormal.png", "ButtonActive.png");
	button1->setText("Play");
	button1->setPosition(100, 500);
	button1->setOnClickCallback([=]() {
		 m_audioID = AudioEngine::play("05 Strength & Guile.wav", true);
		//AudioEngine::play("タイフーンパレード.wav");
		});
	addChild(button1);

	auto stopButton = Button::create("ButtonNormal.png", "ButtonActive.png");
	stopButton->setText("Stop");
	stopButton->setPosition(300, 500);
	stopButton->setOnClickCallback([=]() {
		AudioEngine::stop(m_audioID);
		});
	addChild(stopButton);

    return true;
}
