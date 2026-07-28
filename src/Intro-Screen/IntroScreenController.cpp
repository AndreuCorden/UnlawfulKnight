#include "IntroScreenController.h"

IntroScreenController::IntroScreenController(QObject *parent)
    : QObject(parent) {}

IntroScreen* IntroScreenController::getView(QWidget *parent) {
    if (!m_introView) {
        m_introView = new IntroScreen(parent);
        setupConnections();
    }
    return m_introView;
}

void IntroScreenController::startIntroForSlot(int slotId) {
    m_activeSaveSlot = slotId;
    // Perform any data initialization for the save slot here
}

void IntroScreenController::setupConnections() {
    if (!m_introView) return;

    // Handle view back button -> signal MainWindow
    connect(m_introView, &IntroScreen::backToMenuRequested, this, [this]() {
        emit backToSaveSelectRequested();
    });
}