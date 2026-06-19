// views/loginview.cpp
#include "loginview.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

LoginView::LoginView(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void LoginView::setupUI()
{
    setWindowTitle("Вход - Учёт междугородних переговоров");
    setMinimumSize(450, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch();

    // Заголовок
    QLabel *titleLabel = new QLabel("Авторизация", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(30);

    QHBoxLayout *loginLayout = new QHBoxLayout();
    loginLayout->addStretch();

    QLabel *loginLabel = new QLabel("Логин:", this);
    loginLabel->setFixedWidth(80);
    QFont labelFont = loginLabel->font();
    labelFont.setPointSize(12);
    loginLabel->setFont(labelFont);

    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText("Введите логин");
    m_loginEdit->setMinimumHeight(35);
    QFont editFont = m_loginEdit->font();
    editFont.setPointSize(12);
    m_loginEdit->setFont(editFont);
    m_loginEdit->setMinimumWidth(200);

    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(m_loginEdit);
    loginLayout->addStretch();
    mainLayout->addLayout(loginLayout);
    mainLayout->addSpacing(15);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->addStretch();

    QLabel *passwordLabel = new QLabel("Пароль:", this);
    passwordLabel->setFixedWidth(80);
    passwordLabel->setFont(labelFont);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Введите пароль");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMinimumHeight(35);
    m_passwordEdit->setFont(editFont);
    m_passwordEdit->setMinimumWidth(200);

    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);
    passwordLayout->addStretch();
    mainLayout->addLayout(passwordLayout);
    mainLayout->addSpacing(30);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_loginBtn = new QPushButton("Войти", this);
    m_loginBtn->setDefault(true);
    m_loginBtn->setMinimumHeight(40);
    m_loginBtn->setMinimumWidth(100);
    QFont btnFont = m_loginBtn->font();
    btnFont.setPointSize(12);
    m_loginBtn->setFont(btnFont);
    m_loginBtn->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px;");

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginView::onLoginClicked);
    // connect(m_registerBtn, &QPushButton::clicked, this, &LoginView::onRegisterClicked);

    buttonLayout->addWidget(m_loginBtn);
    buttonLayout->addSpacing(20);
    // buttonLayout->addWidget(m_registerBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(20);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red; font-size: 12px;");
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addStretch();

    setLayout(mainLayout);
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

// void LoginView::onRegisterClicked()
// {
//     emit registerRequested();
// }
