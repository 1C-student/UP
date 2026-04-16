#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class UserWindow; }
QT_END_NAMESPACE

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    UserWindow(const QString& login, QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void loadMembers();
    void loadEvents();
    void loadCities();
    void loadMyLog();
    void applyFilter();
    void markAttendance();
    void showMemberPhoto(const QModelIndex &index);
    void showHelp();
    void showAbout();
    void logout();

private:
    Ui::UserWindow *ui;
    QSqlQueryModel *membersModel;
    QSqlTableModel *eventsModel;
    QSqlTableModel *citiesModel;
    QSqlTableModel *myLogModel;
    QString currentUserLogin;
    int currentUserId;

    void refreshAll();
};

#endif
