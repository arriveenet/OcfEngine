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

	float leftX = 300.0f;
	float leftX2 = 550.0f;

	std::string audioFileName = "05 Strength & Guile.wav";

	auto fileNameLabel = Label::create(audioFileName);
	fileNameLabel->setPosition(leftX, 550);
	addChild(fileNameLabel);

	auto button1 = Button::create("ButtonNormal.png", "ButtonActive.png");
	button1->setText("Play");
	button1->setPosition(leftX, 500);
	button1->setOnClickCallback([=]() {
		 m_audioID = AudioEngine::play(audioFileName, true);
		});
	addChild(button1);

	auto stopButton = Button::create("ButtonNormal.png", "ButtonActive.png");
	stopButton->setText("Stop");
	stopButton->setPosition(leftX2, 500);
	stopButton->setOnClickCallback([=]() {
		AudioEngine::stop(m_audioID);
		});
	addChild(stopButton);

	auto pauseButton = Button::create("ButtonNormal.png", "ButtonActive.png");
	pauseButton->setText("Pause");
	pauseButton->setPosition(leftX, 400);
	pauseButton->setOnClickCallback([=]() {
		AudioEngine::pause(m_audioID);
		});
	addChild(pauseButton);

	auto resumeButton = Button::create("ButtonNormal.png", "ButtonActive.png");
	resumeButton->setText("Resume");
	resumeButton->setPosition(leftX2, 400);
	resumeButton->setOnClickCallback([=]() {
		AudioEngine::resume(m_audioID);
		});
	addChild(resumeButton);

	m_volumeLabel = Label::create("Volume: 1.0f");
	m_volumeLabel->setPosition(leftX, 330);
	addChild(m_volumeLabel);

	auto voluemUp = Button::create("ButtonNormal.png", "ButtonActive.png");
	voluemUp->setText("+");
	voluemUp->setPosition(leftX, 300);
	voluemUp->setOnClickCallback([=]() {
		float voluem = AudioEngine::getVolume(m_audioID);
		voluem += 0.1f;
		AudioEngine::setVolume(m_audioID, voluem);
		char str[128];
		sprintf_s(str, "Volume: %f", AudioEngine::getVolume(m_audioID));
		m_volumeLabel->setString(str);
		});
	addChild(voluemUp);

	auto voluemDown = Button::create("ButtonNormal.png", "ButtonActive.png");
	voluemDown->setText("-");
	voluemDown->setPosition(leftX2, 300);
	voluemDown->setOnClickCallback([=]() {
		float voluem = AudioEngine::getVolume(m_audioID);
		voluem -= 0.1f;
		AudioEngine::setVolume(m_audioID, voluem);
		char str[128];
		sprintf_s(str, "Volume: %f", AudioEngine::getVolume(m_audioID));
		m_volumeLabel->setString(str);
		});
	addChild(voluemDown);

    return true;
}
