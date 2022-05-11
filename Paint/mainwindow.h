#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    void createMenu();
    void createAction();
    bool isSaved();
    bool saveFile( const QByteArray & file);

    ScribbleArea *scribbleArea;

    QMenu * saveAs;
    QMenu * fileMenu;
    QMenu * options;

    QAction * openAct;
    QAction *exitAct;
    QList <QAction*> saveAsAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction * printAct;
    QAction * clearAct;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
};
#endif // MAINWINDOW_H
