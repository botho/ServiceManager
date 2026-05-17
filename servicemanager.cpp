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
        int maxId = 0;
        if (this->eraseHistory) {
            maxId = 1; // account for the erase line we wrote as id 1
        }

        // Normalize line endings then split into lines
        QString normalized = historyStr;
        normalized.replace("\r\n", "\n");
        normalized.replace('\r', '\n');
        QStringList lines = normalized.split('\n', Qt::SkipEmptyParts);
        for (const QString &line : lines) {
            QStringList parts = line.split("|");
            if (parts.size() > 0) {
                bool ok = false;
                int id = parts[0].toInt(&ok);
                if (ok && id > maxId) maxId = id;
            }
        }

        int newId = maxId + 1;
        out << QString("%1|g_mmi|STEUERGERAETE_RESET\r\n").arg(newId);
    }
    file.flush();
    file.close();
    return true;
}
