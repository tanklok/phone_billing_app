// views/callview.cpp
#include "callview.h"
#include "../controllers/callcontroller.h"
#include "../models/calldetail.h"
#include "../core/logger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QInputDialog>
#include <QDate>

CallView::CallView(QWidget *parent) : QWidget(parent)
{
    setupUI();
    setupTable();
    loadAbonents();
    refreshData();
}

void CallView::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Панель фильтров
    QGridLayout *filterLayout = new QGridLayout();

    filterLayout->addWidget(new QLabel("Дата с:", this), 0, 0);
    m_dateFromEdit = new QDateEdit(this);
    m_dateFromEdit->setCalendarPopup(true);
    m_dateFromEdit->setDate(QDate::currentDate().addDays(-30));
    filterLayout->addWidget(m_dateFromEdit, 0, 1);

    filterLayout->addWidget(new QLabel("по:", this), 0, 2);
    m_dateToEdit = new QDateEdit(this);
    m_dateToEdit->setCalendarPopup(true);
    m_dateToEdit->setDate(QDate::currentDate());
    filterLayout->addWidget(m_dateToEdit, 0, 3);

    filterLayout->addWidget(new QLabel("Абонент:", this), 0, 4);
    m_abonentComboBox = new QComboBox(this);
    m_abonentComboBox->addItem("Все абоненты", 0);
    filterLayout->addWidget(m_abonentComboBox, 0, 5);

    filterLayout->addWidget(new QLabel("Поиск:", this), 1, 0);
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Город, абонент...");
    filterLayout->addWidget(m_searchEdit, 1, 1, 1, 3);

    m_searchBtn = new QPushButton("Поиск", this);
    connect(m_searchBtn, &QPushButton::clicked, this, &CallView::onSearch);
    filterLayout->addWidget(m_searchBtn, 1, 4);

    m_refreshBtn = new QPushButton("Обновить", this);
    connect(m_refreshBtn, &QPushButton::clicked, this, &CallView::onRefresh);
    filterLayout->addWidget(m_refreshBtn, 1, 5);

    mainLayout->addLayout(filterLayout);

    // Таблица
    m_tableView = new QTableView(this);
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    mainLayout->addWidget(m_tableView);

    // Кнопки управления
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    m_addBtn = new QPushButton("Добавить звонок", this);
    connect(m_addBtn, &QPushButton::clicked, this, &CallView::onAdd);
    buttonLayout->addWidget(m_addBtn);

    m_deleteBtn = new QPushButton("Удалить", this);
    connect(m_deleteBtn, &QPushButton::clicked, this, &CallView::onDelete);
    buttonLayout->addWidget(m_deleteBtn);

    m_calcBtn = new QPushButton("Расчёт стоимости", this);
    connect(m_calcBtn, &QPushButton::clicked, this, &CallView::onCalculate);
    buttonLayout->addWidget(m_calcBtn);

    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void CallView::setupTable()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({
        "ID", "Дата", "Длит.", "Время", "Отправитель", "Получатель",
        "Город отпр.", "Город назн.", "Цена/мин", "Скидка", "Итог"
    });
    m_tableView->setModel(model);

    m_tableView->setColumnHidden(0, true);

    int widths[] = {0, 90, 50, 70, 150, 150, 120, 120, 80, 60, 100};
    for (int i = 0; i < 11; ++i) {
        m_tableView->setColumnWidth(i, widths[i]);
    }
}

void CallView::loadAbonents()
{
    // Для MVP просто оставляем "Все абоненты"
    m_abonentComboBox->clear();
    m_abonentComboBox->addItem("Все абоненты", 0);
}

void CallView::loadData()
{
    QDate dateFrom = m_dateFromEdit->date();
    QDate dateTo = m_dateToEdit->date();
    qint64 abonentId = m_abonentComboBox->currentData().toLongLong();

    CallController controller;
    QList<CallDetail*> details = controller.getCallDetails(dateFrom, dateTo, abonentId);

    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(m_tableView->model());
    if (!model) {
        setupTable();
        model = qobject_cast<QStandardItemModel*>(m_tableView->model());
    }

    model->setRowCount(0);

    for (CallDetail *detail : details) {
        QList<QStandardItem*> row;

        QStandardItem *idItem = new QStandardItem(QString::number(detail->getCallId()));
        idItem->setData(detail->getCallId(), Qt::UserRole);
        row << idItem;

        row << new QStandardItem(detail->getDate().toString("dd.MM.yyyy"));
        row << new QStandardItem(QString::number(detail->getDuration()));
        row << new QStandardItem(detail->getTimeOfDay());
        row << new QStandardItem(detail->getSenderName());
        row << new QStandardItem(detail->getReceiverName());
        row << new QStandardItem(detail->getFromCity());
        row << new QStandardItem(detail->getToCity());
        row << new QStandardItem(QString::number(detail->getCostPerMinute(), 'f', 2));
        row << new QStandardItem(QString::number(detail->getDiscount(), 'f', 0) + "%");
        row << new QStandardItem(QString::number(detail->getTotalCost(), 'f', 2));

        model->appendRow(row);

        delete detail;
    }

    Logger::getInstance().info("Loaded " + QString::number(details.size()) + " calls");
}

void CallView::refreshData()
{
    loadData();
}

void CallView::onSearch()
{
    loadData();
}

void CallView::onRefresh()
{
    loadAbonents();
    loadData();
}

void CallView::onAdd()
{
    bool ok;
    QString dateStr = QInputDialog::getText(this, "Добавление звонка",
                                            "Дата (ГГГГ-ММ-ДД):", QLineEdit::Normal,
                                            QDate::currentDate().toString("yyyy-MM-dd"), &ok);
    if (!ok) return;

    int duration = QInputDialog::getInt(this, "Добавление звонка",
                                        "Длительность (мин):", 5, 1, 10000, 1, &ok);
    if (!ok) return;

    QStringList times = {"день", "ночь"};
    QString timeOfDay = QInputDialog::getItem(this, "Добавление звонка",
                                              "Время суток:", times, 0, false, &ok);
    if (!ok) return;

    qint64 callerId = QInputDialog::getInt(this, "Добавление звонка",
                                           "ID отправителя:", 1, 1, 999999, 1, &ok);
    if (!ok) return;

    qint64 receiverId = QInputDialog::getInt(this, "Добавление звонка",
                                             "ID получателя:", 2, 1, 999999, 1, &ok);
    if (!ok) return;

    Call call;
    call.setDate(QDate::fromString(dateStr, "yyyy-MM-dd"));
    call.setDuration(duration);
    call.setTimeOfDay(timeOfDay);
    call.setCallerId(callerId);
    call.setReceiverId(receiverId);

    CallController controller;
    if (controller.createCall(call)) {
        QMessageBox::information(this, "Успех", "Звонок добавлен");
        refreshData();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить звонок");
    }
}

void CallView::onDelete()
{
    QModelIndex index = m_tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите запись для удаления");
        return;
    }

    qint64 id = index.sibling(index.row(), 0).data(Qt::UserRole).toLongLong();

    if (QMessageBox::question(this, "Подтверждение удаления",
                              "Удалить выбранный звонок?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        CallController controller;
        if (controller.deleteCall(id)) {
            QMessageBox::information(this, "Успех", "Звонок удалён");
            refreshData();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить звонок");
        }
    }
}

void CallView::onCalculate()
{
    bool ok;
    qint64 callerId = QInputDialog::getInt(this, "Расчёт стоимости",
                                           "ID отправителя:", 1, 1, 999999, 1, &ok);
    if (!ok) return;

    qint64 receiverId = QInputDialog::getInt(this, "Расчёт стоимости",
                                             "ID получателя:", 2, 1, 999999, 1, &ok);
    if (!ok) return;

    int duration = QInputDialog::getInt(this, "Расчёт стоимости",
                                        "Длительность (мин):", 5, 1, 10000, 1, &ok);
    if (!ok) return;

    QStringList times = {"день", "ночь"};
    QString timeOfDay = QInputDialog::getItem(this, "Расчёт стоимости",
                                              "Время суток:", times, 0, false, &ok);
    if (!ok) return;

    CallController controller;
    double discount = 0;
    double totalCost = 0;

    double result = controller.calculateCallCost(callerId, receiverId, timeOfDay,
                                                  duration, discount, totalCost);

    if (result < 0) {
        QMessageBox::warning(this, "Ошибка", "Не удалось рассчитать стоимость");
        return;
    }

    double baseCost = totalCost * 100 / (100 - discount);

    QMessageBox::information(this, "Результат расчёта",
                             QString("Стоимость без скидки: %1 ₽\n"
                                     "Скидка: %2%\n"
                                     "Итоговая стоимость: %3 ₽")
                             .arg(baseCost, 0, 'f', 2)
                             .arg(discount, 0, 'f', 0)
                             .arg(totalCost, 0, 'f', 2));
}
