#include "DirProcessor.h"


DirProcessor::DirProcessor(QObject *parent):
    VoskProcessor(parent)
{

}

void DirProcessor::wav_to_txt_dir()
{
    qDebug()<<"START!";

    QStringList fileNames = getFileNamesInDir(m_InDirPath);
    int countFiles = fileNames.size();
    if(countFiles==0)
    {
        emit messageSig(Message("Отсутствуют файлы для обработки!"));
        emit finished();
        return;
    }

    emit this->numberOfFiles(countFiles);

    setRunning(true);

    for(int i=0;i<countFiles;i++)
    {

        QString     InFullWavFileName      =   m_InDirPath+"/"+fileNames.at(i);
        QString     language            =   "Processed";
        QString     moveDir             =   m_OutDirPath    +"/"+language;
        QString     outTxtDir           =   m_OutDirPath    +"/TXT";
        QString     MoveFullWavFileName     =   moveDir         +"/"+fileNames.at(i);

        createDir(moveDir);
        createDir(outTxtDir);

        QString TxtNameFile = QString(fileNames.at(i)).replace(".wav",".txt");

        QString OutFullTxtFileName = outTxtDir + "/"+TxtNameFile;

        if(getRunning()==false)
            return;

        emit this->numberOfCurrentFile(i);
        emit this->nameOfCurrentFile(fileNames.at(i));

        int SampleRate = getFileSamplRate(InFullWavFileName);

        if(SampleRate<0)
        {
            emit messageSig(Message("Не удалось прочитать заголовок wav файла!",Message::NEGATIVE));
            continue;
        }

        setSampleRate(SampleRate);

        if(!init())
        {
            qDebug()<<"invalid initialization!";
            emit messageSig(Message("Не удалось инициализировать модель для распознования!",Message::NEGATIVE));
            emit finished();
            return;
        }

        VoskProcessor::wav_to_txt(InFullWavFileName,OutFullTxtFileName);

        if(getRunning())
        {
            emit this->messageSig(Message("Файл: "+ InFullWavFileName+" обработан!",Message::POSITIVE));
            if(moveFile(InFullWavFileName,MoveFullWavFileName))
            {
                emit this->messageSig(Message("Файл: "+ InFullWavFileName+" перемещен в :"+ MoveFullWavFileName,Message::POSITIVE));
            }
            else
            {
                emit messageSig(Message("Не удалось переместить файл: "+InFullWavFileName+" в "+MoveFullWavFileName+". Возможно файл с таким именем уже присутствует в выходной директории.",Message::NEGATIVE));

            }
        }
        VoskProcessor::free();
    }
    emit finished();
}

void DirProcessor::setParams(const QString & InDirPath, const QString & OutDirPath, const QString &modelPath)
{
    setModelPath(modelPath);
    m_InDirPath     = InDirPath;
    m_OutDirPath    = OutDirPath;
}

void DirProcessor::stop()
{
    VoskProcessor::stop();
    setRunning(false);

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

bool DirProcessor::moveFile(QString pathFile, QString pathMove)
{
    QFile file(pathFile);
    bool result = file.rename(pathFile,pathMove);
    return result;
}

int DirProcessor::getFileSamplRate(const QString &filename)
{
   WavFile file;

   if(!file.open(filename))
       return -1;

  int SampleRate =  file.fileFormat().sampleRate();

  file.close();

   return SampleRate;
}

bool DirProcessor::getRunning() const
{
    return m_running;
}

void DirProcessor::setRunning(bool running)
{
    if(m_running == running)
        return;

    m_running = running;

    VoskProcessor::setRunning(m_running);


    emit runningChanged();
}
