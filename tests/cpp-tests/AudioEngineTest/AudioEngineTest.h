#pragma once
#include "../BaseTest.h"

class AudioEngineTest : public TestCase {
public:
    AudioEngineTest();
    ~AudioEngineTest();

    bool init() override;

    void onExit() override;

    ocf::AUDIO_ID m_audioID;
    ocf::Label* m_volumeLabel;
    std::string m_audioFileName;
};