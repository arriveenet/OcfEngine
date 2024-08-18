#pragma once
#include <string>
#include"OcfEngine.h"

class TestCase : public ocf::Scene {
public:
    TestCase();
    virtual ~TestCase();

    bool init() override;

    void setTestCaseName(std::string name) { m_testCaseName = name; }
    std::string getTestCaseName() const { return m_testCaseName; }

protected:
    ocf::Label* m_pTitleLabel;

private:
    std::string m_testCaseName;
};