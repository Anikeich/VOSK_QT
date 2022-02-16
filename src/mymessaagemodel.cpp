#include "mymessaagemodel.h"

MyMessaageModel::MyMessaageModel(QObject *parent):QAbstractListModel(parent)
{

}

void MyMessaageModel::addMessage(const Message & msg)
{
    roleNames();

    beginInsertRows(QModelIndex(),m_messages.size(),m_messages.size());
    m_messages.append(msg);
    endInsertRows();
    QModelIndex index = createIndex(0,0,static_cast<void*>(0));
    emit dataChanged(index,index);
}

void MyMessaageModel::clear()
{
    m_messages.clear();
}

int MyMessaageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_messages.count();
}

QVariant MyMessaageModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if (index.row() >= m_messages.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return "Msg: "+m_messages.at(index.row()).text()+"\n";

    if (role == Qt::ForegroundRole)
    {
        if(m_messages.at(index.row()).msgType()==Message::NEGATIVE)
            return QBrush(Qt::red);
        else if (m_messages.at(index.row()).msgType()==Message::POSITIVE)
            return QBrush(Qt::darkGreen);
        else if(m_messages.at(index.row()).msgType()==Message::ORDINARY)
            return QBrush(Qt::blue);

    }

    if (role == Qt::DecorationRole)
    {
        if(m_messages.at(index.row()).msgType()==Message::NEGATIVE)
            return QIcon(":/ico/negative.ico");
        else if (m_messages.at(index.row()).msgType()==Message::POSITIVE)
            return QIcon(":/ico/positive.ico");
        else if(m_messages.at(index.row()).msgType()==Message::ORDINARY)
            return QIcon(":/ico/ordinary.ico");

    }



    return QVariant();
}

