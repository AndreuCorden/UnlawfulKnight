#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class NameEntryWidget : public QWidget {
    Q_OBJECT
public:
    explicit NameEntryWidget(QWidget *parent = nullptr);

signals:
    void nameConfirmed(const QString &characterName);

private:
    QLineEdit *m_nameInput;
    QPushButton *m_confirmBtn;
};