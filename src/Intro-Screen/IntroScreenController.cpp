// IntroScreenController.cpp
#include "IntroScreenController.h"

IntroScreenController::IntroScreenController(QObject *parent) : QObject(parent) {}

IntroScreen* IntroScreenController::getView(QWidget *parent) {
    if (!m_introView) {
        m_introView = new IntroScreen(parent);
        setupConnections();
    }
    return m_introView;
}

void IntroScreenController::startIntroForSlot(int slotId) {
    m_activeSaveSlot = slotId;
}

void IntroScreenController::setupConnections() {
    if (!m_introView) return;

    connect(m_introView, &IntroScreen::backToMenuRequested, this, [this]() {
        emit backToSaveSelectRequested();
    });

    connect(m_introView, &IntroScreen::gameStartedRequested, this, [this]() {
        emit gameStarted(m_activeSaveSlot);
    });
}