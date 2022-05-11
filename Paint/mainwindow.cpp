#include <QtWidgets>
#include "mainwindow.h"
#include "scribble.h"

MainWindow::MainWindow()
{
    scribbleArea = new ScribbleArea;
    setCentralWidget(scribbleArea);

    createAction();
    createMenu();

    setWindowTitle(tr("Péjnt"));

    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isSaved()) {
        event->accept();
    } else {

        event->ignore();
    }
}

void MainWindow::open()
{
    if (isSaved()) {

        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Fájl megnyitása"), QDir::currentPath());

        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());

    QByteArray fileFormat = action->data().toByteArray();

    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());

    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;

    int newWidth = QInputDialog::getInt(this, tr("Péjnt"),
                                        tr("Toll vastagság:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::createAction()
{
    openAct = new QAction(tr("Megnyitás"), this);

    openAct->setShortcuts(QKeySequence::Open);

    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);

        action->setData(format);

        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        saveAsAct.append(action);
    }

    printAct = new QAction(tr("Nyomtatás"), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));

    exitAct = new QAction(tr("Kilépés"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("Toll szín"), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Toll vastagság"), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearAct = new QAction(tr("Képernyő törlése"), this);
    connect(clearAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));
}

void MainWindow::createMenu()
{
    saveAs = new QMenu(tr("Mentés mésként"), this);
    foreach (QAction *action, saveAsAct)
        saveAs->addAction(action);

    fileMenu = new QMenu(tr("Fájl"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAs);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    options = new QMenu(tr("Beállítások"), this);
    options->addAction(penColorAct);
    options->addAction(penWidthAct);
    options->addSeparator();
    options->addAction(clearAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(options);
}

bool MainWindow::isSaved()
{
    if (scribbleArea->isModified()) {
        QMessageBox::StandardButton ret;

        ret = QMessageBox::warning(this, tr("Péjnt"),
                                   tr("Módosítások mentése."),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveFile("png");

        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Mentés másként"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty()) {
        return false;
    } else {

        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}
