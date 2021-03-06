#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->pushButton, SIGNAL(clicked(bool)), SLOT(choosePng()));
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

QFileInfoList MainWindow::getFileListByDir(const QString &dpath, const QStringList &filter, bool recursively)
{
    QDir dir(dpath);
    if(!dir.exists())
    {
        return QFileInfoList();
    }

    QFileInfoList fileList = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    if(recursively)
    {
        const QFileInfoList& folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach(const QFileInfo &fileInfo, folderList)
        {
            fileList.append( getFileListByDir(fileInfo.absoluteFilePath(), filter, recursively) );
        }
    }

    return fileList;
}

void MainWindow::choosePng()
{
    const QString &dir = QFileDialog::getExistingDirectory(this, "Please Choose PNG Dir First!");
    if(dir.isEmpty())
    {
        return;
    }

    QStringList filters;
    filters << "*.png";
    foreach(const QFileInfo &info, getFileListByDir(dir, filters, true))
    {
        QImage image(info.absoluteFilePath());
        if(image.isNull())
        {
            continue;
        }

        if(!image.save(info.absoluteFilePath(), "png"))
        {
            QMessageBox::warning(this, "Error", QString("%1 save fail").arg(info.absoluteFilePath()));
        }
    }

    QMessageBox::information(this, "Done", "All png image processing is completed");
}
