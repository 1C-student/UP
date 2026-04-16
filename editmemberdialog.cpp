#include "editmemberdialog.h"
#include "ui_editmemberdialog.h"
#include "database.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QPixmap>
#include <QFile>
#include <QDir>

EditMemberDialog::EditMemberDialog(QWidget *parent, int memberId)
    : QDialog(parent)
    , ui(new Ui::EditMemberDialog)
    , currentMemberId(memberId)
{
    ui->setupUi(this);
    loadCities();
    connect(ui->selectPhotoBtn, &QPushButton::clicked, this, &EditMemberDialog::selectPhoto);
    connect(ui->saveBtn, &QPushButton::clicked, this, &EditMemberDialog::save);

    if (memberId != -1) loadMemberData();
}

EditMemberDialog::~EditMemberDialog()
{
    delete ui;
}

void EditMemberDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    if (!selectedPhotoPath.isEmpty() && QFile::exists(selectedPhotoPath)) {
        QPixmap pix(selectedPhotoPath);
        ui->photoPreview->setPixmap(pix.scaled(ui->photoPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void EditMemberDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    if (!selectedPhotoPath.isEmpty() && QFile::exists(selectedPhotoPath)) {
        QPixmap pix(selectedPhotoPath);
        ui->photoPreview->setPixmap(pix.scaled(ui->photoPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void EditMemberDialog::loadCities()
{
    QSqlQuery q = Database::getInstance().execute("SELECT id, name FROM cities ORDER BY name");
    while (q.next()) {
        ui->cityCombo->addItem(q.value(1).toString(), q.value(0).toInt());
    }
}

void EditMemberDialog::loadMemberData()
{
    QSqlQuery q = Database::getInstance().execute(
        "SELECT full_name, city_id, photo_path FROM members WHERE id = :id",
        {currentMemberId}
        );
    if (q.next()) {
        ui->fioEdit->setText(q.value(0).toString());
        int cityId = q.value(1).toInt();
        for (int i = 0; i < ui->cityCombo->count(); ++i) {
            if (ui->cityCombo->itemData(i).toInt() == cityId) {
                ui->cityCombo->setCurrentIndex(i);
                break;
            }
        }
        selectedPhotoPath = q.value(2).toString();
        if (!selectedPhotoPath.isEmpty() && QFile::exists(selectedPhotoPath)) {
            ui->photoPathLabel->setText(selectedPhotoPath);
            QPixmap pix(selectedPhotoPath);
            ui->photoPreview->setPixmap(pix.scaled(ui->photoPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void EditMemberDialog::selectPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите фото", "", "Images (*.jpg *.jpeg *.png)");
    if (fileName.isEmpty()) return;

    QDir dir("photos");
    if (!dir.exists()) dir.mkdir("photos");

    QString newPath = "photos/" + QFileInfo(fileName).fileName();

    if (QFile::exists(newPath)) {
        QFile::remove(newPath);
    }

    if (!QFile::copy(fileName, newPath)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось скопировать файл");
        return;
    }

    selectedPhotoPath = newPath;

    ui->photoPathLabel->setText(selectedPhotoPath);

    QPixmap pix;
    if (pix.load(selectedPhotoPath)) {
        ui->photoPreview->setPixmap(pix.scaled(ui->photoPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        ui->photoPreview->setText("Ошибка загрузки");
        ui->photoPreview->setAlignment(Qt::AlignCenter);
    }
}

void EditMemberDialog::save()
{
    QString fio = ui->fioEdit->text().trimmed();
    if (fio.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите ФИО");
        return;
    }

    int cityId = ui->cityCombo->currentData().toInt();

    if (currentMemberId == -1) {
        Database::getInstance().execute(
            "INSERT INTO members (full_name, city_id, photo_path) VALUES (:fio, :city, :photo)",
            {fio, cityId, selectedPhotoPath}
            );
    } else {
        Database::getInstance().execute(
            "UPDATE members SET full_name = :fio, city_id = :city, photo_path = :photo WHERE id = :id",
            {fio, cityId, selectedPhotoPath, currentMemberId}
            );
    }

    accept();
}
