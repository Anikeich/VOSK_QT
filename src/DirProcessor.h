#ifndef DIRPROCESSOR_H
#define DIRPROCESSOR_H

#include <QObject>
#include "voskprocessor.h"

class DirProcessor :public VoskProcessor
{    
 Q_OBJECT

public:
    DirProcessor(const QString & modelPath="",QObject * parent = nullptr);
    void wav_to_txt_dir(const QString & InDirPath, const QString & OutDirPath = "out");

signals:
    void errorSig                  (const QString & err);
    void numberOfFiles          (int numOfFiles);
    void numberOfCurrentFile    (int numOfCurFile);
    void nameOfCurrentFile      (const QString & CurFileName);
    void MsgOfProcess           (const QString & msg);


public slots:
    void stop();



private:
    QStringList     getFileNamesInDir(const QString & DirPath);
    void            createDir(QString pathDir);
    bool            flagStop;
    void            moveFile(QString pathFile,QString pathMove);


};

#endif // DIRPROCESSOR_H
