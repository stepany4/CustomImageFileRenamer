#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileInfoList>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpenFolder_triggered();

    void on_actionNext_triggered();

    void on_actionPrev_triggered();

    void on_actionOptions_triggered();

    void on_btnRename_clicked();

    void on_lineEditValue1_textChanged(const QString &arg1);

    void on_lineEditValue2_textChanged(const QString &arg1);

    void on_cbTemplate_currentIndexChanged(const QString &arg1);

private:
    QString nextFile();
    QString prevFile();
    QString curFile();
    void updateImage(QString fileName);

    void readSettings();
    void writeSettings();

    QString updateResultName();

private:
    Ui::MainWindow *ui;

    QString m_folder;
    QGraphicsScene * m_scene;
    QFileInfoList m_filesList;
    int m_currentItemId;
    int m_scale;
};

#endif // MAINWINDOW_H
