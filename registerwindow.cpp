#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "database.h"
#include <QMessageBox>
#include <QSqlQuery>

RegisterWindow::RegisterWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    connect(ui->registerBtn, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::onRegisterClicked()
{
    QString login = ui->loginEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString confirm = ui->confirmEdit->text();
    QString fio = ui->fioEdit->text().trimmed();

    if (login.isEmpty() || password.isEmpty() || fio.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают");
        return;
    }

    if (password.length() < 4) {
        QMessageBox::warning(this, "Ошибка", "Пароль должен быть не менее 4 символов");
        return;
    }

    QSqlQuery check = Database::getInstance().execute(
        "SELECT id FROM users WHERE login = :login", {login}
        );

    if (check.next()) {
        QMessageBox::warning(this, "Ошибка", "Пользователь с таким логином уже существует");
        return;
    }

    QString hashedPassword = Database::getInstance().hashPassword(password);

    Database::getInstance().execute(
        "INSERT INTO users (login, password, role) VALUES (:login, :password, 'user')",
        {login, hashedPassword}
        );

    QMessageBox::information(this, "Успех", "Регистрация завершена. Теперь вы можете войти.");
    emit backToAuth();
    this->close();
}
