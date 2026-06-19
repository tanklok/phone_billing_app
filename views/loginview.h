#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginView : public QWidget
{
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);

signals:
    void loginRequested(const QString &login, const QString &password);
    // void registerRequested();

private slots:
    void onLoginClicked();
    // void onRegisterClicked();

private:
    QLineEdit *m_loginEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginBtn;
    // QPushButton *m_registerBtn;
    QLabel *m_statusLabel;

    void setupUI();
};

#endif // LOGINVIEW_H
