#include "Player.h"
#include <QtGlobal>

Player::Player(const QString &name)
    : m_name(name)
    , m_strength(10)
    , m_maxStamina(100)
    , m_currentStamina(100)
    , m_maxHealth(100)
    , m_currentHealth(100)
    , m_money(0)
    , m_reputation(0)
{}

void Player::modifyHealth(int amount) {
    m_currentHealth = qBound(0, m_currentHealth + amount, m_maxHealth);
}

bool Player::consumeStamina(int amount) {
    if (m_currentStamina >= amount) {
        m_currentStamina -= amount;
        return true;
    }
    return false; // Not enough stamina to block/attack
}

void Player::restoreStamina(int amount) {
    m_currentStamina = qBound(0, m_currentStamina + amount, m_maxStamina);
}

bool Player::spendMoney(int amount) {
    if (m_money >= amount) {
        m_money -= amount;
        return true;
    }
    return false;
}