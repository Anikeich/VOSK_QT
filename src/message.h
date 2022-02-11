#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class Message
{

public:

    enum TypeMessage
    {
        POSITIVE,
        ORDINARY,
        NEGATIVE
    };

    explicit Message(const QString & msg="", TypeMessage type = Message::ORDINARY);

    QString text() const;
    void setText(const QString &text);

    TypeMessage msgType() const;
    void setMsgType(const TypeMessage &msgType);

signals:

public slots:

private:
    QString m_text;
    TypeMessage m_msgType;
};


#endif // MESSAGE_H
