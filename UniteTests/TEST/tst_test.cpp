#include <QtTest>
#include "../../src/voskprocessor.h"
#include "../../VoskLib/vosk_api.h"
#include <QCoreApplication>
#include <QDebug>

// add necessary includes here

class TEST : public QObject
{
    Q_OBJECT

public:
    TEST();
    ~TEST();

private slots:
    void test_case1();

};

TEST::TEST()
{

}

TEST::~TEST()
{

}

void TEST::test_case1()
{
    VoskProcessor  processor;


    QCOMPARE(processor.getModelPath()   ,"");

    const QString modelPath = "../../testVoiceData/LiteTest/model";
    const QString InPathDir = "../../testVoiceData/LiteTest/VoiceData";
    const QString InPathFile = "../../testVoiceData/LiteTest/VoiceData/test.wav";



        processor.setModelPath(modelPath);
        QCOMPARE(processor.getModelPath()   ,modelPath);
        processor.setSampleRate(16000.0);
        processor.init();
        QCOMPARE(processor.getSampleRate()   ,16000.0);

        processor.free();




}

QTEST_APPLESS_MAIN(TEST)

#include "tst_test.moc"
