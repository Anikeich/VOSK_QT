#include "settingscontroller.h"

SettingsController::SettingsController(QObject *parent) : QObject(parent)
{

}

void SettingsController::read()
{

    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());

    setInputDirPath         (settings.value("InputDirPath").toString());
    setOutDirPath           (settings.value("OutputDirPath").toString());

    m_AvailableModels = getAvailableModels(m_modelsDir);

    qDebug()<<"Прочитано :"<<m_AvailableModels.size()<<"Модели.";
    for(int i=0;i<m_AvailableModels.size();i++)
        qDebug()<<m_AvailableModels.at(i);


    QString currentModelPath = settings.value("CurrentModelPath").toString();

    if(m_AvailableModels.contains(currentModelPath))
        setCurrentModelPath     (currentModelPath);
    else
        setCurrentModelPath("");


}



void SettingsController::save()
{
    QSettings settings(QCoreApplication::organizationName(),QCoreApplication::applicationName());
    settings.setValue("InputDirPath"    ,getInputDirPath());
    settings.setValue("OutputDirPath"   ,getOutDirPath());
    settings.setValue("CurrentModelPath",getCurrentModelPath());

}

QString SettingsController::getCurrentModelPath() const
{
    return m_CurrentModelPath;
}

QString SettingsController::getInputDirPath()   const
{
    return m_InputDirPath;
}

QString SettingsController::getOutDirPath() const
{
    return m_OutDirPath;
}

QStringList SettingsController::availableModels() const
{
        return m_AvailableModels;
}



void SettingsController::setInputDirPath(const QString &InputDirPath)
{
    m_InputDirPath = InputDirPath;
}

void SettingsController::setOutDirPath(const QString &OutDirPath)
{
    m_OutDirPath = OutDirPath;
}


void SettingsController::setCurrentModelPath(const QString &CurrentModelPath)
{
    m_CurrentModelPath = CurrentModelPath;
}



QStringList SettingsController::getAvailableModels(const QString &dir)
{
    QDir modelsDir(dir);
    modelsDir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);

    QStringList modelsList =  modelsDir.entryList();
    for(int i=0;i<modelsList.size();i++)
        modelsList[i]=createFullPathForModel(modelsList[i]);

    return modelsList;
}

QString SettingsController::createFullPathForModel(QString modelName)
{
    return m_modelsDir+"/"+modelName+"/model";
}

