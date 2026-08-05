#include "NameEntryWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

NameEntryWidget::NameEntryWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QWidget *card = new QWidget(this);
    card->setFixedSize(500, 220);
    card->setStyleSheet(R"(
        QWidget {
            background-color: rgba(20, 18, 15, 240);
            border: 2px solid #8c6d46;
            border-radius: 8px;
        }
        QLabel {
            color: #f0c050;
            font-size: 18px;
            font-weight: bold;
            border: none;
        }
        QLineEdit {
            background-color: #120f0c;
            color: #ffffff;
            border: 1px solid #5a4b3c;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 16px;
        }
        QLineEdit:focus {
            border: 1px solid #f0c050;
        }
        QPushButton {
            background-color: #3d3126;
            color: #e0e0e0;
            border: 1px solid #8c6d46;
            border-radius: 4px;
            padding: 8px 20px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5a4b3c;
            color: #ffffff;
        }
    )");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(25, 20, 25, 20);
    cardLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("Enter Your Title & Name:", card);
    titleLabel->setAlignment(Qt::AlignCenter);

    m_nameInput = new QLineEdit(card);
    m_nameInput->setPlaceholderText("Sir...");
    m_nameInput->setMaxLength(24);

    m_confirmBtn = new QPushButton("Begin Journey", card);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(m_nameInput);
    cardLayout->addWidget(m_confirmBtn, 0, Qt::AlignRight);

    mainLayout->addWidget(card, 0, Qt::AlignCenter);

    auto submitName = [this]() {
        QString name = m_nameInput->text().trimmed();
        if (name.isEmpty()) name = "Sir Knight";
        emit nameConfirmed(name);
    };

    connect(m_confirmBtn, &QPushButton::clicked, this, submitName);
    connect(m_nameInput, &QLineEdit::returnPressed, this, submitName);
}