#ifndef __GUI_MESSAGING_H__
#define __GUI_MESSAGING_H__

#include <QObject>

class GuiMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString gui_message READ gui_message WRITE set_gui_message NOTIFY gui_message_changed)

public:
    GuiMessage() { m_message = "3D printing!"; }
 
    void set_gui_message(const QString &msg) {
        if (msg != m_message) {
            m_message = msg;
            emit gui_message_changed();
        }
    }

    QString gui_message() const {
        return m_message;
    }

signals:
    void gui_message_changed();

private:
    QString m_message;
};


#endif /* __GUI_MESSAGING_H__ */