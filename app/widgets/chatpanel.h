#ifndef CHATPANEL_H
#define CHATPANEL_H

#include <QWidget>

class QTextEdit;
class QLineEdit;
class QPushButton;
class QListWidget;
class QLabel;

class ChatPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPanel(QWidget *parent = nullptr);

signals:
    void sendMessage(const QString &message);
    void attachFile();
    void attachCode();
    void attachMention();

private:
    void setupUI();
    void setupStyle();
    void addWelcomeMessage();
    void addTaskItem(const QString &text, const QString &status);
    void addFileAttachment(const QString &name, const QString &size);

    QLabel *m_headerLabel;
    QListWidget *m_chatList;
    QWidget *m_inputArea;
    QTextEdit *m_inputEdit;
    QPushButton *m_attachBtn;
    QPushButton *m_fileBtn;
    QPushButton *m_codeBtn;
    QPushButton *m_mentionBtn;
    QPushButton *m_sendBtn;
};

#endif // CHATPANEL_H
