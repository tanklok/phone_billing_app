#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

class AddUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddUserDialog(QWidget *parent = nullptr);

signals:
    void userAdded();

private slots:
    void onSave();
    void onCancel();

private:
    QLineEdit *m_loginEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QLineEdit *m_fullNameEdit;
    QComboBox *m_roleComboBox;
    QPushButton *m_saveBtn;
    QPushButton *m_cancelBtn;
    QLabel *m_statusLabel;

    void setupUI();
    bool validateForm();
    void loadRoles();
};

#endif // ADDUSERDIALOG_H