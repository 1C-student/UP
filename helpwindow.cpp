#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QMessageBox>

HelpWindow::HelpWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Справка");

    connect(ui->exportPdfBtn, &QPushButton::clicked, this, &HelpWindow::exportToPdf);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::exportToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить справку", "Справка_Партийная_работа.pdf", "PDF files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QTextDocument doc;

    QString html;
    html += "<html><head><meta charset='UTF-8'><title>Справка - Партийная работа</title>";
    html += "<style>";
    html += "body { font-family: 'Segoe UI', Arial, sans-serif; font-size: 12pt; margin: 20px; }";
    html += "h1 { color: #2c3e50; font-size: 24pt; border-bottom: 2px solid #3498db; }";
    html += "h2 { color: #2980b9; font-size: 18pt; margin-top: 20px; }";
    html += "h3 { color: #16a085; font-size: 14pt; margin-top: 15px; }";
    html += "ul, ol { margin-left: 20px; }";
    html += "li { margin: 5px 0; }";
    html += "b { color: #e74c3c; }";
    html += ".header { text-align: center; margin-bottom: 30px; }";
    html += ".footer { text-align: center; margin-top: 40px; font-size: 10pt; color: #7f8c8d; border-top: 1px solid #bdc3c7; padding-top: 10px; }";
    html += "</style>";
    html += "</head><body>";

    html += "<div class='header'>";
    html += "<h1>Партийная работа</h1>";
    html += "<h2>Руководство пользователя</h2>";
    html += "<p>Версия 1.0 | © 2026</p>";
    html += "</div>";

    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        QWidget *tab = ui->tabWidget->widget(i);
        QTextBrowser *browser = tab->findChild<QTextBrowser *>();
        if (browser) {
            html += "<div style='page-break-before: avoid;'>";
            html += browser->toHtml();
            html += "</div>";
        }
    }

    html += "<div class='footer'>";
    html += "<p>Программа разработана для автоматизации учета членов партии, мероприятий и контроля посещаемости.</p>";
    html += "<p>Дата создания: " + QDate::currentDate().toString("dd.MM.yyyy") + "</p>";
    html += "</div>";

    html += "</body></html>";

    doc.setHtml(html);

    doc.print(&printer);

    QMessageBox::information(this, "Успех", "Справка успешно сохранена в PDF файл");
}
