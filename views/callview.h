// views/callview.h
#ifndef CALLVIEW_H
#define CALLVIEW_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>

class CallView : public QWidget
{
    Q_OBJECT
public:
    explicit CallView(QWidget *parent = nullptr);

public slots:
    void refreshData();

private slots:
    void onSearch();
    void onAdd();
    void onDelete();
    void onRefresh();
    void onCalculate();

private:
    QTableView *m_tableView;

    QDateEdit *m_dateFromEdit;
    QDateEdit *m_dateToEdit;
    QComboBox *m_abonentComboBox;
    QLineEdit *m_searchEdit;

    QPushButton *m_searchBtn;
    QPushButton *m_addBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_calcBtn;
    QPushButton *m_refreshBtn;

    void setupUI();
    void setupTable();
    void loadData();
    void loadAbonents();
};

#endif // CALLVIEW_H
