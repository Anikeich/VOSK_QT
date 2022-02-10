#include "DirProcessor.h"

DirProcessor::DirProcessor(const QString &modelPath, QObject *parent):VoskProcessor(modelPath,parent),flagStop(false)
{

}

void DirProcessor::wav_to_txt_dir(const QString & InDirPath, const QString & OutDirPath)
{
        QStringList fileNames = getFileNamesInDir(InDirPath);
        int countFiles = fileNames.size();
        if(countFiles==0)
            throw QString("No Files in dir:"+InDirPath);

        emit this->numberOfFiles(countFiles);


        for(int i=0;i<countFiles;i++)
        {
            QString     InFullWavFileName      =   InDirPath+"/"+fileNames.at(i);
            QString     language            =   "Processed";
            QString     moveDir             =   OutDirPath    +"/"+language;
            QString     outTxtDir           =   OutDirPath    +"/TXT";
            QString     MoveFullWavFileName     =   moveDir         +"/"+fileNames.at(i);

            createDir(moveDir);
            createDir(outTxtDir);

            QString TxtNameFile = QString(fileNames.at(i)).replace(".wav",".txt");

            QString OutFullTxtFileName = outTxtDir + "/"+TxtNameFile;

            try {

                if(flagStop==true)
                   return;

                emit this->numberOfCurrentFile(i);
                emit this->nameOfCurrentFile(fileNames.at(i));

                VoskProcessor::wav_to_txt(InFullWavFileName,OutFullTxtFileName);

                VoskProcessor::reset();

                if(!flagStop)
                moveFile(InFullWavFileName,MoveFullWavFileName);

                emit this->MsgOfProcess("Файл: "+ InFullWavFileName+" язык:"+ language);
                emit this->MsgOfProcess("Перемещен: "+ MoveFullWavFileName);
            }
            catch (const QString & err)
            {
                emit this->errorSig(err);
                throw;
            }
        }
}

void DirProcessor::stop()
{
      flagStop = true;
      VoskProcessor::stop();

}

QStringList DirProcessor::getFileNamesInDir(const QString &DirPath)
{
     QStringList namesFiles;
     QDir messageDir(DirPath);
     namesFiles=(messageDir.entryList(QStringList("*.*"),QDir::Files,QDir::NoSort));//создание листа с названиями файлов
     return namesFiles;
}

void DirProcessor::createDir(QString pathDir)
{
    QDir InDir(pathDir);
    if(!InDir.exists())
        InDir.mkpath(pathDir);
}

void DirProcessor::moveFile(QString pathFile, QString pathMove)
{
    QFile file(pathFile);
    file.rename(pathFile,pathMove);
}
