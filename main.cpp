// main.cpp
#include <QApplication>
#include <QMessageBox>
#include "core/config.h"
#include "core/logger.h"
#include "core/databasemanager.h"
#include "views/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("PhoneBillingApp");

    Config::getInstance().loadFromFile("config.ini");

    if (!DatabaseManager::getInstance().connect()) {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось подключиться к базе данных!\n\n" +
                              DatabaseManager::getInstance().getLastError());
        return 1;
    }

    Logger::getInstance().info("Application started");

    MainWindow mainWindow;
    mainWindow.show();

    int result = app.exec();

    DatabaseManager::getInstance().disconnect();
    Logger::getInstance().info("Application finished");

    return result;
}