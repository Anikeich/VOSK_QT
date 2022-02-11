#include "message.h"


Message::Message(const QString &msg, Message::TypeMessage type):m_text(msg),m_msgType(type)
{
    if (QMetaType::type("Message") == QMetaType::UnknownType) {
          qRegisterMetaType<Message>("Message");
       }
}

QString Message::text() const
{
    return m_text;
}

void Message::setText(const QString &text)
{
    m_text = text;
}

Message::TypeMessage Message::msgType() const
{
    return m_msgType;
}

void Message::setMsgType(const TypeMessage &msgType)
{
    m_msgType = msgType;
}
