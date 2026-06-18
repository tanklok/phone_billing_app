#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class RegisterView : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterView(QWidget *parent = nullptr);

signals:
    void registerRequested(const QString &login, const QString &password,
                          const QString &fullName, const QString &roleName);
    void backToLoginRequested();

private slots:
    void onRegisterClicked();
    void onBackClicked();

private:
    QLineEdit *m_loginEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QLineEdit *m_fullNameEdit;
    QComboBox *m_roleComboBox;
    QPushButton *m_registerBtn;
    QPushButton *m_backBtn;
    QLabel *m_statusLabel;

    void setupUI();
    bool validateForm();
};

#endif // REGISTERVIEW_H