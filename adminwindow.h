#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminWindow; }
QT_END_NAMESPACE

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void loadMembers();
    void loadEvents();
    void loadCities();
    void loadLog();
    void applyFilter();
    void addMember();
    void editMember();
    void deleteMember();
    void addEvent();
    void editEvent();
    void deleteEvent();
    void addCity();
    void editCity();
    void deleteCity();
    void addLogEntry();
    void editLogEntry();
    void deleteLogEntry();
    void showHelp();
    void showAbout();
    void logout();

private:
    Ui::AdminWindow *ui;
    QSqlQueryModel *membersModel;
    QSqlTableModel *eventsModel;
    QSqlTableModel *citiesModel;
    QSqlTableModel *logModel;

    void refreshAll();
};

#endif
