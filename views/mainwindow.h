#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "../models/user.h"
#include "../controllers/authcontroller.h"
#include <QSharedPointer>

class LoginView;
class RegisterView;
class CallView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showLogin();
    void showRegister();
    void showMain(const QSharedPointer<User> &user);
    void logout();

private slots:
    void onLoginAttempt(const QString &login, const QString &password);
    void onRegisterAttempt(const QString &login, const QString &password,
                          const QString &fullName, const QString &roleName);
    void onLoginSuccess(const QSharedPointer<User> &user);
    void onLoginFailed(const QString &reason);
    void onRegistrationSuccess(const QSharedPointer<User> &user);
    void onRegistrationFailed(const QString &reason);

private:
    QStackedWidget *m_stackedWidget;
    LoginView *m_loginView;
    RegisterView *m_registerView;
    CallView *m_callView;

    AuthController m_authController;
    QSharedPointer<User> m_currentUser;

    void setupUI();
    void setupMenu();
    void createViews();
    void switchView(int index);
};

#endif // MAINWINDOW_H
