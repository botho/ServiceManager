#include "servicemanager.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>

ServiceManager::ServiceManager() {}

ServiceHistory * ServiceManager::readHistoryFromFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return nullptr;
    }
    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();
    this->eraseHistory = fileContent.contains("STEUERN_SERVICEHISTORY_ERASE");
    this->steuergeraeteReset = fileContent.contains("STEUERGERAETE_RESET");
    return new ServiceHistory(fileContent);
}

bool ServiceManager::writeHistoryToFile(QString fileName, ServiceHistory * history) {
    QFile file(fileName);
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
        return false;
    }
    QTextStream out(&file);
    int startingOffset = 0;
    QString eraseLine;
    if (this->eraseHistory) {
        eraseLine = "1|g_mmi|STEUERN_SERVICEHISTORY_ERASE\r\n";
        startingOffset = 1;
    }

    QString historyStr = history->toString(startingOffset);

    if (!eraseLine.isEmpty()) {
        out << eraseLine;
    }
    out << historyStr;

    if (this->steuergeraeteReset) {
        int newId = startingOffset + history->services.count() + 1;
        out << QString("%1|g_mmi|STEUERGERAETE_RESET\r\n").arg(newId);
    }
    file.flush();
    file.close();
    return true;
}
