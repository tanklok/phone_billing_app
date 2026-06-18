#include "mainwindow.h"
#include "loginview.h"
#include "registerview.h"
#include "callview.h"
#include "../controllers/authcontroller.h"
#include "../controllers/callcontroller.h"

#include <QMessageBox>
#include <QStatusBar>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    showLogin();

    connect(&m_authController, &AuthController::loginSuccess,
            this, &MainWindow::onLoginSuccess);
    connect(&m_authController, &AuthController::loginFailed,
            this, &MainWindow::onLoginFailed);
    connect(&m_authController, &AuthController::registrationSuccess,
            this, &MainWindow::onRegistrationSuccess);
    connect(&m_authController, &AuthController::registrationFailed,
            this, &MainWindow::onRegistrationFailed);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    setWindowTitle("Учёт междугородних переговоров");
    resize(1100, 700);

    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    createViews();
}

void MainWindow::createViews()
{
    m_loginView = new LoginView(this);
    connect(m_loginView, &LoginView::loginRequested, this, &MainWindow::onLoginAttempt);
    connect(m_loginView, &LoginView::registerRequested, this, &MainWindow::showRegister);

    m_registerView = new RegisterView(this);
    connect(m_registerView, &RegisterView::registerRequested, this, &MainWindow::onRegisterAttempt);
    connect(m_registerView, &RegisterView::backToLoginRequested, this, &MainWindow::showLogin);

    m_callView = new CallView(this);

    m_stackedWidget->addWidget(m_loginView);
    m_stackedWidget->addWidget(m_registerView);
    m_stackedWidget->addWidget(m_callView);
}

void MainWindow::setupMenu()
{
    menuBar()->clear();

    QMenu *fileMenu = menuBar()->addMenu("&Файл");
    QAction *logoutAction = fileMenu->addAction("&Выйти");
    connect(logoutAction, &QAction::triggered, this, &MainWindow::logout);

    QMenu *callMenu = menuBar()->addMenu("&Звонки");
    QAction *viewCallsAction = callMenu->addAction("&Список звонков");
    connect(viewCallsAction, &QAction::triggered, [this]() { switchView(2); });

    QMenu *helpMenu = menuBar()->addMenu("&Помощь");
    QAction *aboutAction = helpMenu->addAction("&О программе");
    connect(aboutAction, &QAction::triggered, []() {
        QMessageBox::information(nullptr, "О программе",
                                 "Информационная система для учёта междугородних переговоров\n"
                                 "Версия 1.0 (MVP)");
    });

    if (m_currentUser) {
        statusBar()->showMessage(
            QString("Пользователь: %1 (%2)").arg(m_currentUser->getFullName(), m_currentUser->getLogin())
        );
    } else {
        statusBar()->showMessage("Не авторизован");
    }
}

void MainWindow::switchView(int index)
{
    m_stackedWidget->setCurrentIndex(index);
}

void MainWindow::showLogin()
{
    m_stackedWidget->setCurrentIndex(0);
    setWindowTitle("Вход - Учёт междугородних переговоров");
}

void MainWindow::showRegister()
{
    m_stackedWidget->setCurrentIndex(1);
    setWindowTitle("Регистрация - Учёт междугородних переговоров");
}

void MainWindow::showMain(const QSharedPointer<User> &user)
{
    m_currentUser = user;
    setupMenu();
    switchView(2);
    setWindowTitle(QString("Учёт междугородних переговоров - %1").arg(user->getFullName()));
    statusBar()->showMessage(
        QString("Пользователь: %1 (%2)").arg(user->getFullName(), user->getLogin())
    );
}

void MainWindow::logout()
{
    m_authController.logout();
    m_currentUser.clear();
    showLogin();
    statusBar()->showMessage("Вы вышли из системы");
}

void MainWindow::onLoginAttempt(const QString &login, const QString &password)
{
    QSharedPointer<User> user;
    if (m_authController.login(login, password, user)) {
        // сигнал сделает остальное
    }
}

void MainWindow::onRegisterAttempt(const QString &login, const QString &password,
                                   const QString &fullName, const QString &roleName)
{
    QSharedPointer<User> user;
    if (m_authController.registerUser(login, password, fullName, roleName, user)) {
        // сигнал сделает остальное
    }
}

void MainWindow::onLoginSuccess(const QSharedPointer<User> &user)
{
    QMessageBox::information(this, "Успех", "Вход выполнен успешно!");
    showMain(user);
}

void MainWindow::onLoginFailed(const QString &reason)
{
    QMessageBox::warning(this, "Ошибка входа", reason);
}

void MainWindow::onRegistrationSuccess(const QSharedPointer<User> &user)
{
    QMessageBox::information(this, "Успех", "Регистрация прошла успешно!");
    showLogin();
}

void MainWindow::onRegistrationFailed(const QString &reason)
{
    QMessageBox::warning(this, "Ошибка регистрации", reason);
}
