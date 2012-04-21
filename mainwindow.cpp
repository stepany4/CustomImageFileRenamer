#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
        m_scene->addPixmap(QPixmap(absFileName));
    }
    else
        ui->lineEditCurName->setText("");
}
