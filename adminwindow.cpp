#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "editmemberdialog.h"
#include "authwindow.h"
#include "helpwindow.h"
#include "aboutwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDateEdit>
#include <QDialog>
#include <QDate>

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);

    membersModel = new QSqlQueryModel(this);
    eventsModel = new QSqlTableModel(this);
    eventsModel->setTable("events");
    eventsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    eventsModel->setHeaderData(0, Qt::Horizontal, "ID");
    eventsModel->setHeaderData(1, Qt::Horizontal, "Название");
    eventsModel->setHeaderData(2, Qt::Horizontal, "Дата");
    ui->eventsTable->setModel(eventsModel);
    ui->eventsTable->horizontalHeader()->setStretchLastSection(true);

    citiesModel = new QSqlTableModel(this);
    citiesModel->setTable("cities");
    citiesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    citiesModel->setHeaderData(0, Qt::Horizontal, "ID");
    citiesModel->setHeaderData(1, Qt::Horizontal, "Название города");
    ui->citiesTable->setModel(citiesModel);
    ui->citiesTable->horizontalHeader()->setStretchLastSection(true);

    logModel = new QSqlTableModel(this);
    logModel->setTable("event_log");
    logModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->logTable->setModel(logModel);

    QStringList columns = {"ID", "ФИО", "Город"};
    ui->filterColumn->addItems(columns);
    QStringList conditions = {"содержит", "равно", "начинается с"};
    ui->filterCondition->addItems(conditions);
    QStringList sort = {"по возрастанию", "по убыванию"};
    ui->filterSort->addItems(sort);

    connect(ui->addMemberBtn, &QPushButton::clicked, this, &AdminWindow::addMember);
    connect(ui->editMemberBtn, &QPushButton::clicked, this, &AdminWindow::editMember);
    connect(ui->deleteMemberBtn, &QPushButton::clicked, this, &AdminWindow::deleteMember);
    connect(ui->addEventBtn, &QPushButton::clicked, this, &AdminWindow::addEvent);
    connect(ui->editEventBtn, &QPushButton::clicked, this, &AdminWindow::editEvent);
    connect(ui->deleteEventBtn, &QPushButton::clicked, this, &AdminWindow::deleteEvent);
    connect(ui->addCityBtn, &QPushButton::clicked, this, &AdminWindow::addCity);
    connect(ui->editCityBtn, &QPushButton::clicked, this, &AdminWindow::editCity);
    connect(ui->deleteCityBtn, &QPushButton::clicked, this, &AdminWindow::deleteCity);
    connect(ui->addLogBtn, &QPushButton::clicked, this, &AdminWindow::addLogEntry);
    connect(ui->editLogBtn, &QPushButton::clicked, this, &AdminWindow::editLogEntry);
    connect(ui->deleteLogBtn, &QPushButton::clicked, this, &AdminWindow::deleteLogEntry);
    connect(ui->applyFilterBtn, &QPushButton::clicked, this, &AdminWindow::applyFilter);
    connect(ui->helpBtn, &QPushButton::clicked, this, &AdminWindow::showHelp);
    connect(ui->aboutBtn, &QPushButton::clicked, this, &AdminWindow::showAbout);
    connect(ui->logoutBtn, &QPushButton::clicked, this, &AdminWindow::logout);

    refreshAll();
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::loadMembers()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT m.id, m.full_name, m.photo_path, c.name as city_name "
        "FROM members m "
        "LEFT JOIN cities c ON m.city_id = c.id"
        );
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ФИО");
    model->setHeaderData(3, Qt::Horizontal, "Город");
    ui->membersTable->setModel(model);
    ui->membersTable->setColumnHidden(2, true);
    ui->membersTable->horizontalHeader()->setStretchLastSection(true);

    delete membersModel;
    membersModel = model;
}

void AdminWindow::loadEvents()
{
    eventsModel->select();
}

void AdminWindow::loadCities()
{
    citiesModel->select();
}

void AdminWindow::loadLog()
{
    logModel->select();
}

void AdminWindow::applyFilter()
{
    QString column = ui->filterColumn->currentText();
    QString condition = ui->filterCondition->currentText();
    QString value = ui->filterValue->text();
    QString sortOrder = ui->filterSort->currentText();

    QString filterStr;
    QString colName;
    if (column == "ID") colName = "m.id";
    else if (column == "ФИО") colName = "m.full_name";
    else colName = "c.name";

    if (!value.isEmpty()) {
        if (condition == "содержит") filterStr = QString("%1::text LIKE '%%2%'").arg(colName).arg(value);
        else if (condition == "равно") filterStr = QString("%1::text = '%2'").arg(colName).arg(value);
        else if (condition == "начинается с") filterStr = QString("%1::text LIKE '%2%'").arg(colName).arg(value);
    }

    QString query = "SELECT m.id, m.full_name, m.photo_path, c.name as city_name "
                    "FROM members m "
                    "LEFT JOIN cities c ON m.city_id = c.id";
    if (!filterStr.isEmpty()) query += " WHERE " + filterStr;
    if (sortOrder == "по возрастанию") query += " ORDER BY m.id ASC";
    else query += " ORDER BY m.id DESC";

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ФИО");
    model->setHeaderData(3, Qt::Horizontal, "Город");
    ui->membersTable->setModel(model);
    ui->membersTable->setColumnHidden(2, true);

    delete membersModel;
    membersModel = model;
}

void AdminWindow::addMember()
{
    EditMemberDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) refreshAll();
}

void AdminWindow::editMember()
{
    int row = ui->membersTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите члена партии");
        return;
    }
    int id = membersModel->data(membersModel->index(row, 0)).toInt();
    EditMemberDialog dlg(this, id);
    if (dlg.exec() == QDialog::Accepted) refreshAll();
}

void AdminWindow::deleteMember()
{
    int row = ui->membersTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите члена партии");
        return;
    }
    if (QMessageBox::question(this, "Удаление", "Удалить члена партии?") == QMessageBox::Yes) {
        int id = membersModel->data(membersModel->index(row, 0)).toInt();
        Database::getInstance().execute("DELETE FROM members WHERE id = :id", {id});
        refreshAll();
    }
}

void AdminWindow::addEvent()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить мероприятие");
    QVBoxLayout layout(&dialog);
    QFormLayout form;

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    nameEdit->setPlaceholderText("Введите название мероприятия");
    form.addRow("Название:", nameEdit);

    QDateEdit *dateEdit = new QDateEdit(&dialog);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    form.addRow("Дата:", dateEdit);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout.addLayout(&form);
    layout.addWidget(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название мероприятия");
            return;
        }
        QString date = dateEdit->date().toString("yyyy-MM-dd");
        Database::getInstance().execute(
            "INSERT INTO events (name, event_date) VALUES (:name, :date)",
            {name, date}
            );
        refreshAll();
    }
}

void AdminWindow::editEvent()
{
    int row = ui->eventsTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите мероприятие");
        return;
    }
    int id = eventsModel->data(eventsModel->index(row, 0)).toInt();
    QString oldName = eventsModel->data(eventsModel->index(row, 1)).toString();
    QString oldDate = eventsModel->data(eventsModel->index(row, 2)).toString();

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать мероприятие");
    QVBoxLayout layout(&dialog);
    QFormLayout form;

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    nameEdit->setText(oldName);
    form.addRow("Название:", nameEdit);

    QDateEdit *dateEdit = new QDateEdit(&dialog);
    dateEdit->setDate(QDate::fromString(oldDate, "yyyy-MM-dd"));
    dateEdit->setCalendarPopup(true);
    form.addRow("Дата:", dateEdit);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout.addLayout(&form);
    layout.addWidget(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = nameEdit->text().trimmed();
        if (newName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название мероприятия");
            return;
        }
        QString newDate = dateEdit->date().toString("yyyy-MM-dd");
        Database::getInstance().execute(
            "UPDATE events SET name = :name, event_date = :date WHERE id = :id",
            {newName, newDate, id}
            );
        refreshAll();
    }
}

void AdminWindow::deleteEvent()
{
    int row = ui->eventsTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите мероприятие");
        return;
    }

    QSqlQuery check = Database::getInstance().execute(
        "SELECT id FROM event_log WHERE event_id = :eventId LIMIT 1",
        {eventsModel->data(eventsModel->index(row, 0)).toInt()}
        );

    if (check.next()) {
        QMessageBox::warning(this, "Ошибка", "Нельзя удалить мероприятие, так как есть записи в журнале выходов");
        return;
    }

    if (QMessageBox::question(this, "Удаление", "Удалить мероприятие?") == QMessageBox::Yes) {
        int id = eventsModel->data(eventsModel->index(row, 0)).toInt();
        Database::getInstance().execute("DELETE FROM events WHERE id = :id", {id});
        refreshAll();
    }
}

void AdminWindow::addCity()
{
    bool ok;
    QString cityName = QInputDialog::getText(this, "Добавить город", "Название города:", QLineEdit::Normal, "", &ok);
    if (ok && !cityName.isEmpty()) {
        Database::getInstance().execute(
            "INSERT INTO cities (name) VALUES (:name)", {cityName}
            );
        refreshAll();
    }
}

void AdminWindow::editCity()
{
    int row = ui->citiesTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите город");
        return;
    }
    int id = citiesModel->data(citiesModel->index(row, 0)).toInt();
    QString oldName = citiesModel->data(citiesModel->index(row, 1)).toString();
    bool ok;
    QString newName = QInputDialog::getText(this, "Редактировать город", "Новое название:", QLineEdit::Normal, oldName, &ok);
    if (ok && !newName.isEmpty()) {
        Database::getInstance().execute(
            "UPDATE cities SET name = :name WHERE id = :id", {newName, id}
            );
        refreshAll();
    }
}

void AdminWindow::deleteCity()
{
    int row = ui->citiesTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите город");
        return;
    }
    int id = citiesModel->data(citiesModel->index(row, 0)).toInt();

    QSqlQuery check = Database::getInstance().execute(
        "SELECT id FROM members WHERE city_id = :cityId LIMIT 1", {id}
        );

    if (check.next()) {
        QMessageBox::warning(this, "Ошибка", "Нельзя удалить город, так как есть члены партии из этого города");
        return;
    }

    if (QMessageBox::question(this, "Удаление", "Удалить город?") == QMessageBox::Yes) {
        Database::getInstance().execute("DELETE FROM cities WHERE id = :id", {id});
        refreshAll();
    }
}

void AdminWindow::addLogEntry()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить запись в журнал");
    QVBoxLayout layout(&dialog);
    QFormLayout form;

    QComboBox *memberCombo = new QComboBox(&dialog);
    QSqlQuery memberQuery = Database::getInstance().execute(
        "SELECT id, full_name FROM members ORDER BY full_name"
        );
    while (memberQuery.next()) {
        memberCombo->addItem(memberQuery.value(1).toString(), memberQuery.value(0).toInt());
    }

    QComboBox *eventCombo = new QComboBox(&dialog);
    QSqlQuery eventQuery = Database::getInstance().execute(
        "SELECT id, name FROM events ORDER BY name"
        );
    while (eventQuery.next()) {
        eventCombo->addItem(eventQuery.value(1).toString(), eventQuery.value(0).toInt());
    }

    QDateEdit *dateEdit = new QDateEdit(&dialog);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setCalendarPopup(true);

    form.addRow("Член партии:", memberCombo);
    form.addRow("Мероприятие:", eventCombo);
    form.addRow("Дата:", dateEdit);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout.addLayout(&form);
    layout.addWidget(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int memberId = memberCombo->currentData().toInt();
        int eventId = eventCombo->currentData().toInt();
        QString date = dateEdit->date().toString("yyyy-MM-dd");

        QSqlQuery check = Database::getInstance().execute(
            "SELECT id FROM event_log WHERE member_id = :mid AND event_id = :eid",
            {memberId, eventId}
            );

        if (check.next()) {
            QMessageBox::warning(this, "Ошибка", "Такая запись уже существует");
            return;
        }

        Database::getInstance().execute(
            "INSERT INTO event_log (member_id, event_id, visit_date) VALUES (:mid, :eid, :date)",
            {memberId, eventId, date}
            );
        refreshAll();
    }
}

void AdminWindow::editLogEntry()
{
    int row = ui->logTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите запись");
        return;
    }

    int logId = logModel->data(logModel->index(row, 0)).toInt();
    int oldMemberId = logModel->data(logModel->index(row, 1)).toInt();
    int oldEventId = logModel->data(logModel->index(row, 2)).toInt();
    QString oldDate = logModel->data(logModel->index(row, 3)).toString();

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать запись");
    QVBoxLayout layout(&dialog);
    QFormLayout form;

    QComboBox *memberCombo = new QComboBox(&dialog);
    QSqlQuery memberQuery = Database::getInstance().execute(
        "SELECT id, full_name FROM members ORDER BY full_name"
        );
    int memberIndex = 0;
    while (memberQuery.next()) {
        memberCombo->addItem(memberQuery.value(1).toString(), memberQuery.value(0).toInt());
        if (memberQuery.value(0).toInt() == oldMemberId) {
            memberIndex = memberCombo->count() - 1;
        }
    }
    memberCombo->setCurrentIndex(memberIndex);

    QComboBox *eventCombo = new QComboBox(&dialog);
    QSqlQuery eventQuery = Database::getInstance().execute(
        "SELECT id, name FROM events ORDER BY name"
        );
    int eventIndex = 0;
    while (eventQuery.next()) {
        eventCombo->addItem(eventQuery.value(1).toString(), eventQuery.value(0).toInt());
        if (eventQuery.value(0).toInt() == oldEventId) {
            eventIndex = eventCombo->count() - 1;
        }
    }
    eventCombo->setCurrentIndex(eventIndex);

    QDateEdit *dateEdit = new QDateEdit(&dialog);
    dateEdit->setDate(QDate::fromString(oldDate, "yyyy-MM-dd"));
    dateEdit->setCalendarPopup(true);

    form.addRow("Член партии:", memberCombo);
    form.addRow("Мероприятие:", eventCombo);
    form.addRow("Дата:", dateEdit);

    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout.addLayout(&form);
    layout.addWidget(&buttons);

    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        int newMemberId = memberCombo->currentData().toInt();
        int newEventId = eventCombo->currentData().toInt();
        QString newDate = dateEdit->date().toString("yyyy-MM-dd");

        QSqlQuery check = Database::getInstance().execute(
            "SELECT id FROM event_log WHERE member_id = :mid AND event_id = :eid AND id != :id",
            {newMemberId, newEventId, logId}
            );

        if (check.next()) {
            QMessageBox::warning(this, "Ошибка", "Такая запись уже существует");
            return;
        }

        Database::getInstance().execute(
            "UPDATE event_log SET member_id = :mid, event_id = :eid, visit_date = :date WHERE id = :id",
            {newMemberId, newEventId, newDate, logId}
            );
        refreshAll();
    }
}

void AdminWindow::deleteLogEntry()
{
    int row = ui->logTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите запись");
        return;
    }

    if (QMessageBox::question(this, "Удаление", "Удалить запись из журнала?") == QMessageBox::Yes) {
        int id = logModel->data(logModel->index(row, 0)).toInt();
        Database::getInstance().execute("DELETE FROM event_log WHERE id = :id", {id});
        refreshAll();
    }
}

void AdminWindow::showHelp()
{
    HelpWindow *help = new HelpWindow(this);
    help->exec();
}

void AdminWindow::showAbout()
{
    AboutWindow *about = new AboutWindow(this);
    about->exec();
}

void AdminWindow::refreshAll()
{
    loadMembers();
    loadEvents();
    loadCities();
    loadLog();
}

void AdminWindow::logout()
{
    this->close();
    AuthWindow *auth = new AuthWindow();
    auth->show();
}
