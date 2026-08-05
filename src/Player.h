#pragma once
#include <QString>

class Player {
public:
    Player(const QString &name = "Sir Hero");

    // Getters & Setters
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    int maxHealth() const { return m_maxHealth; }
    int currentHealth() const { return m_currentHealth; }
    void modifyHealth(int amount);

    int maxStamina() const { return m_maxStamina; }
    int currentStamina() const { return m_currentStamina; }
    bool consumeStamina(int amount);
    void restoreStamina(int amount);

    int strength() const { return m_strength; }
    void setStrength(int val) { m_strength = val; }

    int money() const { return m_money; }
    void addMoney(int amount) { m_money += amount; }
    bool spendMoney(int amount);

    int reputation() const { return m_reputation; }
    void modifyReputation(int amount) { m_reputation += amount; }

private:
    QString m_name;

    // Core Stats
    int m_strength;
    int m_maxStamina;
    int m_currentStamina;
    int m_maxHealth;
    int m_currentHealth;
    int m_money;
    int m_reputation;
};