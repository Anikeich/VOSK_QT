#ifndef SEPARATOR_VIEW_H
#define SEPARATOR_VIEW_H

#include <QWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QLabel>
#include <qlayout.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QColor>
#include <QBrush>
#include <QCoreApplication>
#include <QListView>
#include <QStringListModel>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QStackedWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QGroupBox>

#include "VoskLib/vosk_api.h"
#include "DirProcessor.h"
#include "message.h"
#include "mymessaagemodel.h"
#include "tcpserver.h"
#include "settingscontroller.h"





class view : public QWidget
{
    Q_OBJECT

public:
    explicit view(QWidget *parent = nullptr);

    void setArg(int count,char * arg[]);

    ~view()                  override;

    void closeEvent(QCloseEvent *event) override;



signals:
    void stopAll();
    void runningChanged();

public slots:
    void processMessage(const Message & msg);
    void finish();
    void stop();
    void switchTcpControl(int  enable);
    void startTcpControl();
    void stopTcpControl();
    void setCurrentModelPath();



private:



    struct Dialogs
    {
        QString chuseInputDir           = "Выбирите входной каталог";
        QString chuseOutDir             = "Выбирите выходной каталог";
        QString chuseModel              = "Выбирите каталог модели распознавания";


    }dialogsName;





    const struct Buttons
    {
        QString btnChInputCat           = "Выбрать";
        QString btnChOutCat             = "Выбрать";
        QString btnChModel              = "Выбрать";
        QString btnStart                = " Старт ";
        QString btnStop                 = " Стоп  ";

    }buttonsName;


    const struct labels
    {
        QString lblInputCatal           = "<b>Путь к входному каталогу</b>";
        QString lblOutputCatal          = "<b>Путь к выходному каталогу</b>";
        QString lblModelPath            = "<b>Путь к модели распознавания</b>";

    }labelsName;


    const struct ElementsStyle
    {
        QString MainWindowStyle             = "QWidget#MainWindow           { background: #fffcdb }";

        QString ProgressBarStyle            = "QProgressBar                 { border: 2px solid gray    ;    border-radius: 5px  ; background: #55ff7f          ;   width: 20px;    text-align: center}";
        QString ProgressBarChunkStyle       = "QProgressBar::chunk          { background: green}";

        QString PushButtonStyle             = "QPushButton                  { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: #f0e7b7           ;   min-height: 30 }";
        QString PushButtonStylePressed      = "QPushButton:pressed          { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: green             ;   min-height: 30 }";
        QString PushButtonStyleTracked      = "QPushButton:hover            { border: 2px solid #d2d27b ;   border-radius: 5px  ; background: #f0e7b7           ;   min-height: 30 }";

        QString PushButtonStyleStart        = "QPushButton#Start            { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: #55ff7f           ;   min-height: 30 }";
        QString PushButtonStyleStartPressed = "QPushButton#Start:pressed    { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: green             ;   min-height: 30 }";
        QString PushButtonStyleStartTracked = "QPushButton#Start:hover      { border: 2px solid #d2d27b ;   border-radius: 5px  ; background: #55ff7f           ;   min-height: 30 }";

        QString PushButtonStyleStop        = "QPushButton#Stop              { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: #aa5500           ;   min-height: 30 }";
        QString PushButtonStyleStopPressed = "QPushButton#Stop:pressed      { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: rad               ;   min-height: 30 }";
        QString PushButtonStyleStopTracked = "QPushButton#Stop:hover        { border: 2px solid #d2d27b ;   border-radius: 5px  ; background: #aa5500           ;   min-height: 30 }";

        QString LineEditeStyle              = "QLineEdit                    { border: 2px solid #00aa00 ;   border-radius: 5px  ; background: #fffcdb           ;   min-height: 30}";
        QString TextEditeStyle              = "QTextEdit                    { border: 5px solid #d2d27b ;   border-radius: 5px  ; background-color: #fffcdb     ;   min-height: 30}";
        QString ListViewStyle               = "QListView                    { border: 5px solid #d2d27b ;   border-radius: 5px  ; background-color: #fffcdb     ;   min-height: 30}";



    }myStyle;

    void            createMainWindow();
    QGroupBox *     createModelLout();

    void            setStyle();
    void            connectButtonsWhithFunctions();
    void            connectProcessorWithViewAndNewThread();
    QString         selectDir(QString nameDir);


    void            setParamsOnForm(); //устанавливает параметры на форму

    void            func_MainProcess();
    void            func_selectDirInputCatal();
    void            func_selectDirOutputCatal();

    QPushButton     *   m_btnChooseInputCatal       = nullptr;
    QPushButton     *   m_btnChooseOutputCatal      = nullptr;
    QPushButton     *   m_Start                     = nullptr;
    QPushButton     *   m_Stop                      = nullptr;
    QPushButton     *   m_StartServer               = nullptr;
    QPushButton     *   m_StopServer                = nullptr;


    QLabel          *   m_lblInputCatal             = nullptr;
    QLabel          *   m_lblOutputCatal            = nullptr;
    QLabel          *   m_lblControlPort            = nullptr;


    QLineEdit       *   m_InputDirPath              = nullptr;
    QLineEdit       *   m_OutputDirPath           = nullptr;

    QListView       *   m_listViewe                 = nullptr;
    MyMessaageModel     m_msgModel;
    QStringList         m_Messages;

    QProgressBar    *   m_pBar                      = nullptr;
    QProgressBar    *   m_pBarProcessFile           = nullptr;


    VoskModel       *   m_model                     = nullptr;
    VoskRecognizer  *   m_recognizer                = nullptr;

    DirProcessor   *   processor                    = nullptr;
    QThread        *   m_processThread              = nullptr;

    QCheckBox      *   m_UseTcpServerCkeck          = nullptr;
    QSpinBox       *   m_spinSetNumPort             = nullptr;

    QGroupBox      *   m_ModelsLay                  = nullptr;

    QList<QRadioButton   *>   m_newRbForModels;

    void deleteResources();

    TCPServer m_TCPserver;

    SettingsController m_SettingsController;





};








#endif // SEPARATOR_VIEW_H
