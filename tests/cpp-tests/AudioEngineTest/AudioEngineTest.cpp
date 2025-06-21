#include "AudioEngineTest.h"

USING_NS_OCF;
using namespace ocf::ui;

AudioEngineTest::AudioEngineTest()
    : m_audioID(-1)
    , m_volumeLabel(nullptr)
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

    std::string audioFileName = "audio/Canon in D Major.mp3";
    //std::string audioFileName = "audio/タイフーンパレード.wav";

    auto fileNameLabel = Label::createWithBMFont("MS Gothic.fnt", audioFileName);
    fileNameLabel->setPosition(leftX, 550);
    addChild(fileNameLabel);

    auto button1 = Button::create("ButtonNormal.png", "ButtonActive.png");
    button1->setText("Play");
    button1->setPosition(leftX, 500);
    button1->setOnAction([=]() {
         m_audioID = AudioEngine::play(audioFileName, true);
        });
    addChild(button1);

    auto stopButton = Button::create("ButtonNormal.png", "ButtonActive.png");
    stopButton->setText("Stop");
    stopButton->setPosition(leftX2, 500);
    stopButton->setOnAction([=]() {
        AudioEngine::stop(m_audioID);
        });
    addChild(stopButton);

    auto pauseButton = Button::create("ButtonNormal.png", "ButtonActive.png");
    pauseButton->setText("Pause");
    pauseButton->setPosition(leftX, 400);
    pauseButton->setOnAction([=]() {
        AudioEngine::pause(m_audioID);
        });
    addChild(pauseButton);

    auto resumeButton = Button::create("ButtonNormal.png", "ButtonActive.png");
    resumeButton->setText("Resume");
    resumeButton->setPosition(leftX2, 400);
    resumeButton->setOnAction([=]() {
        AudioEngine::resume(m_audioID);
        });
    addChild(resumeButton);

    m_volumeLabel = Label::create("Volume: 1.0");
    m_volumeLabel->setPosition(leftX, 330);
    addChild(m_volumeLabel);

    Slider* volumeSlider = Slider::create(400, 10);
    volumeSlider->setPosition(leftX + 100, 300);
    volumeSlider->setValue(100);
    volumeSlider->setOnValueChangedCallback([=](int value) {
        const float volume = static_cast<float>(value) / 100.0f;
        AudioEngine::setVolume(m_audioID, volume);

        char str[128];
        snprintf(str, sizeof(str), "Volume: %f", volume);
        m_volumeLabel->setString(str);
        });
    addChild(volumeSlider);

    return true;
}

void AudioEngineTest::onExit()
{
    AudioEngine::stop(m_audioID);

    Node::onExit();
}
