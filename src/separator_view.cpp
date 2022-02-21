#include "separator_view.h"





separator_view::separator_view(QWidget *parent) : QWidget(parent)
{
    setObjectName("MainWindow");
    QCoreApplication::setOrganizationName("CIRI");
    QCoreApplication::setApplicationName("VOSK");

    setWindowTitle("VOSK_v2.0");

    resize(800,600);

    updateModelsInformation();

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
    Q_UNUSED(event)
    qDebug()<<"Stop";
    emit stopAll();
}

void separator_view::processMessage(const Message & msg)
{
    qDebug()<<msg.text();
    m_msgModel.addMessage(msg);
}



void separator_view::finish()
{
    m_pBar->setValue(0);

    m_pBarProcessFile->setValue(0);

    m_pBarProcessFile->setFormat("");

    m_pBar->close();

    m_pBarProcessFile->close();

    m_processThread->quit();

    m_processThread->wait();

    deleteResources();

    m_Start->show();

    m_Stop->close();

    processMessage(Message("Обработка завершена!",Message::ORDINARY));

    qDebug()<<"finish";
}

void separator_view::stop()
{
    emit stopAll();
    processMessage(Message("Остановлено!",Message::ORDINARY));
}

void separator_view::switchTcpControl(int enable)
{
    if(enable)
    {
        m_lblControlPort->show();
        m_spinSetNumPort->show();
        m_StartServer->show();

    }
    else
    {
        m_lblControlPort->close();
        m_spinSetNumPort->close();
        m_StartServer->close();
        m_StopServer->close();
    }
}

void separator_view::startTcpControl()
{
      quint16 port = m_spinSetNumPort->value();
      m_TCPserver.start(QHostAddress::Any,port);
      m_spinSetNumPort->setEnabled(false);
      m_StartServer->close();
      m_StopServer->show();
}

void separator_view::stopTcpControl()
{
    m_TCPserver.stop();
    m_spinSetNumPort->setEnabled(true);
    m_StartServer->show();
    m_StopServer->close();

}

void separator_view::setCurrentModelPath()
{
    QRadioButton * sender = static_cast<QRadioButton*>(QObject::sender());
    m_modelsSettings.currentModelPath = m_modelsSettings.ModelsNames_ModelsPath.value(sender->text());
}

void separator_view::createMainWindow()
{
    m_btnChooseInputCatal    =   new QPushButton(buttonsName.btnChInputCat);
    m_btnChooseOutputCatal   =   new QPushButton(buttonsName.btnChOutCat);
    m_Start                 =   new QPushButton(buttonsName.btnStart);
    m_Stop                  =   new QPushButton(buttonsName.btnStop);
    m_Start->setObjectName  ("Start");
    m_Stop->setObjectName   ("Stop");

    m_lblInputCatal         =   new QLabel(labelsName.lblInputCatal);
    m_lblOutputCatal        =   new QLabel(labelsName.lblOutputCatal);

    m_InputCatalPath        =   new QLineEdit;
    m_OutputCatalPath       =   new QLineEdit;

    m_listViewe              =   new QListView;
    m_listViewe->setViewMode(QListView::ListMode);

    m_listViewe->setModel(&m_msgModel);

    m_pBar                  =   new QProgressBar;
    m_pBarProcessFile       =   new QProgressBar;





    QVBoxLayout* vloutlabel         =   new QVBoxLayout;
    QVBoxLayout* vloutLine          =   new QVBoxLayout;
    QVBoxLayout* vloutButton        =   new QVBoxLayout;
    QHBoxLayout* HloutStartStop     =   new QHBoxLayout;
    QHBoxLayout* HServerSettings   =    new QHBoxLayout;

    QHBoxLayout* hlout              =   new QHBoxLayout;

    vloutlabel->addWidget(m_lblInputCatal);
    vloutlabel->addWidget(m_lblOutputCatal);

    vloutLine->addWidget(m_InputCatalPath);
    vloutLine->addWidget(m_OutputCatalPath);


    vloutButton->addWidget(m_btnChooseInputCatal);
    vloutButton->addWidget(m_btnChooseOutputCatal);


    hlout->addLayout(vloutlabel);
    hlout->addLayout(vloutLine);
    hlout->addLayout(vloutButton);


    m_Start->setMinimumWidth(100);
    m_Stop->setMinimumWidth(100);

    m_lblControlPort        =   new QLabel("Порт управления: ");
    m_spinSetNumPort        =   new QSpinBox;
    m_UseTcpServerCkeck     =   new QCheckBox("Использовать сетевое управление");
    m_StartServer           =   new QPushButton("Старт сервер");
    m_StopServer            =   new QPushButton("Стоп сервер");


    m_spinSetNumPort->setMinimum(100);
    m_spinSetNumPort->setMaximum(65535);

    HServerSettings->addWidget(m_UseTcpServerCkeck,1,Qt::AlignLeft);
    HServerSettings->addWidget(m_lblControlPort,1,Qt::AlignLeft);
    HServerSettings->addWidget(m_spinSetNumPort,1,Qt::AlignLeft);
    HServerSettings->addWidget(m_StopServer,1,Qt::AlignLeft);
    HServerSettings->addWidget(m_StartServer,1,Qt::AlignLeft);



    HloutStartStop->addLayout(HServerSettings);
    HloutStartStop->addWidget(m_Start,1,Qt::AlignRight);
    HloutStartStop->addWidget(m_Stop,1,Qt::AlignRight);



    QVBoxLayout* Mainhlout = new QVBoxLayout;
    Mainhlout->addWidget(createModelLout());
    Mainhlout->addLayout(hlout);
    Mainhlout->addSpacing(5);
    Mainhlout->addLayout(HloutStartStop);
    Mainhlout->addSpacing(5);
    Mainhlout->addWidget(m_listViewe);
    Mainhlout->addWidget(m_pBar);
    Mainhlout->addWidget(m_pBarProcessFile);


    setLayout(Mainhlout);

    m_pBar->close();

    m_pBarProcessFile->close();

    m_Stop->close();

    m_lblControlPort->close();

    m_spinSetNumPort->close();

    m_StartServer->close();

    m_StopServer->close();


}

QGroupBox *separator_view::createModelLout()
{
    m_ModelsLay = new QGroupBox("Доступные модели");
    QHBoxLayout * modelsLay = new QHBoxLayout;

    QHashIterator<QString, QString> i(m_modelsSettings.ModelsNames_ModelsPath);
     while (i.hasNext()) {
         i.next();
         m_newRbModel = new QRadioButton(i.key());
         connect(m_newRbModel,&QRadioButton::clicked,this,&separator_view::setCurrentModelPath);
         modelsLay->addWidget(m_newRbModel);
     }
    modelsLay->addStretch(1);
    m_ModelsLay->setLayout(modelsLay);
    return m_ModelsLay;

}

void separator_view::setStyle()
{

    qApp->setStyleSheet(    myStyle.MainWindowStyle +
                            myStyle.LineEditeStyle  +
                            myStyle.PushButtonStyle +
                            myStyle.ProgressBarStyle+
                            myStyle.ProgressBarChunkStyle+
                            myStyle.TextEditeStyle  +
                            myStyle.PushButtonStyleTracked+
                            myStyle.PushButtonStylePressed+
                            myStyle.PushButtonStyleStart+
                            myStyle.PushButtonStyleStartTracked+
                            myStyle.PushButtonStyleStartPressed+
                            myStyle.PushButtonStyleStop+
                            myStyle.PushButtonStyleStopTracked+
                            myStyle.PushButtonStyleStopPressed+
                            myStyle.ListViewStyle
                            );


}

void separator_view::connectButtonsWhithFunctions()
{
    connect(m_Start,                &QPushButton::clicked,      this,   &separator_view::func_MainProcess);
    connect(m_Stop,                 &QPushButton::clicked,      this,   &separator_view::stop);
    connect(m_btnChooseInputCatal,   &QPushButton::clicked,      this,   &separator_view::func_selectDirInputCatal);
    connect(m_btnChooseOutputCatal,  &QPushButton::clicked,      this,   &separator_view::func_selectDirOutputCatal);
    connect(m_UseTcpServerCkeck,    &QCheckBox::stateChanged,   this,   &separator_view::switchTcpControl);
    connect(m_StartServer,          &QPushButton::clicked,      this,   &separator_view::startTcpControl);
    connect(m_StopServer,           &QPushButton::clicked,      this,   &separator_view::stopTcpControl);
    connect(&m_TCPserver,           &TCPServer::msg,            this,   &separator_view::processMessage);


}

void separator_view::connectProcessorWithViewAndNewThread()
{
    connect(m_processThread,      &QThread::started,                      processor,                  &DirProcessor::wav_to_txt_dir);

    connect(m_processThread,      &QThread::destroyed,[](){
        qDebug()<<"QThread destroyed!";
    });
    connect(this,               &separator_view::stopAll,               processor,                  &DirProcessor::stop                  ,Qt::DirectConnection);
    connect(processor,          &DirProcessor::finished,                this,                       &separator_view::finish);
    connect(processor,          &DirProcessor::messageSig,              this,                       &separator_view::processMessage       );
    connect(processor,          &DirProcessor::fileSizeSig,             m_pBarProcessFile,          &QProgressBar::setMaximum);
    connect(processor,          &DirProcessor::bitOfFileSig,            m_pBarProcessFile,          &QProgressBar::setValue);
    connect(processor,          &DirProcessor::nameOfCurrentFile,       m_pBarProcessFile,          &QProgressBar::setFormat);
    connect(processor,          &DirProcessor::numberOfCurrentFile,     m_pBar,                     &QProgressBar::setValue);
    connect(processor,          &DirProcessor::numberOfFiles,           m_pBar,                     &QProgressBar::setMaximum);
}



separator_view::params separator_view::readParams()
{
    params m_ParamsForLib;
    m_ParamsForLib.InputCatalPath     =   m_InputCatalPath->text().trimmed();
    m_ParamsForLib.OutputCatalPath    =   m_OutputCatalPath->text().trimmed();

    m_ParamsForLib.CurrentModelPath          =  m_modelsSettings.currentModelPath ;

    if(m_ParamsForLib.InputCatalPath.size()==0||m_ParamsForLib.CurrentModelPath==0)
        throw QString("Error readParams():Invalid read params! Ну казан путь к входному каталогу или модели данных!");

    return m_ParamsForLib;

}

void separator_view::func_MainProcess()
{
    if(m_processThread!=nullptr)
    {
        return;
    }

    m_msgModel.clear();


    m_ParamsForLib = readParams();

    saveParams(m_ParamsForLib);

    processor            = new DirProcessor;

    m_processThread      = new QThread;

    processor->setParams(m_ParamsForLib.InputCatalPath,m_ParamsForLib.OutputCatalPath,m_ParamsForLib.CurrentModelPath);

    if(processor->getFileNamesInDir(m_ParamsForLib.InputCatalPath).size()==0)
    {
        this->processMessage(Message("Отсутствуют файлы для обработки!",Message::ORDINARY));

        deleteResources();

        return;
    }

    connectProcessorWithViewAndNewThread();

    processor->moveToThread(m_processThread);

    m_processThread->start();

    m_pBar->show();

    m_pBarProcessFile->show();

    m_Start->close();

    m_Stop->show();
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


void separator_view::deleteResources()
{
    delete processor;

    delete m_processThread;

    processor = nullptr;

    m_processThread = nullptr;
}


QString separator_view::selectDir(QString nameDir)
{
    QString dir = QFileDialog::getExistingDirectory(this,nameDir,
                                                    QDir::rootPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    return dir;
}

void separator_view::saveParams(separator_view::params m_params)
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    settings.setValue("InputCatalPath",m_params.InputCatalPath);
    settings.setValue("OutputCatalPath",m_params.OutputCatalPath);
    settings.setValue("ModelPath",m_params.CurrentModelPath);
}

separator_view::params separator_view::readOldParams()
{
    params out;
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    out.InputCatalPath=settings.value("InputCatalPath").toString();
    out.CurrentModelPath=settings.value("ModelPath").toString();
    out.OutputCatalPath=settings.value("OutputCatalPath").toString();
    return out;
}

void separator_view::setParamsOnForm(separator_view::params onForm)
{
    m_InputCatalPath->setText(onForm.InputCatalPath);
    m_OutputCatalPath->setText(onForm.OutputCatalPath);
    //m_ModelPath->setText(onForm.ModelPath);
}

QStringList separator_view::getAvailableModels(const QString & dir)
{
    QDir modelsDir(dir);
    modelsDir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    return modelsDir.entryList();
}

void separator_view::updateModelsInformation()
{
   QStringList modelsNames = getAvailableModels(m_modelsSettings.PathToModelsDir);
    for(int i=0;i< modelsNames.size();i++)
    {
        m_modelsSettings.ModelsNames_ModelsPath.insert(modelsNames.at(i),m_modelsSettings.PathToModelsDir+"/"+modelsNames.at(i)+"/"+"model");

    }

}

