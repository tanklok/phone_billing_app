// views/loginview.cpp
#include "loginview.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginView::LoginView(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void LoginView::setupUI()
{
    setWindowTitle("Вход - Учёт междугородних переговоров");
    setFixedSize(400, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Авторизация", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin: 20px;");
    mainLayout->addWidget(titleLabel);

    // Поле логина
    QHBoxLayout *loginLayout = new QHBoxLayout();
    QLabel *loginLabel = new QLabel("Логин:", this);
    loginLabel->setFixedWidth(80);
    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText("Введите логин");
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(m_loginEdit);
    mainLayout->addLayout(loginLayout);

    // Поле пароля
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("Пароль:", this);
    passwordLabel->setFixedWidth(80);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Введите пароль");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);
    mainLayout->addLayout(passwordLayout);

    mainLayout->addSpacing(20);

    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_loginBtn = new QPushButton("Войти", this);
    m_loginBtn->setDefault(true);
    m_registerBtn = new QPushButton("Регистрация", this);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginView::onLoginClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginView::onRegisterClicked);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_loginBtn);
    buttonLayout->addWidget(m_registerBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    // Статус
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red;");
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addStretch();
}

void LoginView::onLoginClicked()
{
    QString login = m_loginEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (login.isEmpty() || password.isEmpty()) {
        m_statusLabel->setText("Заполните все поля");
        return;
    }

    emit loginRequested(login, password);
}

void LoginView::onRegisterClicked()
{
    emit registerRequested();
}
