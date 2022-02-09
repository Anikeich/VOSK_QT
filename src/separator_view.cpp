#include "separator_view.h"





separator_view::separator_view(QWidget *parent) : QWidget(parent), flagStop(false)
{
    QCoreApplication::setOrganizationName("CIRI");
    QCoreApplication::setApplicationName("VOSK");

    setWindowTitle("VOSK");
    resize(400,100);

    createMainWindow();
    setStyle();
    connectButtonsWhithFunctions();
    setParamsOnForm(readOldParams());






}

void separator_view::setArg(int count, char *arg[])
{
    if(count==1)
        return;

    m_InputCatalPath->setText(QString(arg[1]));



}


separator_view::~separator_view()
{

}

void separator_view::closeEvent(QCloseEvent *event)
{
    flagStop = true;
    emit stopAll();
}

void separator_view::pocessErrors()
{

}

void separator_view::createMainWindow()
{
    m_btnChuseInputCatal    =   new QPushButton(buttonsName.btnChInputCat);
    m_btnChuseOutputCatal   =   new QPushButton(buttonsName.btnChOutCat);
    m_btnChuseModelPath     =   new QPushButton(buttonsName.btnChModel);
    m_btnMainProcess        =   new QPushButton(buttonsName.btnMainProc);

    m_lblInputCatal         =   new QLabel(labelsName.lblInputCatal);
    m_lblOutputCatal        =   new QLabel(labelsName.lblOutputCatal);
    m_lblModelPath          =   new QLabel(labelsName.lblModelPath);

    m_InputCatalPath        =   new QLineEdit;
    m_OutputCatalPath       =   new QLineEdit;
    m_ModelPath             =   new QLineEdit;

    m_textEdit              =   new QTextEdit;

    m_pBar                  =   new QProgressBar;
    m_pBarProcessFile       =   new QProgressBar;

    QVBoxLayout* vloutlabel =   new QVBoxLayout;
    QVBoxLayout* vloutLine  =   new QVBoxLayout;
    QVBoxLayout* vloutButton=   new QVBoxLayout;
    QHBoxLayout* hlout      =   new QHBoxLayout;

    vloutlabel->addWidget(m_lblInputCatal);
    vloutlabel->addWidget(m_lblOutputCatal);
    vloutlabel->addWidget(m_lblModelPath);
    vloutlabel->addSpacing(30);

    vloutLine->addWidget(m_InputCatalPath);
    vloutLine->addWidget(m_OutputCatalPath);
    vloutLine->addWidget(m_ModelPath);
    vloutLine->addSpacing(30);


    vloutButton->addWidget(m_btnChuseInputCatal);
    vloutButton->addWidget(m_btnChuseOutputCatal);
    vloutButton->addWidget(m_btnChuseModelPath);
    vloutButton->addWidget(m_btnMainProcess);


    hlout->addLayout(vloutlabel);
    hlout->addLayout(vloutLine);
    hlout->addLayout(vloutButton);

    QVBoxLayout* Mainhlout = new QVBoxLayout;
    Mainhlout->addLayout(hlout);
    Mainhlout->addWidget(m_textEdit);
    Mainhlout->addWidget(m_pBar);
    Mainhlout->addWidget(m_pBarProcessFile);



    setLayout(Mainhlout);
    m_pBar->close();
    m_pBarProcessFile->close();

}

void separator_view::setStyle()
{
    //    setWindowFlags(Qt::FramelessWindowHint);
    //    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(myStyle.mainWindowStyle);

    setMouseTracking(true);

    m_pBar                  ->setStyleSheet(myStyle.progressBarStyle);
    m_pBarProcessFile       ->setStyleSheet(myStyle.progressBarStyle);

    m_btnChuseInputCatal    ->setStyleSheet(myStyle.selectButtonStyle);
    m_btnChuseOutputCatal   ->setStyleSheet(myStyle.selectButtonStyle);
    m_btnChuseModelPath     ->setStyleSheet(myStyle.selectButtonStyle);
    m_btnMainProcess        ->setStyleSheet(myStyle.mainProcButtonStyle);

    m_OutputCatalPath       ->setStyleSheet(myStyle.lineEditStyle);
    m_InputCatalPath        ->setStyleSheet(myStyle.lineEditStyle);
    m_ModelPath             ->setStyleSheet(myStyle.lineEditStyle);

    m_lblInputCatal         ->setStyleSheet(myStyle.labelStyle);
    m_lblOutputCatal        ->setStyleSheet(myStyle.labelStyle);
    m_lblModelPath          ->setStyleSheet(myStyle.labelStyle);

    m_textEdit              ->setStyleSheet(myStyle.textEditStyle);

}

void separator_view::connectButtonsWhithFunctions()
{
    connect(m_btnMainProcess,&QPushButton::clicked,this,&separator_view::func_MainProcess);
    connect(m_btnChuseInputCatal,&QPushButton::clicked,this,&separator_view::func_selectDirInputCatal);
    connect(m_btnChuseOutputCatal,&QPushButton::clicked,this,&separator_view::func_selectDirOutputCatal);
    connect(m_btnChuseModelPath,&QPushButton::clicked,this,&separator_view::func_selectDirModelCatal);
}

separator_view::params separator_view::readParams()
{
    params m_ParamsForLib;
    m_ParamsForLib.InputCatalPath     =   m_InputCatalPath->text().trimmed();
    m_ParamsForLib.OutputCatalPath    =   m_OutputCatalPath->text().trimmed();
    m_ParamsForLib.ModelPath          =   m_ModelPath->text().trimmed();

    if(m_ParamsForLib.InputCatalPath.size()==0||m_ParamsForLib.ModelPath==0)
        throw QString("Error readParams():Invalid read params! Ну казан путь к входному каталогу или модели данных!");

    return m_ParamsForLib;

}

void separator_view::func_MainProcess()
{
    try {

        m_ParamsForLib = readParams();

        saveParams(m_ParamsForLib);

        QStringList fileNames = readFileNameInCatalog(m_ParamsForLib.InputCatalPath);

        processDir(fileNames);

    } catch (const QString & out) {
        showError(out);
    }

    m_pBar->setValue(m_pBar->maximum());
    m_pBar->close();
    m_pBarProcessFile->close();

}

void separator_view::showError(const QString err)
{
    QMessageBox msgBox;
    msgBox.setStyleSheet(myStyle.mainWindowStyle);
    msgBox.setIcon(QMessageBox::Warning);


    msgBox.setText(err);
    msgBox.exec();
}

void separator_view::func_selectDirInputCatal()
{
    QString path = selectDir(dialogsName.chuseInputDir);
    if(path!="")
        m_InputCatalPath->setText(path);
}

void separator_view::func_selectDirOutputCatal()
{
    QString path = selectDir(dialogsName.chuseOutDir);
    if(path!="")
        m_OutputCatalPath->setText(path);

}

void separator_view::func_selectDirModelCatal()
{
    QString path = selectDir(dialogsName.chuseModel);
    if(path!="")
        m_ModelPath->setText(path);
}

QString separator_view::selectDir(QString nameDir)
{
    QString dir = QFileDialog::getExistingDirectory(this,nameDir,
                                                    QDir::rootPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    return dir;
}

QStringList separator_view::readFileNameInCatalog(QString catalDir)
{
    QStringList namesFiles;
    QDir messageDir(catalDir);
    namesFiles=(messageDir.entryList(QStringList("*.*"),QDir::Files,QDir::NoSort));//создание листа с названиями файлов
    return namesFiles;
}

void separator_view::saveParams(separator_view::params m_params)
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    settings.setValue("InputCatalPath",m_params.InputCatalPath);
    settings.setValue("OutputCatalPath",m_params.OutputCatalPath);
    settings.setValue("ModelPath",m_params.ModelPath);
}

separator_view::params separator_view::readOldParams()
{
    params out;
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    out.InputCatalPath=settings.value("InputCatalPath").toString();
    out.ModelPath=settings.value("ModelPath").toString();
    out.OutputCatalPath=settings.value("OutputCatalPath").toString();
    return out;
}

void separator_view::setParamsOnForm(separator_view::params onForm)
{
    m_InputCatalPath->setText(onForm.InputCatalPath);
    m_OutputCatalPath->setText(onForm.OutputCatalPath);
    m_ModelPath->setText(onForm.ModelPath);
}

void separator_view::processDir(QStringList fileNames)
{
    int countFiles = fileNames.size();
    if(countFiles==0)
        throw QString("No Files in dir:"+m_ParamsForLib.InputCatalPath);

    m_pBar->setMaximum(countFiles);
    m_pBar->setValue(0);
    m_pBar->show();
    m_textEdit->clear();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    QString outCatalPath    = m_ParamsForLib.OutputCatalPath;

    if(outCatalPath=="")
        outCatalPath = m_ParamsForLib.InputCatalPath;

    for(int i=0;i<countFiles;i++)
    {

        QString     fullpathFileIn      =   m_ParamsForLib.InputCatalPath+"/"+fileNames.at(i);
        QString     language            =   "Processed";
        QString     moveDir             =   outCatalPath    +"/"+language;
        QString     outTxtDir           =   outCatalPath    +"/TXT";
        QString     fullpathFileOut     =   moveDir         +"/"+fileNames.at(i);

        createDir(moveDir);
        createDir(outTxtDir);

        QString txtNameFile = QString(fileNames.at(i)).replace(".wav",".txt");

        try {

            if(flagStop==true)
               return;

            m_pBar->setValue(i);

            processFile(fullpathFileIn,fullpathFileOut);

            m_textEdit->append("Файл: "+ fullpathFileIn+" язык:"+ language);

            m_textEdit->append("Перемещен: "+ fullpathFileOut);
        }
        catch (const QString & err)
        {
            m_pBar->close();

            m_pBarProcessFile->close();

            throw;
        }

    }

    m_textEdit->append("Обработка завершена! Количество файлов: "+QString::number(countFiles));    
}

void separator_view::processFile(const QString &filePathIn, const QString &filePathOut)
{
    VoskProcessor voskProc(m_ParamsForLib.ModelPath);

    voskProc.setSampleRate(8000.0);

    voskProc.init();

    connect(&voskProc,&VoskProcessor::processMaxValue,m_pBarProcessFile,&QProgressBar::setMaximum);

    connect(&voskProc,&VoskProcessor::prosessValue,m_pBarProcessFile,&QProgressBar::setValue);

    connect(this,&separator_view::stopAll,&voskProc,&VoskProcessor::stop);

    m_pBarProcessFile->setTextVisible(true);

    m_pBarProcessFile->setFormat(filePathOut);

    m_pBarProcessFile->setAlignment(Qt::AlignCenter);

    m_pBarProcessFile->show();

    voskProc.wav_to_txt(filePathIn,filePathOut);

    m_pBarProcessFile->close();

    voskProc.free();

}

void separator_view::createDir(QString pathDir)
{
    QDir InDir(pathDir);
    if(!InDir.exists())
        InDir.mkpath(pathDir);
}

void separator_view::moveFile(QString pathFile, QString pathMove)
{
    QFile file(pathFile);
    file.rename(pathFile,pathMove);
}

void separator_view::writeTextToFile(const QString & txt, const QString fileName)
{
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly))
        throw QString("File "+fileName+"is not writable!");

    file.write(txt.toStdString().data(),txt.size());

}

QByteArray separator_view::readFile(const QString & pathFile)
{
    QFile file (pathFile);
    if(!file.open(QIODevice::ReadOnly))
        throw QString("File "+pathFile+" no read!");

    QByteArray out = file.readAll();
    file.close();

    return out;
}

QString separator_view::getTextFromVosk(const QString & filePath)
{
    if(!filePath.contains(".wav"))
        throw QString("Файл "+filePath+" не является wav файлом!");

    FILE *wavin;
    char buf[3200];
    int nread, final;
    wavin = fopen(filePath.toStdString().data(), "rb");
    fseek(wavin, 44, SEEK_SET);
    while (!feof(wavin)) {
        nread = fread(buf, 1, sizeof(buf), wavin);
        final = vosk_recognizer_accept_waveform(m_recognizer, buf, nread);

        if (final) {
            printf("%s\n", vosk_recognizer_result(m_recognizer));
        } else {
            printf("%s\n", vosk_recognizer_partial_result(m_recognizer));
        }
        // printf("%s\n", vosk_recognizer_partial_result(m_recognizer));

    }
    QString out (vosk_recognizer_final_result(m_recognizer));
    vosk_recognizer_reset(m_recognizer);

    fclose(wavin);
    return out;
}


