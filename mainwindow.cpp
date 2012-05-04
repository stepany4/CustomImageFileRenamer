#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <settingsdialog.h>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scale(100)
{
    ui->setupUi(this);

    m_folder = QDir::homePath();

    m_scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(m_scene);

    readSettings();
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete m_scene;
    delete ui;
}

void MainWindow::on_actionOpenFolder_triggered()
{
    m_folder = QFileDialog::getExistingDirectory();
    // получаем список файлов в папке
    QDir dir(m_folder);
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.gif" << "*.tiff" << "*.bmp";
    dir.setNameFilters(filters);
    m_filesList = dir.entryInfoList(QDir::NoFilter, QDir::Name);
    m_currentItemId = 0;

    // показываем первое изображение в папке по алфавиту\дате
    QString fileName = m_filesList.at(0).absoluteFilePath();
    bool exists = QFile::exists(fileName);
    qDebug() << exists;
    qDebug() << fileName;

    updateImage(fileName);
}

void MainWindow::on_actionNext_triggered()
{
    QString fileName = nextFile();
    updateImage(fileName);
}

void MainWindow::on_actionPrev_triggered()
{
    QString fileName = prevFile();
    updateImage(fileName);
}

void MainWindow::on_actionOptions_triggered()
{
    // Добавить настройку масштаба изображения
    // Добавить настройку шаблонов имен файла
    int count = ui->cbTemplate->count();
    QStringList list;
    for (int i = 0; i < count; i++)
        list.append(ui->cbTemplate->itemText(i));

    SettingsDialog dialog(list, m_scale, this);
    int result = dialog.exec();
    if (result == QDialog::Accepted)
    {
        m_scale = dialog.scale();
        ui->cbTemplate->clear();
        list = dialog.templatesList();
        foreach(QString str, list)
            ui->cbTemplate->addItem(str);
        QString fileName = curFile();
        updateImage(fileName);
    }
}

QString MainWindow::nextFile()
{
    if (m_currentItemId >= m_filesList.count()-1)
        return QString();
    else
    {
        m_currentItemId++;
        return m_filesList.at(m_currentItemId).absoluteFilePath();
    }
}

QString MainWindow::prevFile()
{
    if (m_currentItemId <= 0)
        return QString();
    else
    {
        m_currentItemId--;
        return m_filesList.at(m_currentItemId).absoluteFilePath();
    }
}

QString MainWindow::curFile()
{
    if (m_currentItemId < 0 || m_currentItemId >= m_filesList.count())
        return QString();
    else
        return m_filesList.at(m_currentItemId).absoluteFilePath();
}

void MainWindow::on_btnRename_clicked()
{
    QString resultName = updateResultName();

    // переименовываем файл
    QDir dir(m_folder);
    QString sourceFN = dir.absoluteFilePath(ui->lineEditCurName->text());
    qDebug() << sourceFN;
    QFileInfo fi(sourceFN);
    QString suffix = fi.suffix();
    QString destFN = dir.absoluteFilePath(resultName.append(".").append(suffix));
    qDebug() << destFN;
    bool result = QFile::rename(sourceFN, destFN);
    if (!result)
    {
        QMessageBox msgBox;
        msgBox.setText(trUtf8("Не удалось переименовать файл."));
        msgBox.setInformativeText(trUtf8("Файл с таким именем уже существует,"
                                         " перезаписать его?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Ok)
        {
            QFile::remove(sourceFN);
            result = QFile::rename(sourceFN, destFN);
            qDebug() << result;
        }
    }
}

void MainWindow::updateImage(QString absFileName)
{
    m_scene->clear();
    if (QFile::exists(absFileName))
    {
        QFileInfo fi(absFileName);
        QString name = fi.fileName();
        ui->lineEditCurName->setText(name);
        QPixmap pm(absFileName);
        int width = pm.width()*m_scale/100.0;
        m_scene->addPixmap(pm.scaledToWidth(width));
    }
    else
        ui->lineEditCurName->setText("");
}

void MainWindow::writeSettings()
{
    int count = ui->cbTemplate->count();
    QStringList list;
    for (int i = 0; i < count; i++)
        list.append(ui->cbTemplate->itemText(i));

    QSettings settings("BAST", "Image Renamer");

    settings.beginGroup("Options");
    settings.setValue("scale", m_scale);
    settings.setValue("list", list);
    settings.endGroup();

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings()
{
    QSettings settings("BAST", "Image Renamer");

    settings.beginGroup("Options");
    m_scale = settings.value("scale", 100).toInt();
    QStringList list = settings.value("list", QStringList()).toStringList();
    settings.endGroup();

    ui->cbTemplate->clear();
    foreach(QString str, list)
        ui->cbTemplate->addItem(str);

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(1024, 700)).toSize());
    move(settings.value("pos", QPoint(0, 0)).toPoint());
    settings.endGroup();
}

void MainWindow::on_lineEditValue1_textChanged(const QString &/*arg1*/)
{
    updateResultName();
}

void MainWindow::on_lineEditValue2_textChanged(const QString &/*arg1*/)
{
    updateResultName();
}

void MainWindow::on_cbTemplate_currentIndexChanged(const QString &/*arg1*/)
{
    updateResultName();
}

QString MainWindow::updateResultName()
{
    QString templ = ui->cbTemplate->currentText();
    QString resultName = templ
            .arg(ui->lineEditValue1->text())
            .arg(ui->lineEditValue2->text());
    ui->lineEditResult->setText(resultName);
    return resultName;
}
