#include "userwindow.h"
#include "ui_userwindow.h"
#include "authwindow.h"
#include "helpwindow.h"
#include "aboutwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFile>
#include <QSqlQuery>

UserWindow::UserWindow(const QString& login, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
    , currentUserLogin(login)
    , membersModel(nullptr)
{
    ui->setupUi(this);

    connect(ui->applyFilterBtn, &QPushButton::clicked, this, &UserWindow::applyFilter);
    connect(ui->markBtn, &QPushButton::clicked, this, &UserWindow::markAttendance);
    connect(ui->helpBtn, &QPushButton::clicked, this, &UserWindow::showHelp);
    connect(ui->aboutBtn, &QPushButton::clicked, this, &UserWindow::showAbout);
    connect(ui->logoutBtn, &QPushButton::clicked, this, &UserWindow::logout);

    QSqlQuery q = Database::getInstance().execute(
        "SELECT id FROM users WHERE login = :login", {login}
        );
    if (q.next()) currentUserId = q.value(0).toInt();

    eventsModel = new QSqlTableModel(this);
    eventsModel->setTable("events");
    eventsModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    eventsModel->setHeaderData(1, Qt::Horizontal, "Название");
    eventsModel->setHeaderData(2, Qt::Horizontal, "Дата");
    ui->eventsTable->setModel(eventsModel);
    ui->eventsTable->setColumnHidden(0, true);
    ui->eventsTable->horizontalHeader()->setStretchLastSection(true);

    citiesModel = new QSqlTableModel(this);
    citiesModel->setTable("cities");
    citiesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    citiesModel->setHeaderData(1, Qt::Horizontal, "Город");
    ui->citiesTable->setModel(citiesModel);
    ui->citiesTable->setColumnHidden(0, true);
    ui->citiesTable->horizontalHeader()->setStretchLastSection(true);

    myLogModel = new QSqlTableModel(this);
    myLogModel->setTable("event_log");
    myLogModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->myLogTable->setModel(myLogModel);

    QStringList columns = {"ФИО", "Город"};
    ui->filterColumn->addItems(columns);
    QStringList conditions = {"содержит", "равно", "начинается с"};
    ui->filterCondition->addItems(conditions);
    QStringList sort = {"по возрастанию", "по убыванию"};
    ui->filterSort->addItems(sort);

    refreshAll();
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::loadMembers()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT m.id, m.full_name, m.photo_path, c.name as city_name "
        "FROM members m "
        "LEFT JOIN cities c ON m.city_id = c.id"
        );
    model->setHeaderData(1, Qt::Horizontal, "ФИО");
    model->setHeaderData(3, Qt::Horizontal, "Город");
    ui->membersTable->setModel(model);
    ui->membersTable->setColumnHidden(0, true);
    ui->membersTable->setColumnHidden(2, true);
    ui->membersTable->horizontalHeader()->setStretchLastSection(true);

    delete membersModel;
    membersModel = model;

    if (ui->membersTable->selectionModel()) {
        connect(ui->membersTable->selectionModel(), &QItemSelectionModel::currentChanged,
                this, &UserWindow::showMemberPhoto, Qt::UniqueConnection);
    }
}

void UserWindow::loadEvents()
{
    eventsModel->select();
}

void UserWindow::loadCities()
{
    citiesModel->select();
}

void UserWindow::loadMyLog()
{
    myLogModel->setFilter(QString("member_id = %1").arg(currentUserId));
    myLogModel->select();
}

void UserWindow::applyFilter()
{
    QString column = ui->filterColumn->currentText();
    QString condition = ui->filterCondition->currentText();
    QString value = ui->filterValue->text();
    QString sortOrder = ui->filterSort->currentText();

    QString filterStr;
    QString colName = (column == "ФИО") ? "m.full_name" : "c.name";

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
    model->setHeaderData(1, Qt::Horizontal, "ФИО");
    model->setHeaderData(3, Qt::Horizontal, "Город");
    ui->membersTable->setModel(model);
    ui->membersTable->setColumnHidden(0, true);
    ui->membersTable->setColumnHidden(2, true);

    delete membersModel;
    membersModel = model;
}

void UserWindow::showMemberPhoto(const QModelIndex &index)
{
    int row = index.row();
    QString photoPath = membersModel->data(membersModel->index(row, 2)).toString();
    QPixmap pix;
    if (QFile::exists(photoPath)) pix.load(photoPath);
    ui->photoLabel->setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatio));
}

void UserWindow::markAttendance()
{
    int row = ui->eventsTable->currentIndex().row();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите мероприятие");
        return;
    }
    int eventId = eventsModel->data(eventsModel->index(row, 0)).toInt();
    QString eventName = eventsModel->data(eventsModel->index(row, 1)).toString();

    QSqlQuery check = Database::getInstance().execute(
        "SELECT id FROM event_log WHERE member_id = :mid AND event_id = :eid",
        {currentUserId, eventId}
        );

    if (check.next()) {
        QMessageBox::warning(this, "Ошибка", "Вы уже отметились на это мероприятие");
        return;
    }

    Database::getInstance().execute(
        "INSERT INTO event_log (member_id, event_id, visit_date) VALUES (:mid, :eid, CURRENT_DATE)",
        {currentUserId, eventId}
        );

    QMessageBox::information(this, "Успех", QString("Вы отметились на: %1").arg(eventName));
    loadMyLog();
}

void UserWindow::showHelp()
{
    HelpWindow *help = new HelpWindow(this);
    help->exec();
}

void UserWindow::showAbout()
{
    AboutWindow *about = new AboutWindow(this);
    about->exec();
}

void UserWindow::refreshAll()
{
    loadMembers();
    loadEvents();
    loadCities();
    loadMyLog();
}

void UserWindow::logout()
{
    this->close();
    AuthWindow *auth = new AuthWindow();
    auth->show();
}
