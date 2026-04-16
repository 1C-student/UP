#include "authwindow.h"
#include "ui_authwindow.h"
#include "adminwindow.h"
#include "userwindow.h"
#include "registerwindow.h"
#include "helpwindow.h"
#include "aboutwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QSqlQuery>

AuthWindow::AuthWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &AuthWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &AuthWindow::onRegisterClicked);
    connect(ui->helpButton, &QPushButton::clicked, this, &AuthWindow::onHelpClicked);
    connect(ui->aboutButton, &QPushButton::clicked, this, &AuthWindow::onAboutClicked);
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

void AuthWindow::onLoginClicked()
{
    QString login = ui->loginEdit->text();
    QString password = Database::getInstance().hashPassword(ui->passwordEdit->text());

    QSqlQuery q = Database::getInstance().execute(
        "SELECT role FROM users WHERE login = :login AND password = :password",
        {login, password}
        );

    if (q.next()) {
        QString role = q.value(0).toString();
        if (role == "admin") {
            AdminWindow *adminWin = new AdminWindow();
            adminWin->show();
        } else {
            UserWindow *userWin = new UserWindow(login);
            userWin->show();
        }
        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
}

void AuthWindow::onRegisterClicked()
{
    RegisterWindow *regWin = new RegisterWindow();
    connect(regWin, &RegisterWindow::backToAuth, this, [this]() {
        this->show();
    });
    regWin->show();
    this->hide();
}

void AuthWindow::onHelpClicked()
{
    HelpWindow *help = new HelpWindow(this);
    help->exec();
}

void AuthWindow::onAboutClicked()
{
    AboutWindow *about = new AboutWindow(this);
    about->exec();
}
