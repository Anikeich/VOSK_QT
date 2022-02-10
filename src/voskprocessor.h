#ifndef VOSKPROCESSOR_H
#define VOSKPROCESSOR_H

#include <QObject>
#include <QString>
#include <fstream>
#include <iostream>
#include <cstring>
#include <QDebug>
#include <QFile>
#include <QDir>
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
    void errorSig           (const QString & errorSig );
    void bitOfFileSig       (int val);
    void fileSizeSig        (int val);

public slots:
    void stop();



protected:

    void decode(const QString &filePathIn, const QString &filePathOut);

    void writeResultToFile(const QString & fileName);

    VoskModel       *   m_model                   = nullptr;
    VoskRecognizer  *   m_recognizer              = nullptr;
    QString m_modelPath;
    float m_SampleRate;
    bool stopFlag;
    QString m_filePathIn;
    QString m_filePathOut;


};

#endif // VOSKPROCESSOR_H
