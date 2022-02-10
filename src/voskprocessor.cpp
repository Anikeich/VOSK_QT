#include "voskprocessor.h"

VoskProcessor::VoskProcessor( const QString &modelPath,QObject *parent): QObject(parent),m_modelPath(modelPath),stopFlag(false)
{
    setSampleRate(16000.0);


}

void VoskProcessor::wav_to_txt(const QString & filePathIn,const QString & filePathOut)
{
    if(!filePathIn.contains(".wav"))
        throw QString("Файл "+filePathIn+" не является wav файлом!");

    decode(filePathIn,filePathOut);

}

void VoskProcessor::setModelPath(const QString &modelPath)
{
    m_modelPath = modelPath;
}

void VoskProcessor::setSampleRate(float rate)
{
    m_SampleRate = rate;
}

void VoskProcessor::init()
{
        m_model            = vosk_model_new(m_modelPath.toStdString().data());
        if(m_model==nullptr)
            throw QString ("Укажите верный путь к модели!");

        m_recognizer       = vosk_recognizer_new(m_model, m_SampleRate);
}

void VoskProcessor::free()
{
    vosk_recognizer_free(m_recognizer);
    vosk_model_free(m_model);
}

void VoskProcessor::reset()
{
    vosk_recognizer_reset(m_recognizer);

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

    if(!filePathIn.contains(".wav"))
        throw QString("Файл "+filePathIn+" не является wav файлом!");

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

    while (!feof(wavin)&&(!stopFlag))
    {
        nread = fread(buf, 1, sizeof(buf), wavin);
        vosk_recognizer_accept_waveform(m_recognizer, buf, nread);
        position +=nread;
        emit bitOfFileSig(position);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }

    if(!stopFlag)
    writeResultToFile(m_filePathOut);

    fclose(wavin);
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

float VoskProcessor::getSampleRate() const
{
    return m_SampleRate;
}

void VoskProcessor::stop()
{
    qDebug()<<"Stop VoskProcessor";

   stopFlag=true;
   vosk_recognizer_reset(m_recognizer);
   // deleteLater();
}

QString VoskProcessor::getModelPath() const
{
    return m_modelPath;
}
