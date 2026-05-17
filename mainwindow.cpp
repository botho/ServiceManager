#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QHeaderView>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    this->service = new ServiceHistory();

    ui->setupUi(this);

    this->setWindowTitle("BMW Service History Manager");
    ui->servicesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->servicesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->servicesTable->horizontalHeader()->setStretchLastSection(false);
    ui->servicesTable->setColumnCount(5);
    ui->servicesTable->setHorizontalHeaderLabels(QStringList() << "Date" << "Mileage" << "Dealer" << "BMW" << "Status");
    connect(ui->servicesTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::on_servicesTable_headerClicked);
    resizeTableColumns(ui->servicesTable);

    ui->serviceItemsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->serviceItemsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->serviceItemsTable->horizontalHeader()->setStretchLastSection(false);
    ui->serviceItemsTable->setColumnCount(4);
    ui->serviceItemsTable->setHorizontalHeaderLabels(QStringList() << "Type" << "Status" << "Remaining distance (Km)" << "Remaining time (Months)");
    resizeTableColumns(ui->serviceItemsTable);

    ui->status->addItems(ServiceTimeList());
    ui->serviceStatus->addItems(ServiceTimeList());
    ui->serviceType->addItems(ServiceItemsList());
    ui->milage->setValidator(new QIntValidator(0, 999999, this));
    ui->remainingDistance->setValidator(new QIntValidator(-999999, 999999, this));
    ui->remainingTime->setValidator(new QIntValidator(0, 999999, this));

    setFirstSelectionAvailable(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::resizeTableColumns(QTableWidget *table) {
    table->resizeColumnsToContents();
    
    int availableWidth = table->width() - table->verticalHeader()->width();
    
    if (table->horizontalScrollBar()->isVisible()) {
        availableWidth -= table->horizontalScrollBar()->height();
    }
    
    int totalWidth = 0;
    for (int i = 0; i < table->columnCount(); ++i) {
        totalWidth += table->columnWidth(i);
    }
    
    if (totalWidth < availableWidth) {
        int extraWidth = availableWidth - totalWidth;
        for (int i = 0; i < table->columnCount(); ++i) {
            int currentWidth = table->columnWidth(i);
            int proportion = (currentWidth * extraWidth) / totalWidth;
            table->setColumnWidth(i, currentWidth + proportion);
        }
    }
}

void MainWindow::on_actionCreate_triggered() {
    this->fileName = QFileDialog::getSaveFileName(this, tr("Create File"), QDir::homePath() + "/Desktop/test.tst", tr("Test file (*.tst)"));
    if (this->fileName.count()) {
        this->service = new ServiceHistory();
        if (!this->manager.writeHistoryToFile(fileName, this->service)) {
            QMessageBox::information(0, "Error", "Could not create file");
        }
        reloadTable();
    }
}

void MainWindow::on_actionOpen_triggered() {
    this->fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath() + "/Desktop", tr("Test file (*.tst)"));
    if (this->fileName.count()) {
        this->service = this->manager.readHistoryFromFile(fileName);
        if (!this->service) {
            QMessageBox::information(0, "Error", "Could not open file");
        }
        reloadTable();
    }
}

void MainWindow::on_actionSafe_triggered() {
    if (this->fileName.count() > 0) {
        if (!this->manager.writeHistoryToFile(this->fileName, this->service)) {
            QMessageBox::information(0, "Error", "Could not save file");
        }
    } else {
        on_actionSave_as_triggered();
    }
}

void MainWindow::on_actionSave_as_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Create File"), QDir::homePath() + "/Desktop/test.tst", tr("Test file (*.tst)"));
    if (fileName.count()) {
        if (!this->manager.writeHistoryToFile(fileName, this->service)) {
            QMessageBox::information(0, "Error", "Could not save file");
        }
        if (this->fileName.count() == 0) {
            this->fileName = fileName;
        }
    }
}

void MainWindow::reloadTable() {
    ui->historyCheckBox->setChecked(this->manager.eraseHistory);
    ui->steuergeraeteResetCheckBox->setChecked(this->manager.steuergeraeteReset);
    ui->servicesTable->setColumnCount(5);
    ui->servicesTable->setHorizontalHeaderLabels(QStringList() << "Date" << "Mileage" << "Dealer" << "BMW" << "Status");
    ui->servicesTable->setRowCount(this->service->services.count());
    for (int i = 0; i < this->service->services.count(); ++i) {
        ui->servicesTable->setItem(i, 0, new QTableWidgetItem(this->service->services[i].date.toString("dd.MM.yyyy")));
        ui->servicesTable->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(this->service->services[i].milage)));
        ui->servicesTable->setItem(i, 2, new QTableWidgetItem(this->service->services[i].dealer));
        ui->servicesTable->setItem(i, 3, new QTableWidgetItem(this->service->services[i].bmw ? "Yes" : "No"));
        ui->servicesTable->setItem(i, 4, new QTableWidgetItem(TimeStringValue(this->service->services[i].time)));
    }
    resizeTableColumns(ui->servicesTable);
}

void MainWindow::reloadSecondTable() {
    ui->serviceItemsTable->setColumnCount(4);
    ui->serviceItemsTable->setHorizontalHeaderLabels(QStringList() << "Type" << "Status" << "Remaining distance (Km)" << "Remaining time (Months)");
    ui->serviceItemsTable->setRowCount(this->service->services[this->selectedRow].items.count());
    for (int i = 0; i < this->service->services[this->selectedRow].items.count(); ++i) {
        ServiceItem item = this->service->services[this->selectedRow].items[i];
        ui->serviceItemsTable->setItem(i, 0, new QTableWidgetItem(ServiceStringValue(item.type)));
        ui->serviceItemsTable->setItem(i, 1, new QTableWidgetItem(TimeStringValue(item.time)));
        ui->serviceItemsTable->setItem(i, 2, new QTableWidgetItem(tr("%1").arg(item.remainingDistance)));
        ui->serviceItemsTable->setItem(i, 3, new QTableWidgetItem(tr("%1").arg(item.remainingTime)));
    }
    resizeTableColumns(ui->serviceItemsTable);
}

void MainWindow::setFirstSelectionAvailable(bool available) {
    ui->time->setEnabled(available);
    ui->milage->setEnabled(available);
    ui->dealer->setEnabled(available);
    ui->bmw->setEnabled(available);
    ui->status->setEnabled(available);
    ui->deleteSelectedHistory->setEnabled(available);

    ui->serviceItemsTable->setEnabled(available);
    ui->addTopService->setEnabled(available);
    ui->addBottomService->setEnabled(available);

    if (!available) {
        setSecondSelectionAvailable(false);
    }
}

void MainWindow::setSecondSelectionAvailable(bool available) {
    ui->deleteSelectedService->setEnabled(available);
    ui->serviceStatus->setEnabled(available);
    ui->serviceType->setEnabled(available);
    ui->remainingDistance->setEnabled(available);
    ui->remainingTime->setEnabled(available);
    ui->moveUpService->setEnabled(available);
    ui->moveDownService->setEnabled(available);
}

void MainWindow::on_servicesTable_cellClicked(int row, int column) {
    this->selectedRow = row;
    Service s = this->service->services[row];
    setFirstSelectionAvailable(true);

    ui->time->setDate(s.date);
    ui->milage->setText(tr("%1").arg(s.milage));
    ui->dealer->setText(s.dealer);
    ui->bmw->setChecked(s.bmw);
    ui->status->setCurrentIndex(IndexForTime(s.time));

    reloadSecondTable();
    setSecondSelectionAvailable(false);
}

void MainWindow::on_historyCheckBox_stateChanged() {
    this->manager.eraseHistory = ui->historyCheckBox->checkState();
}

void MainWindow::on_steuergeraeteResetCheckBox_stateChanged() {
    this->manager.steuergeraeteReset = ui->steuergeraeteResetCheckBox->checkState();
}

void MainWindow::on_time_userDateChanged(const QDate &date) {
    int i = this->selectedRow;
    this->service->services[i].date = date;
    ui->servicesTable->setItem(i, 0, new QTableWidgetItem(this->service->services[i].date.toString("dd.MM.yyyy")));
}

void MainWindow::on_milage_textChanged(const QString &arg1) {
    int i = this->selectedRow;
    this->service->services[i].milage = arg1.toInt();
    ui->servicesTable->setItem(i, 1, new QTableWidgetItem(tr("%1").arg(this->service->services[i].milage)));
}

void MainWindow::on_dealer_textChanged(const QString &arg1) {
    int i = this->selectedRow;
    this->service->services[i].dealer = arg1;
    ui->servicesTable->setItem(i, 2, new QTableWidgetItem(this->service->services[i].dealer));
}

void MainWindow::on_bmw_stateChanged(int arg1) {
    int i = this->selectedRow;
    this->service->services[i].bmw = ui->bmw->isChecked();
    ui->servicesTable->setItem(i, 3, new QTableWidgetItem(this->service->services[i].bmw ? "Yes" : "No"));
}

void MainWindow::on_status_currentIndexChanged(int index) {
    if (this->selectedRow < 0) { return; }
    int i = this->selectedRow;
    this->service->services[i].time = index == 1 ? late : index == 2 ? over : ok;
    ui->servicesTable->setItem(i, 4, new QTableWidgetItem(TimeStringValue(this->service->services[i].time)));
}

void MainWindow::on_addTopHistory_clicked() {
    this->service->services.insert(0, Service());
    setFirstSelectionAvailable(false);
    this->selectedRow = -1;
    reloadTable();
}

void MainWindow::on_deleteSelectedHistory_clicked() {
    this->service->services.removeAt(this->selectedRow);
    this->selectedRow = -1;
    setFirstSelectionAvailable(false);
    reloadTable();
}

void MainWindow::on_addBottomHistory_clicked() {
    this->service->services.append(Service());
    reloadTable();
}

void MainWindow::on_serviceItemsTable_cellClicked(int row, int column) {
    this->selectedService = row;
    ServiceItem item = this->service->services[this->selectedRow].items[row];
    setSecondSelectionAvailable(true);

    ui->serviceType->setCurrentIndex(IndexForService(item.type));
    ui->serviceStatus->setCurrentIndex(IndexForTime(item.time));
    ui->remainingDistance->setText(tr("%1").arg(item.remainingDistance));
    ui->remainingTime->setText(tr("%1").arg(item.remainingTime));
}

void MainWindow::on_serviceType_currentIndexChanged(int index) {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    ServiceItemType type = ServiceForIndex(index);
    this->service->services[this->selectedRow].items[this->selectedService].type = type;
    ui->serviceItemsTable->setItem(this->selectedService, 0, new QTableWidgetItem(ServiceStringValue(type)));
}

void MainWindow::on_serviceStatus_currentIndexChanged(int index) {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    this->service->services[this->selectedRow].items[this->selectedService].time = index == 1 ? late : index == 2 ? over : ok;
    ui->serviceItemsTable->setItem(this->selectedService, 1, new QTableWidgetItem(TimeStringValue(index == 1 ? late : index == 2 ? over : ok)));
}

void MainWindow::on_deleteSelectedService_clicked() {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    this->service->services[this->selectedRow].items.removeAt(this->selectedService);
    this->selectedService = -1;
    setSecondSelectionAvailable(false);
    reloadSecondTable();
}

void MainWindow::on_addTopService_clicked() {
    this->service->services[this->selectedRow].items.insert(0, ServiceItem(engineOil, ok));
    setSecondSelectionAvailable(false);
    this->selectedService = -1;
    reloadSecondTable();
}

void MainWindow::on_addBottomService_clicked() {
    this->service->services[this->selectedRow].items.append(ServiceItem(engineOil, ok));
    reloadSecondTable();
}

void MainWindow::on_remainingDistance_textChanged(const QString &arg1) {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    int distance = arg1.isEmpty() ? 0 : arg1.toInt();
    this->service->services[this->selectedRow].items[this->selectedService].remainingDistance = distance;
    ui->serviceItemsTable->setItem(this->selectedService, 2, new QTableWidgetItem(tr("%1").arg(distance)));
}

void MainWindow::on_remainingTime_textChanged(const QString &arg1) {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    int time = arg1.isEmpty() ? 0 : arg1.toInt();
    this->service->services[this->selectedRow].items[this->selectedService].remainingTime = time;
    ui->serviceItemsTable->setItem(this->selectedService, 3, new QTableWidgetItem(tr("%1").arg(time)));
}

void MainWindow::on_servicesTable_headerClicked(int column) {
    // Only allow sorting on Date (0) and Mileage (1)
    if (column != 0 && column != 1) {
        return;
    }
    sortServicesTable(column);
}

void MainWindow::sortServicesTable(int column) {
    // Toggle sort order if clicking the same column
    if (this->sortColumn == column) {
        this->sortOrder = (this->sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    } else {
        this->sortColumn = column;
        this->sortOrder = Qt::AscendingOrder;
    }
    
    // Sort the services vector
    std::sort(this->service->services.begin(), this->service->services.end(),
        [this, column](const Service &a, const Service &b) -> bool {
            if (column == 0) {
                // Sort by Date
                int cmp = a.date.daysTo(b.date);
                if (cmp == 0) return false;
                return (this->sortOrder == Qt::AscendingOrder) ? (cmp < 0) : (cmp > 0);
            } else if (column == 1) {
                // Sort by Mileage
                if (a.milage == b.milage) return false;
                return (this->sortOrder == Qt::AscendingOrder) ? (a.milage < b.milage) : (a.milage > b.milage);
            }
            return false;
        }
    );
    
    // Reload the table and reset selection
    this->selectedRow = -1;
    setFirstSelectionAvailable(false);
    reloadTable();
}

void MainWindow::on_moveUpService_clicked() {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    if (this->selectedService == 0) { return; } // Can't move first item up
    
    // Swap the current item with the one above
    ServiceItem temp = this->service->services[this->selectedRow].items[this->selectedService];
    this->service->services[this->selectedRow].items[this->selectedService] = 
        this->service->services[this->selectedRow].items[this->selectedService - 1];
    this->service->services[this->selectedRow].items[this->selectedService - 1] = temp;
    
    // Update selection to follow the moved item
    this->selectedService--;
    reloadSecondTable();
    
    // Reselect the moved item
    ui->serviceItemsTable->selectRow(this->selectedService);
}

void MainWindow::on_moveDownService_clicked() {
    if (this->selectedRow < 0 || this->selectedService < 0) { return; }
    if (this->selectedService >= this->service->services[this->selectedRow].items.count() - 1) { return; } // Can't move last item down
    
    // Swap the current item with the one below
    ServiceItem temp = this->service->services[this->selectedRow].items[this->selectedService];
    this->service->services[this->selectedRow].items[this->selectedService] = 
        this->service->services[this->selectedRow].items[this->selectedService + 1];
    this->service->services[this->selectedRow].items[this->selectedService + 1] = temp;
    
    // Update selection to follow the moved item
    this->selectedService++;
    reloadSecondTable();
    
    // Reselect the moved item
    ui->serviceItemsTable->selectRow(this->selectedService);
}
