#include "view.h"





view::view(QWidget *parent) : QWidget(parent)
{
    setObjectName("MainWindow");
    QCoreApplication::setOrganizationName("CIRI");
    QCoreApplication::setApplicationName("VOSK");

    setWindowTitle("VOSK_v2.0");

    resize(800,600);

    m_SettingsController.read();

    createMainWindow();

    setStyle();

    connectButtonsWhithFunctions();

    setParamsOnForm();


}

void view::setArg(int count, char *arg[])
{
    if(count==1)
        return;

    m_InputDirPath->setText(QString(arg[1]));

}


view::~view()
{


}

void view::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    qDebug()<<"Stop";
    emit stopAll();
}

void view::processMessage(const Message & msg)
{
    qDebug()<<msg.text();
    m_msgModel.addMessage(msg);
}



void view::finish()
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

void view::stop()
{
    emit stopAll();
    processMessage(Message("Остановлено!",Message::ORDINARY));
}

void view::switchTcpControl(int enable)
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

void view::startTcpControl()
{
    quint16 port = m_spinSetNumPort->value();
    m_TCPserver.start(QHostAddress::Any,port);
    m_spinSetNumPort->setEnabled(false);
    m_StartServer->close();
    m_StopServer->show();
}

void view::stopTcpControl()
{
    m_TCPserver.stop();
    m_spinSetNumPort->setEnabled(true);
    m_StartServer->show();
    m_StopServer->close();

}

void view::setCurrentModelPath()
{
    QRadioButton * sender = static_cast<QRadioButton*>(QObject::sender());

    QString modelName = sender->text();

    QStringList avaluableModelsPath = m_SettingsController.availableModels();

    for(int i=0;i<avaluableModelsPath.size();i++)
    {
        QString avModelName = SettingsController::getModelName(avaluableModelsPath.at(i));

        if(modelName == avModelName)
        {
            m_SettingsController.setCurrentModelPath(avaluableModelsPath.at(i));
            return;
        }
    }

    processMessage(Message("Не существует пути соответствующего выбранной модели!"));
    m_SettingsController.setCurrentModelPath("");


}

void view::createMainWindow()
{
    m_btnChooseInputCatal    =   new QPushButton(buttonsName.btnChInputCat);
    m_btnChooseOutputCatal   =   new QPushButton(buttonsName.btnChOutCat);
    m_Start                 =   new QPushButton(buttonsName.btnStart);
    m_Stop                  =   new QPushButton(buttonsName.btnStop);
    m_Start->setObjectName  ("Start");
    m_Stop->setObjectName   ("Stop");

    m_lblInputCatal         =   new QLabel(labelsName.lblInputCatal);
    m_lblOutputCatal        =   new QLabel(labelsName.lblOutputCatal);

    m_InputDirPath        =   new QLineEdit;
    m_OutputDirPath       =   new QLineEdit;

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

    vloutLine->addWidget(m_InputDirPath);
    vloutLine->addWidget(m_OutputDirPath);


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

QGroupBox *view::createModelLout()
{
    m_ModelsLay = new QGroupBox("Доступные модели");

    QHBoxLayout * modelsLay = new QHBoxLayout;

    QStringList avaluableModels = m_SettingsController.availableModels();

    for(int i=0;i<avaluableModels.size();i++)
    {
        QRadioButton * newRb  = new QRadioButton(SettingsController::getModelName(avaluableModels.at(i)));
        if(avaluableModels.at(i) == m_SettingsController.getCurrentModelPath())
            newRb->setChecked(true);

        connect(newRb,&QRadioButton::clicked,this,&view::setCurrentModelPath);
        m_newRbForModels.push_back(newRb);
        modelsLay->addWidget(newRb);
    }
    modelsLay->addStretch(1);
    m_ModelsLay->setLayout(modelsLay);
    return m_ModelsLay;
}

void view::setStyle()
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

void view::connectButtonsWhithFunctions()
{
    connect(m_Start,                &QPushButton::clicked,      this,   &view::func_MainProcess);
    connect(m_Stop,                 &QPushButton::clicked,      this,   &view::stop);
    connect(m_btnChooseInputCatal,   &QPushButton::clicked,      this,   &view::func_selectDirInputCatal);
    connect(m_btnChooseOutputCatal,  &QPushButton::clicked,      this,   &view::func_selectDirOutputCatal);
    connect(m_UseTcpServerCkeck,    &QCheckBox::stateChanged,   this,   &view::switchTcpControl);
    connect(m_StartServer,          &QPushButton::clicked,      this,   &view::startTcpControl);
    connect(m_StopServer,           &QPushButton::clicked,      this,   &view::stopTcpControl);
    connect(&m_TCPserver,           &TCPServer::msg,            this,   &view::processMessage);


}

void view::connectProcessorWithViewAndNewThread()
{
    connect(m_processThread,      &QThread::started,                      processor,                  &DirProcessor::wav_to_txt_dir);

    connect(m_processThread,      &QThread::destroyed,[](){
        qDebug()<<"QThread destroyed!";
    });
    connect(this,               &view::stopAll,               processor,                  &DirProcessor::stop                  ,Qt::DirectConnection);
    connect(processor,          &DirProcessor::finished,                this,                       &view::finish);
    connect(processor,          &DirProcessor::messageSig,              this,                       &view::processMessage       );
    connect(processor,          &DirProcessor::fileSizeSig,             m_pBarProcessFile,          &QProgressBar::setMaximum);
    connect(processor,          &DirProcessor::bitOfFileSig,            m_pBarProcessFile,          &QProgressBar::setValue);
    connect(processor,          &DirProcessor::nameOfCurrentFile,       m_pBarProcessFile,          &QProgressBar::setFormat);
    connect(processor,          &DirProcessor::numberOfCurrentFile,     m_pBar,                     &QProgressBar::setValue);
    connect(processor,          &DirProcessor::numberOfFiles,           m_pBar,                     &QProgressBar::setMaximum);
}



void view::func_MainProcess()
{
    if(m_processThread!=nullptr)
    {
        return;
    }

    m_msgModel.clear();

    m_SettingsController.setInputDirPath(m_InputDirPath->text());

    m_SettingsController.setOutDirPath(m_OutputDirPath->text());

    m_SettingsController.save();


   processor            = new DirProcessor;

    m_processThread      = new QThread;

    qDebug()<< m_SettingsController.getInputDirPath();
    qDebug()<< m_SettingsController.getOutDirPath();
    qDebug()<< m_SettingsController.getCurrentModelPath();


    processor->setParams(m_SettingsController.getInputDirPath(),m_SettingsController.getOutDirPath(),m_SettingsController.getCurrentModelPath());

    if(processor->getFileNamesInDir(m_SettingsController.getInputDirPath()).size()==0)
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



void view::func_selectDirInputCatal()
{
    QString path = selectDir(dialogsName.chuseInputDir);
    if(path!="")
        m_InputDirPath->setText(path);
}

void view::func_selectDirOutputCatal()
{
    QString path = selectDir(dialogsName.chuseOutDir);
    if(path!="")
        m_OutputDirPath->setText(path);

}


void view::deleteResources()
{
    delete processor;

    delete m_processThread;

    processor = nullptr;

    m_processThread = nullptr;
}


QString view::selectDir(QString nameDir)
{
    QString dir = QFileDialog::getExistingDirectory(this,nameDir,
                                                    QDir::rootPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    return dir;
}




void view::setParamsOnForm()
{
    m_InputDirPath->setText(    m_SettingsController.getInputDirPath()  );
    m_OutputDirPath->setText( m_SettingsController.getOutDirPath()    );
}



