#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <settingsdialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_scale(100)
{
    ui->setupUi(this);

    m_folder = QDir::homePath();

    m_scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow()
{
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
    QString templ = ui->cbTemplate->currentText();
    QString resultName = templ
            .arg(ui->lineEditValue1->text())
            .arg(ui->lineEditValue2->text());
    ui->labelResult->setText(resultName);
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
