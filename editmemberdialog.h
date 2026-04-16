#ifndef EDITMEMBERDIALOG_H
#define EDITMEMBERDIALOG_H

#include <QDialog>
#include <QResizeEvent>
#include <QShowEvent>

namespace Ui {
class EditMemberDialog;
}

class EditMemberDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMemberDialog(QWidget *parent = nullptr, int memberId = -1);
    ~EditMemberDialog();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void selectPhoto();
    void save();

private:
    Ui::EditMemberDialog *ui;
    int currentMemberId;
    QString selectedPhotoPath;

    void loadCities();
    void loadMemberData();
};

#endif
