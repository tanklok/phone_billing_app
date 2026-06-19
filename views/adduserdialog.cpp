#include "adduserdialog.h"
#include "../controllers/authcontroller.h"
#include "../controllers/rolecontroller.h"
#include "../models/role.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

AddUserDialog::AddUserDialog(QWidget *parent) : QDialog(parent)
{
    setupUI();
    loadRoles();
}

void AddUserDialog::setupUI()
{
    setWindowTitle("Добавление пользователя (администратор)");
    setModal(true);
    resize(400, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Логин
    QHBoxLayout *loginLayout = new QHBoxLayout();
    QLabel *loginLabel = new QLabel("Логин:*", this);
    loginLabel->setFixedWidth(100);
    m_loginEdit = new QLineEdit(this);
    m_loginEdit->setPlaceholderText("Введите логин");
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(m_loginEdit);
    mainLayout->addLayout(loginLayout);

    // Пароль
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    QLabel *passwordLabel = new QLabel("Пароль:*", this);
    passwordLabel->setFixedWidth(100);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Минимум 8 символов");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(m_passwordEdit);
    mainLayout->addLayout(passwordLayout);

    // Подтверждение пароля
    QHBoxLayout *confirmLayout = new QHBoxLayout();
    QLabel *confirmLabel = new QLabel("Подтвердите:*", this);
    confirmLabel->setFixedWidth(100);
    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmLayout->addWidget(confirmLabel);
    confirmLayout->addWidget(m_confirmPasswordEdit);
    mainLayout->addLayout(confirmLayout);

    // ФИО
    QHBoxLayout *nameLayout = new QHBoxLayout();
    QLabel *nameLabel = new QLabel("ФИО:*", this);
    nameLabel->setFixedWidth(100);
    m_fullNameEdit = new QLineEdit(this);
    m_fullNameEdit->setPlaceholderText("Введите ФИО");
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(m_fullNameEdit);
    mainLayout->addLayout(nameLayout);

    // Роль
    QHBoxLayout *roleLayout = new QHBoxLayout();
    QLabel *roleLabel = new QLabel("Роль:*", this);
    roleLabel->setFixedWidth(100);
    m_roleComboBox = new QComboBox(this);
    roleLayout->addWidget(roleLabel);
    roleLayout->addWidget(m_roleComboBox);
    mainLayout->addLayout(roleLayout);

    mainLayout->addSpacing(20);

    // Подсказка
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

    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_saveBtn = new QPushButton("Создать", this);
    m_saveBtn->setDefault(true);
    m_cancelBtn = new QPushButton("Отмена", this);

    connect(m_saveBtn, &QPushButton::clicked, this, &AddUserDialog::onSave);
    connect(m_cancelBtn, &QPushButton::clicked, this, &AddUserDialog::onCancel);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_saveBtn);
    buttonLayout->addWidget(m_cancelBtn);
    mainLayout->addLayout(buttonLayout);

    // Статус
    m_statusLabel = new QLabel(this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: red;");
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addStretch();
}

void AddUserDialog::loadRoles()
{
    RoleController roleController;
    QList<Role*> roles = roleController.getAllRoles();

    m_roleComboBox->clear();
    for (Role* role : roles) {
        m_roleComboBox->addItem(role->getName(), role->getId());
        delete role;
    }
}

bool AddUserDialog::validateForm()
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

    if (!AuthController::isPasswordStrong(password)) {
        m_statusLabel->setText("Пароль не соответствует требованиям");
        return false;
    }

    if (fullName.isEmpty()) {
        m_statusLabel->setText("Введите ФИО");
        return false;
    }

    m_statusLabel->clear();
    return true;
}

void AddUserDialog::onSave()
{
    if (!validateForm()) return;

    QString login = m_loginEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString fullName = m_fullNameEdit->text().trimmed();
    QString roleName = m_roleComboBox->currentText();

    AuthController authController;
    QSharedPointer<User> user;

    if (authController.createUserByAdmin(login, password, fullName, roleName, user)) {
        QMessageBox::information(this, "Успех", "Пользователь создан");
        emit userAdded();
        accept();
    } else {
        // Ошибка уже показана через сигнал registrationFailed, но мы можем показать дополнительно
        // Для простоты просто закроем диалог с ошибкой
        QMessageBox::warning(this, "Ошибка", "Не удалось создать пользователя");
    }
}

void AddUserDialog::onCancel()
{
    reject();
}
