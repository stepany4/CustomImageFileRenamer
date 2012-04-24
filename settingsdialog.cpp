#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "textdialog.h"
#include <QDialog>

SettingsDialog::SettingsDialog(const QStringList & list, int scale, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->scaleComboBox->addItem("10%", 10);
    ui->scaleComboBox->addItem("25%", 25);
    ui->scaleComboBox->addItem("50%", 50);
    ui->scaleComboBox->addItem("100%", 100);
    setScale(scale);
    setTemplatesList(list);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setTemplatesList(const QStringList &list)
{
    ui->templatesListWidget->clear();
    ui->templatesListWidget->addItems(list);
}

QStringList SettingsDialog::templatesList()
{
    QList<QListWidgetItem *> widgList = ui->templatesListWidget->findItems("", Qt::MatchContains);
    QStringList list;
    foreach (QListWidgetItem * widg, widgList)
        list.append(widg->text());
    return list;
}

void SettingsDialog::setScale(int scale)
{
    int id = ui->scaleComboBox->findData(scale);
    if (id != -1)
        ui->scaleComboBox->setCurrentIndex(id);
}

int SettingsDialog::scale()
{
    int id = ui->scaleComboBox->currentIndex();
    return ui->scaleComboBox->itemData(id).toInt();
}

void SettingsDialog::on_buttonBox_accepted()
{
    accept();
}

void SettingsDialog::on_buttonBox_rejected()
{
    reject();
}

void SettingsDialog::on_addTemplateButton_clicked()
{
    TextDialog dialog(this);
    dialog.setText("");
    if (dialog.exec() == QDialog::Accepted)
        ui->templatesListWidget->addItem(dialog.text());
}

void SettingsDialog::on_editTemplateButton_clicked()
{
    TextDialog dialog(this);
    dialog.setText(ui->templatesListWidget->currentItem()->text());
    if (dialog.exec() == QDialog::Accepted)
        ui->templatesListWidget->currentItem()->setText(dialog.text());
}

void SettingsDialog::on_delTemplateButton_clicked()
{
    int row = ui->templatesListWidget->currentRow();
    ui->templatesListWidget->takeItem(row);
}
