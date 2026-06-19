#include "registerview.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

RegisterView::RegisterView(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void RegisterView::setupUI()
{
    setWindowTitle("Регистрация - Учёт междугородних переговоров");
    setFixedSize(450, 450);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Регистрация", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 20px;");
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *loginLayout = new QHBoxLayout();
    QLabel *loginLabel = new QLabel("Логин:*", this);
    loginLabel->setFixedWidth(120);
    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText("Введите логин");
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(m_loginEdit);
    mainLayout->addLayout(loginLayout);

    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("Пароль:*", this);
    passwordLabel->setFixedWidth(120);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Минимум 8 символов");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);
    mainLayout->addLayout(passwordLayout);

    QHBoxLayout *confirmLayout = new QHBoxLayout();
    QLabel *confirmLabel = new QLabel("Подтвердите:*", this);
    confirmLabel->setFixedWidth(120);
    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmLayout->addWidget(confirmLabel);
    confirmLayout->addWidget(m_confirmPasswordEdit);
    mainLayout->addLayout(confirmLayout);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    QLabel *nameLabel = new QLabel("ФИО:*", this);
    nameLabel->setFixedWidth(120);
    m_fullNameEdit = new QLineEdit(this);
    m_fullNameEdit->setPlaceholderText("Введите ФИО");
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(m_fullNameEdit);
    mainLayout->addLayout(nameLayout);

    QHBoxLayout *roleLayout = new QHBoxLayout();
    QLabel *roleLabel = new QLabel("Роль:*", this);
    roleLabel->setFixedWidth(120);
    m_roleComboBox = new QComboBox(this);
    m_roleComboBox->addItems({"operator", "manager"});
    roleLayout->addWidget(roleLabel);
    roleLayout->addWidget(m_roleComboBox);
    mainLayout->addLayout(roleLayout);

    mainLayout->addSpacing(20);

    QLabel *hintLabel = new QLabel(
        "Требования к паролю:\n"
        "• Минимум 8 символов\n"
        "• Одна заглавная буква (A-Z)\n"
        "• Одна цифра (0-9)\n"
        "• Один спецсимвол (@$!%*?&)",
        this
    );
    hintLabel->setStyleSheet("color: #666; font-size: 10px;");
    hintLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(hintLabel);

    mainLayout->addSpacing(20);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_backBtn = new QPushButton("Назад", this);
    m_registerBtn = new QPushButton("Зарегистрироваться", this);
    m_registerBtn->setDefault(true);

    connect(m_backBtn, &QPushButton::clicked, this, &RegisterView::onBackClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &RegisterView::onRegisterClicked);

    buttonLayout->addWidget(m_backBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_registerBtn);
    mainLayout->addLayout(buttonLayout);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red;");
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addStretch();
}

bool RegisterView::validateForm()
{
    QString login = m_loginEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirm = m_confirmPasswordEdit->text();
    QString fullName = m_fullNameEdit->text().trimmed();

    if (login.isEmpty() || login.length() < 3) {
        m_statusLabel->setText("Логин должен содержать минимум 3 символа");
        return false;
    }

    if (password != confirm) {
        m_statusLabel->setText("Пароли не совпадают");
        return false;
    }

    if (fullName.isEmpty()) {
        m_statusLabel->setText("Введите ФИО");
        return false;
    }

    m_statusLabel->clear();
    return true;
}

void RegisterView::onRegisterClicked()
{
    if (!validateForm()) return;

    emit registerRequested(
        m_loginEdit->text().trimmed(),
        m_passwordEdit->text(),
        m_fullNameEdit->text().trimmed(),
        m_roleComboBox->currentText()
    );
}

void RegisterView::onBackClicked()
{
    emit backToLoginRequested();
}
