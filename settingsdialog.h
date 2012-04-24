#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const QStringList & list, int scale, QWidget *parent = 0);
    ~SettingsDialog();

    void setTemplatesList(const QStringList & list);
    QStringList templatesList();

    void setScale(int scale);
    int scale();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_addTemplateButton_clicked();

    void on_editTemplateButton_clicked();

    void on_delTemplateButton_clicked();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
