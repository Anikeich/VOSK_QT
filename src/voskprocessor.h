#ifndef VOSKPROCESSOR_H
#define VOSKPROCESSOR_H

#include <QObject>
#include <QString>
#include <fstream>
#include <iostream>
#include <cstring>
#include <QDebug>
#include <QCoreApplication>
#include "../VoskLib/vosk_api.h"
class VoskProcessor:public QObject
{
    Q_OBJECT

public:
    explicit VoskProcessor(const QString & modelPath="",QObject * parent = nullptr);
    ~VoskProcessor(){}
    void wav_to_txt(const QString & filePath, const QString &filePathOut);

    void setModelPath(const QString &modelPath);

    void setSampleRate(float rate);

    void init();

    void free();

    void reset();


    QString getModelPath() const;

    float getSampleRate() const;

signals:
    void error(const QString & error );
    void prosessValue   (int val);
    void processMaxValue(int val);



private:

    void decode(const QString &filePathIn, const QString &filePathOut);
    VoskModel       *   m_model                   = nullptr;
    VoskRecognizer  *   m_recognizer              = nullptr;
    QString m_modelPath;
    float m_SampleRate;


};

#endif // VOSKPROCESSOR_H
