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
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QColor>
#include <QBrush>
#include <QCoreApplication>
#include <QTextEdit>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <fstream>

#include "VoskLib/vosk_api.h"
#include "voskprocessor.h"



class separator_view : public QWidget
{
    Q_OBJECT
public:
    explicit separator_view(QWidget *parent = nullptr);

    void setArg(int count,char * arg[]);
    void testprogressbar()
    {
        m_pBar->show();
        m_pBar->setMaximum(100);
        m_pBar->setValue(0);
        for(int i=0;i<100;i++)
        {
            m_pBar->setValue(i);
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            QThread::sleep(2);
        }
        m_pBar->setValue(m_pBar->maximum());
        m_pBar->close();
    }
     ~separator_view() override;

    void closeEvent(QCloseEvent *event) override;

signals:
    void stopAll();

public slots:
    void pocessErrors();

private:

    struct params
    {
        QString  InputCatalPath;
        QString  OutputCatalPath;
        QString  ModelPath;

    }m_ParamsForLib;


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
        QString btnMainProc             = "Обработать";
    }buttonsName;


    const struct labels
    {
        QString lblInputCatal           = "Путь ко входному каталогу";
        QString lblOutputCatal          = "Путь к выходному каталогу";
        QString lblModelPath            = "Путь к модели распозначания";

    }labelsName;


    const struct ElementsStyle
    {
        QString mainWindowStyle         = "background-color:moccasin;";
        QString progressBarStyle        = "border:2px solid gray; border-radius: 5px; background-color:lightblue; width: 20px; text-align: center";
        QString selectButtonStyle        ="border:2px solid gray; border-radius: 5px; color:blue";
        QString mainProcButtonStyle     = "border:2px solid gray; border-radius: 5px; color:green";
        QString lineEditStyle           = "border:2px solid gray; border-radius: 5px; color:green";
        QString textEditStyle           = "border:2px solid gray; border-radius: 5px; color:green";
        QString labelStyle              = "border:2px solid gray; border-radius: 5px; color:green";


    }myStyle;

    void            createMainWindow();
    void            setStyle();
    void            showError(const QString err);
    void            connectButtonsWhithFunctions();
    QString         selectDir(QString nameDir);
    QStringList     readFileNameInCatalog(QString catalDir);

    void            saveParams(params m_params);    //сохраняет параметры в файл настроек
    params          readParams();                   //читает параметры с формы
    params          readOldParams();                //читает параметры из файла настроек
    void            setParamsOnForm(params onForm); //устанавливает параметры на форму
    void            processDir(QStringList fileNames);
    void            processFile(const QString & filePathIn, const QString & filePathOut);


    void            createDir(QString pathDir);
    void            moveFile(QString pathFile,QString pathMove);
    void            writeTextToFile(const QString & txt, const QString fileName);
    QByteArray      readFile(const QString &file);
    QString         getTextFromVosk(const QString &filePath);

    void            func_MainProcess();
    void            func_selectDirInputCatal();
    void            func_selectDirOutputCatal();
    void            func_selectDirModelCatal();

    QPushButton     *   m_btnChuseInputCatal    = nullptr;
    QPushButton     *   m_btnChuseOutputCatal   = nullptr;
    QPushButton     *   m_btnChuseModelPath     = nullptr;
    QPushButton     *   m_btnMainProcess        = nullptr;

    QLabel          *   m_lblInputCatal         = nullptr;
    QLabel          *   m_lblOutputCatal        = nullptr;
    QLabel          *   m_lblModelPath          = nullptr;

    QLineEdit       *   m_InputCatalPath        = nullptr;
    QLineEdit       *   m_OutputCatalPath       = nullptr;
    QLineEdit       *   m_ModelPath             = nullptr;

    QTextEdit       *   m_textEdit              = nullptr;
    QProgressBar    *   m_pBar                  = nullptr;
    QProgressBar    *   m_pBarProcessFile       = nullptr;


    VoskModel       *   m_model                   = nullptr;
    VoskRecognizer  *   m_recognizer              = nullptr;

    bool flagStop;


};



#endif // SEPARATOR_VIEW_H
