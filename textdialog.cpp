#include "textdialog.h"
#include "ui_textdialog.h"

TextDialog::TextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDialog)
{
    ui->setupUi(this);
}

TextDialog::~TextDialog()
{
    delete ui;
}

void TextDialog::on_buttonBox_accepted()
{
    accept();
}

void TextDialog::on_buttonBox_rejected()
{
    reject();
}

void TextDialog::setText(QString text)
{
    ui->lineEdit->setText(text);
}

QString TextDialog::text()
{
    return ui->lineEdit->text();
}
