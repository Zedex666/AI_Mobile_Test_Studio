#ifndef BOTTOMPANEL_H
#define BOTTOMPANEL_H

#include <QWidget>

class QTabWidget;
class QTextEdit;

class BottomPanel : public QWidget
{
    Q_OBJECT

public:
    explicit BottomPanel(QWidget *parent = nullptr);

    enum TabIndex {
        LogTab,
        TerminalTab,
        ResultTab
    };

    void appendLog(const QString &text);
    void appendTerminal(const QString &text);
    void appendResult(const QString &text);

    void setActiveTab(TabIndex tab);

public slots:
    void onLogEmitted(const QString &text);

private:
    void setupUI();
    void setupStyle();

    QTabWidget *m_tabWidget;
    QTextEdit *m_logEdit;
    QTextEdit *m_terminalEdit;
    QTextEdit *m_resultEdit;
};

#endif // BOTTOMPANEL_H
