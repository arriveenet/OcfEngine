#include "AudioEngineTest.h"
#include "ui/UICheckBox.h"

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
    float leftX = 300.0f;
    float leftX2 = 550.0f;

    Viewport* root = getRoot();

    m_audioFileName = "audio/Canon in D Major.mp3";
    //m_audioFileName = "audio/タイフーンパレード.wav";
    auto fileNameLabel = Label::createWithBMFont("MS Gothic.fnt", m_audioFileName);
    fileNameLabel->setPosition(glm::vec2(leftX, 150));
    root->addChild(fileNameLabel);

    auto button1 = Button::create();
    button1->setText("Play");
    button1->setPosition(glm::vec2(leftX, 300));
    button1->setOnAction([=]() {
         m_audioID = AudioEngine::play(m_audioFileName, true);
        });
    root->addChild(button1);

    auto stopButton = Button::create();
    stopButton->setText("Stop");
    stopButton->setPosition(glm::vec2(leftX2, 300));
    stopButton->setOnAction([=]() {
        AudioEngine::stop(m_audioID);
        });
    root->addChild(stopButton);

    auto pauseButton = Button::create();
    pauseButton->setText("Pause");
    pauseButton->setPosition(glm::vec2(leftX, 380));
    pauseButton->setOnAction([=]() {
        AudioEngine::pause(m_audioID);
        });
    root->addChild(pauseButton);

    auto resumeButton = Button::create();
    resumeButton->setText("Resume");
    resumeButton->setPosition(glm::vec2(leftX2, 380));
    resumeButton->setOnAction([=]() {
        AudioEngine::resume(m_audioID);
        });
    root->addChild(resumeButton);

    auto loopCheckbox = CheckBox::create("Loop");
    loopCheckbox->setPosition(glm::vec2(leftX, 250));
    loopCheckbox->setSelected(true);
    loopCheckbox->setOnAction([=]() {
        const bool isSelected = loopCheckbox->isSelected();
        AudioEngine::setLoop(m_audioID, isSelected);
        });
    root->addChild(loopCheckbox);


    m_volumeLabel = Label::create("Volume: 1.0");
    m_volumeLabel->setPosition(glm::vec2(leftX, 450));
    root->addChild(m_volumeLabel);

    Slider* volumeSlider = Slider::create(400, 10);
    volumeSlider->setPosition(glm::vec2(leftX + 100, 500));
    volumeSlider->setValue(100);
    volumeSlider->setOnValueChangedCallback([=](int value) {
        const float volume = static_cast<float>(value) / 100.0f;
        AudioEngine::setVolume(m_audioID, volume);
        char str[128];
        snprintf(str, sizeof(str), "Volume: %f", volume);
        m_volumeLabel->setString(str);
        });
    root->addChild(volumeSlider);

    return TestCase::init();
}

void AudioEngineTest::onExit()
{
    AudioEngine::stop(m_audioID);

    Scene::onExit();
}
