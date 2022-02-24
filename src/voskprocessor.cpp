#include "voskprocessor.h"

VoskProcessor::VoskProcessor(QObject *parent): QObject(parent)
{
    setSampleRate(16000.0);


}

void VoskProcessor::wav_to_txt(const QString & filePathIn,const QString & filePathOut)
{

    decode(filePathIn,filePathOut);

    if(getRunning())
    {
        writeResultToFile(filePathOut);
    }

}

void VoskProcessor::setModelPath(const QString &modelPath)
{
    m_modelPath = modelPath;
}

void VoskProcessor::setSampleRate(float rate)
{
    m_SampleRate = rate;
}

bool VoskProcessor::init()
{
    qDebug()<<m_modelPath;
    m_model            = vosk_model_new(m_modelPath.toStdString().data());
    if(m_model==nullptr)
    {
        emit  messageSig(Message ("Укажите верный путь к модели!",Message::NEGATIVE));
        return false;
    }

    m_recognizer       = vosk_recognizer_new(m_model, m_SampleRate);

    qDebug()<<"model init";
    return true;
}

void VoskProcessor::free()
{
    reset();
    vosk_recognizer_free(m_recognizer);
    vosk_model_free(m_model);
    qDebug()<<"model free";
}

void VoskProcessor::reset()
{
    vosk_recognizer_reset(m_recognizer);
    qDebug()<<"model reset";

}

int getFileSize(const char* file_name){
    int _file_size = 0;
    FILE* fd = fopen(file_name, "rb");
    if(fd == NULL){
        _file_size = -1;
    }
    else{
        while(getc(fd) != EOF)
            _file_size++;
        fclose(fd);
    }
    return _file_size;
}

void VoskProcessor::decode(const QString &filePathIn, const QString &filePathOut)
{
    using namespace std;

    qDebug()<<"SatrtDecode";

    if(!filePathIn.contains(".wav"))
    {
        emit Message("Файл "+filePathIn+" не является wav файлом!",Message::NEGATIVE);
        return;
    }

    m_filePathIn=filePathIn;

    m_filePathOut=filePathOut;

    int fileSize = getFileSize(m_filePathIn.toStdString().data());

    emit fileSizeSig(fileSize);

    FILE *wavin;

    char buf[3200];

    int nread,position=0;

    wavin = fopen(m_filePathIn.toStdString().data(), "rb");

    fseek(wavin, 44, SEEK_SET);

    emit bitOfFileSig(0);

    while (!feof(wavin)&&(m_running))
    {
        nread = fread(buf, 1, sizeof(buf), wavin);

        vosk_recognizer_accept_waveform(m_recognizer, buf, nread);

        position +=nread;

        emit bitOfFileSig(position);        
    }   

    fclose(wavin);

    qDebug()<<"FinishDecode";


}

void VoskProcessor::writeResultToFile(const QString &fileName)
{
    using namespace std;

    const char * p = vosk_recognizer_final_result(m_recognizer);

    ofstream outStr;

    outStr.open(fileName.toStdString().data());

    if(!outStr.is_open())
        throw QString("Невозможно открыть файл: "+fileName+" для записи!");

    outStr<<p;

    outStr.close();
}

bool VoskProcessor::getRunning() const
{
    return m_running;
}

void VoskProcessor::setRunning(bool value)
{
    if(m_running == value)
        return;

    m_running = value;

    emit runningChanged();
}

float VoskProcessor::getSampleRate() const
{
    return m_SampleRate;
}

void VoskProcessor::stop()
{
    qDebug()<<"Stop VoskProcessor";

    setRunning(false);
}

QString VoskProcessor::getModelPath() const
{
    return m_modelPath;
}
