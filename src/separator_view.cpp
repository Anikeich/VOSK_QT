#include "separator_view.h"





separator_view::separator_view(QWidget *parent) : QWidget(parent)
{
    setObjectName("MainWindow");
    QCoreApplication::setOrganizationName("CIRI");
    QCoreApplication::setApplicationName("VOSK");

    setWindowTitle("VOSK");
    resize(800,600);

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
    vloutlabel->addSpacing(40);

    vloutLine->addWidget(m_InputCatalPath);
    vloutLine->addWidget(m_OutputCatalPath);
    vloutLine->addWidget(m_ModelPath);
    vloutLine->addSpacing(40);


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

    qApp->setStyleSheet(    myStyle.MainWindowStyle +
                            myStyle.LineEditeStyle  +
                            myStyle.PushButtonStyle +
                            myStyle.ProgressBarStyle+
                            myStyle.ProgressBarChunkStyle+
                            myStyle.TextEditeStyle  +
                            myStyle.PushButtonStyleTracked+
                            myStyle.PushButtonStylePressed
                        );


}

void separator_view::connectButtonsWhithFunctions()
{
    connect(m_btnMainProcess,       &QPushButton::clicked,  this,   &separator_view::func_MainProcess);
    connect(m_btnChuseInputCatal,   &QPushButton::clicked,  this,   &separator_view::func_selectDirInputCatal);
    connect(m_btnChuseOutputCatal,  &QPushButton::clicked,  this,   &separator_view::func_selectDirOutputCatal);
    connect(m_btnChuseModelPath,    &QPushButton::clicked,  this,   &separator_view::func_selectDirModelCatal);
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

        DirProcessor processor(m_ParamsForLib.ModelPath);

        processor.setSampleRate(8000.0);

        processor.init();

        connect(&processor,&DirProcessor::fileSizeSig,          m_pBarProcessFile,      &QProgressBar::setMaximum);

        connect(&processor,&DirProcessor::bitOfFileSig,         m_pBarProcessFile,      &QProgressBar::setValue);

        connect(&processor,&DirProcessor::numberOfFiles,        m_pBar,                 &QProgressBar::setMaximum);

        connect(&processor,&DirProcessor::numberOfCurrentFile,  m_pBar,                 &QProgressBar::setValue);

        connect(&processor,&DirProcessor::nameOfCurrentFile,    m_pBarProcessFile,      &QProgressBar::setFormat);

        connect(this,&separator_view::stopAll,                  &processor,             &DirProcessor::stop,Qt:: DirectConnection);

        m_pBar->show();

        m_pBarProcessFile->show();

        processor.wav_to_txt_dir(m_ParamsForLib.InputCatalPath,m_ParamsForLib.OutputCatalPath);

        processor.free();

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
    msgBox.setStyleSheet(myStyle.MainWindowStyle);
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

