#ifndef DIRPROCESSOR_H
#define DIRPROCESSOR_H

#include <QObject>
#include "voskprocessor.h"
#include "message.h"
#include <QVector>

class DirProcessor :public VoskProcessor
{    
    Q_OBJECT

    Q_PROPERTY(bool running READ getRunning WRITE setRunning NOTIFY runningChanged)

public:
    DirProcessor(QObject * parent = nullptr);
    ~DirProcessor()
    {
        qDebug()<<"DirProcessor daleted!";
    }
    void wav_to_txt_dir();
    void setParams(const QString & InDirPath, const QString & OutDirPath, const QString & modelPath, int samplRate=8000);

    bool getRunning() const;
    void setRunning(bool running);

    QStringList     getFileNamesInDir(const QString & DirPath);
signals:
    void numberOfFiles          (int numOfFiles);
    void numberOfCurrentFile    (int numOfCurFile);
    void nameOfCurrentFile      (const QString & CurFileName);
    void finished();
    void runningChanged();


public slots:
    void stop();



private:
    void            createDir(QString pathDir);

    void            moveFile(QString pathFile,QString pathMove);


    QString  m_InDirPath;

    QString  m_OutDirPath;

    bool m_running;




};

#endif // DIRPROCESSOR_H
