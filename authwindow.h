#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AuthWindow; }
QT_END_NAMESPACE

class AuthWindow : public QWidget
{
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onHelpClicked();
    void onAboutClicked();

private:
    Ui::AuthWindow *ui;
};

#endif
