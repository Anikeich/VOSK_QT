#ifndef MYMESSAAGEMODEL_H
#define MYMESSAAGEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "message.h"
#include <QDebug>
#include <QBrush>
#include <QIcon>

class MyMessaageModel : public QAbstractListModel
{
public:

    enum Roles
    {
        MessageType = Qt::UserRole+1,
        Text
    };

    MyMessaageModel(QObject * parent = nullptr);
    void addMessage(const Message & msg);
    void clear();

    virtual int                     rowCount    (const QModelIndex &parent)             const;
    virtual QVariant                data        (const QModelIndex &index, int role)    const;

private:

    QList<Message> m_messages;

};

#endif // MYMESSAAGEMODEL_H
