#include <QtGui>
#include<QMessageBox>
#include<QFileDialog>
#include<QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
#include "spreadsheet.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    spreadsheet = new Spreadsheet;
    setCentralWidget(spreadsheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    findDialog = 0;

    setWindowIcon(QIcon(":/images/icon.png"));
    setCurrentFile("");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setStatusTip(tr("Open a spreadsheet file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setStatusTip(tr("Save data to current spreadsheet file"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As"), this);
    saveAsAction->setStatusTip(tr("Save data to a new spreadsheet file"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for( int i = 0; i < MaxRecentFiles; i++){
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }


    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    cutAction = new QAction(tr("&Cut"), this);
    cutAction->setIcon(QIcon(":/images/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), spreadsheet, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), spreadsheet, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), spreadsheet, SLOT(paste()));

    delAction = new QAction(tr("&Delete"), this);
    delAction->setShortcut(QKeySequence::Delete);
    connect(delAction, SIGNAL(triggered()), spreadsheet, SLOT(del()));

    findAction = new QAction(tr("&Find"), this);
    findAction->setIcon(QIcon(":/images/find.png"));
    findAction->setStatusTip(tr("Find cells of a specified word"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

    goToCellAction = new QAction(tr("&Go to Cell"), this);
    goToCellAction->setIcon(QIcon(":/images/gotocell.png"));
    goToCellAction->setStatusTip(tr("Go to a specified cell by location"));
    connect(goToCellAction, SIGNAL(triggered()), this, SLOT(goToCell()));


    recalculateAction = new QAction(tr("&Recalculate"), this);
    recalculateAction->setStatusTip("Recalculate all the result of cells");
    connect(recalculateAction, SIGNAL(triggered()),
            spreadsheet, SLOT(recalculate()));

    sortAction = new QAction(tr("&Sort"), this);
    sortAction->setStatusTip(
                "Sort some cells by ascending or descending order");
    connect(sortAction, SIGNAL(triggered()), this, SLOT(sort()));


    showGridAction = new QAction(tr("&Show Grid"), this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(spreadsheet->showGrid());
    connect(showGridAction, SIGNAL(toggled(bool)),
            spreadsheet, SLOT(setShowGrid(bool)));

    autoRecalculateAction = new QAction(tr("&Auto-Recalculate"), this);
    autoRecalculateAction->setCheckable(true);
    autoRecalculateAction->setChecked(spreadsheet->autoRecalculate());
    connect(showGridAction, SIGNAL(toggled(bool)),
            spreadsheet, SLOT(setAutoRecalculate(bool)));


    aboutAction = new QAction(tr("&About me"), this);
    aboutAction->setStatusTip(tr("Show information about application and developer"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About Box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(delAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalculateAction);
    toolsMenu->addAction(sortAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalculateAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

}

void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignHCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel, 1);

    connect(spreadsheet, SIGNAL(currentCellChanged(int,int,int,int)),
            this, SLOT(updateStatusBar()));
    connect(spreadsheet, SIGNAL(modified()), this,
            SLOT(spreadsheetModified()));
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spreadsheet->currentLocation());
    formulaLabel->setText(spreadsheet->currentFormula());
}

void MainWindow::spreadsheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

// Implementation of File Menu
void MainWindow::newFile()
{
    if(okToContinue()){
        spreadsheet->clear();
        setCurrentFile("");
    }
}
void MainWindow::open()
{
    if(okToContinue()){
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Spreadsheet"), ".",
                                                        tr("Spreadsheet files (*.sp)"));
        if(!fileName.isEmpty())
            loadFile(fileName);
    }
}
bool MainWindow::save()
{
    if(curFile.isEmpty())
        return saveAs();
    else
        return saveFile(curFile);
}
bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"),
                                                    ".", tr("Spreadsheet files (*.sp)"));
    if(fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
void MainWindow::closeEvent(QCloseEvent *event){
    if(okToContinue()){
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}

bool MainWindow::okToContinue()
{
    if(isWindowModified()){
        int r = QMessageBox::warning(this, tr("MyFormer"),
                                        tr("The document has been modified.\n"
                                           "Do you want to save your changes?"),
                                        QMessageBox::Yes | QMessageBox::No
                                     | QMessageBox::Cancel);
        if(r == QMessageBox::Yes){
            return save();
        }
        else if(r == QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}
bool MainWindow::loadFile(const QString &fileName)
{
    if(!spreadsheet->readFile(fileName)){
        statusBar()->showMessage(tr("Loading Canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File Loaded"), 2000);
    return true;
}
bool MainWindow::saveFile(const QString &fileName)
{
    if(!spreadsheet->writeFile(fileName)){
        statusBar()->showMessage(tr("Saving Canceled"), 2000);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File Saved"), 2000);
    return true;
}
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);
    QString showName = tr("Untitled");
    if(!curFile.isEmpty()){
        showName = strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();
    }
    setWindowTitle(tr("%1[*] - %2").arg(showName)
                   .arg(tr("Spreadsheet")));
}
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
void MainWindow::updateRecentFileActions(){
    QMutableStringListIterator i(recentFiles);
    while(i.hasNext()){
        if(!QFile::exists(i.next()))
            i.remove();
    }
    for(int j = 0; j < MaxRecentFiles; ++j){
        if(j < recentFiles.count()){
            QString text = tr("&%1 %2").arg(j+1)
                    .arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
        }
    }
}
void MainWindow::openRecentFile()
{
    if(okToContinue()){
        QAction *action = qobject_cast<QAction *>(sender());
        if(action)
            loadFile(action->data().toString());
    }
}

// Implementation of Edit Menu(find, go to cell)
void MainWindow::find()
{
    if(!findDialog){
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)),
                spreadsheet, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)),
                spreadsheet, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
    }

    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}
void MainWindow::goToCell()
{
    GoToCellDialog *dialog = new GoToCellDialog(this);
    if(dialog->exec()){
        QString str = dialog->lineEdit->text().toUpper();
        spreadsheet->setCurrentCell(str.mid(1).toInt() - 1,
                                    str[0].unicode() - 'A');
    }
    delete dialog;
}

// Implementation of Tools Menu(sort)
void MainWindow::sort()
{
    SortDialog *dialog = new SortDialog(this);
    QTableWidgetSelectionRange range = spreadsheet->selectedRange();
    dialog->setColumnRange('A' + range.leftColumn(),
                           'A' + range.rightColumn());
    if(dialog->exec()){
        SpreadsheetCompare compare;
        compare.keys[0] =
                dialog->primaryColumnCombo->currentIndex();
        compare.keys[1] =
                dialog->secondaryColumnCombo->currentIndex() - 1;
        compare.keys[2] =
                dialog->tertiaryColumnCombo->currentIndex() - 1;
        compare.ascending[0] =
                (dialog->primaryOrderCombo->currentIndex() == 0);
        compare.ascending[1] =
                (dialog->secondaryOrderCombo->currentIndex() == 0);
        compare.ascending[2] =
                (dialog->tertiaryOrderCombo->currentIndex() == 0);
        spreadsheet->sort(compare);
    }
    delete dialog;
}

// Implementation of About
void MainWindow::about()
{
    QMessageBox::about(this, tr("About MyFormer"),
                       tr("<h2>MyFormer 1.0</h2>"
                          "<p>Copyright &copy; 2015 Nightly Sky</p>"
                          "<p>MyFormer is a small spreadsheet application "
                          "that demostrates QAction, QMainWindow, QDialog, "
                          "QTableWidget and many other Qt classes.</p>"));
}

// Settings
void MainWindow::writeSettings()
{
    QSettings settings("NightlySky", "MyFormer");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalculateAction->isChecked());
}
void MainWindow::readSettings()
{
    QSettings settings("NightlySky", "MyFormer");
    restoreGeometry(settings.value("geometry").toByteArray());

    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("autoRecalc", true).toBool();
    autoRecalculateAction->setChecked(autoRecalc);
}
