#include "voskprocessor.h"

VoskProcessor::VoskProcessor( const QString &modelPath,QObject *parent): QObject(parent),m_modelPath(modelPath)
{
    setSampleRate(16000.0);

}

void VoskProcessor::wav_to_txt(const QString & filePathIn,const QString & filePathOut)
{
    if(!filePathIn.contains(".wav"))
        throw QString("Файл "+filePathIn+" не является wav файлом!");

    decode(filePathIn,filePathOut);

}

//void VoskProcessor::wav_to_txt(const QStringList & filePaths)
//{
//    int countFiles = filePaths.size();

//    try {
//        if(countFiles==0)
//            throw QString("No Files in dir");

//        for(const auto & filePath:filePaths)
//            wav_to_txt(filePath);


//    } catch (const QString & err) {
//        emit error(err);
//    }


//}

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

    int fileSize = getFileSize(filePathIn.toStdString().data());
    emit processMaxValue(fileSize);

    FILE *wavin;
    char buf[3200];
    int nread, final, position=0;
    wavin = fopen(filePathIn.toStdString().data(), "rb");
    fseek(wavin, 44, SEEK_SET);
    emit prosessValue(0);

    while (!feof(wavin)) {
        nread = fread(buf, 1, sizeof(buf), wavin);
        final = vosk_recognizer_accept_waveform(m_recognizer, buf, nread);
        position +=nread;
        emit prosessValue(position);
        QCoreApplication::processEvents(QEventLoop::AllEvents);


//                if (final) {
//                  //  printf("%s\n", vosk_recognizer_result(m_recognizer));
//                   //result+= QString(vosk_recognizer_result(m_recognizer));
//                } else {
////                    printf("%s\n", vosk_recognizer_partial_result(m_recognizer));
//                   // vosk_recognizer_partial_result(m_recognizer);
//                }

    }


   // std::cout<<strlen(vosk_recognizer_final_result(m_recognizer));

   // std::cout<<vosk_recognizer_final_result(m_recognizer);

    const char * p = vosk_recognizer_final_result(m_recognizer);
    fclose(wavin);

    ofstream outStr;
    outStr.open(filePathOut.toStdString().data());
    if(!outStr.is_open())
        throw QString("Невозможно открыть файл для записи!");

     outStr<<p;

     outStr.close();


}

float VoskProcessor::getSampleRate() const
{
    return m_SampleRate;
}

QString VoskProcessor::getModelPath() const
{
    return m_modelPath;
}
