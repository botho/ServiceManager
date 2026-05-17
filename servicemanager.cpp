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
    this->eraseHistory = fileContent.contains("steuern_servicehistory_erase");
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
        eraseLine = "1|g_mmi|steuern_servicehistory_erase\r\n";
        startingOffset = 1;
    }

    // Generate history string with the appropriate starting offset
    QString historyStr = history->toString(startingOffset);

    // Write erase line (if any) and the history content
    if (!eraseLine.isEmpty()) {
        out << eraseLine;
    }
    out << historyStr;

    // If the STEUERGERAETE_RESET flag is set, append it with an incremented id
    if (this->steuergeraeteReset) {
        // The next id is simply startingOffset + number_of_services + 1
        int newId = startingOffset + history->services.count() + 1;
        out << QString("%1|g_mmi|STEUERGERAETE_RESET\r\n").arg(newId);
    }
    file.flush();
    file.close();
    return true;
}
