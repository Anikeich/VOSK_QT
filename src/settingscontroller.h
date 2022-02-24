#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>


class SettingsController : public QObject
{
    Q_OBJECT
public:
    explicit SettingsController(QObject *parent = nullptr);

    void read();
    void save();



    QString         getCurrentModelPath()           const;

    QString         getInputDirPath()               const;

    QString         getOutDirPath()                 const;

    QStringList     availableModels()               const;

    void            setInputDirPath(const QString &InputDirPath);

    void            setOutDirPath(const QString &OutDirPath);

    void            setCurrentModelPath         (const QString & CurrentModelPath);



signals:

public slots:

private:

    QStringList  getAvailableModels          (const QString & dir);
    QString      createFullPathForModel      (QString modelName);

    QString m_CurrentModelPath;
    QString m_InputDirPath;
    QString m_OutDirPath;


    QStringList m_AvailableModels;
    const QString m_modelsDir = "Models";

public:

    static const QString getModelName(const QString & modelPath)
    {
        QStringList param;
        param = modelPath.split("/");
        if(param.size()==3)
            return param.at(1);
        else
            return "";
    }

};

#endif // SETTINGSCONTROLLER_H
