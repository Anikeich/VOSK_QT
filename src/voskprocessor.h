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
#include "message.h"

class VoskProcessor:public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning WRITE setRunning NOTIFY runningChanged)

public:
    explicit VoskProcessor(QObject * parent = nullptr);

    ~VoskProcessor(){

     qDebug()<<"VoskProcessor deleted!";

    }

    void wav_to_txt(const QString & filePath, const QString &filePathOut);

    void setModelPath(const QString &modelPath);

    void setSampleRate(float rate);

    bool init();

    void free();

    void reset();


    QString getModelPath() const;

    float getSampleRate() const;

    bool getRunning() const;

    void setRunning(bool value);

signals:
    void messageSig           (const Message & msg);
    void bitOfFileSig       (int val);
    void fileSizeSig        (int val);
    void runningChanged     ();

public slots:
    void stop();



protected:

    void decode(const QString &filePathIn, const QString &filePathOut);

    void writeResultToFile(const QString & fileName);

    VoskModel       *   m_model                   = nullptr;
    VoskRecognizer  *   m_recognizer              = nullptr;
    float m_SampleRate;
    bool m_running;
    QString m_modelPath;
    QString m_filePathIn;
    QString m_filePathOut;


};

#endif // VOSKPROCESSOR_H
