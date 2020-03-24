/*
 * mainwindow.h
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTranslator>
#include <QSystemTrayIcon>
#include <QTextEdit>

#include "optionsdialog.h"
#include "aboutdialog.h"
#include "createimagedialog.h"
#include "diskeditdialog.h"
#include "serialport.h"
#include "sioworker.h"
#include "docdisplaywindow.h"
#include "network.h"
#include "drivewidget.h"
#include "infowidget.h"
#include "printerwidget.h"
#include "Emulator.h"
#include "printers/textprinterwindow.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString g_sessionFile;
    QString g_sessionFilePath;
    QString g_mainWindowTitle;
    static MainWindow *getInstance() { return instance; }

public slots:
    void show();
    int firstEmptyDiskSlot(int startFrom = 0, bool createOne = true);       //
    void mountFileWithDefaultProtection(int no, const QString &fileName);   //
    void autoCommit(int no, bool st);       //
    void happy(int no, bool st);       //
    void chip(int no, bool st);       //
    void bootExeTriggered(const QString &fileName);
    void closeTextPrinterWindow(const Printers::TextPrinterWindow *window);

private:
    int untitledName;
    Ui::MainWindow *ui;
    SioWorkerPtr sio;
    bool shownFirstTime;
    PrinterWidget* printerWidgets[PRINTER_COUNT]; //
    DriveWidget* diskWidgets[DISK_COUNT];    //
    // InfoWidget* infoWidget;

    QLabel *speedLabel, *onOffLabel, *prtOnOffLabel, *netLabel, *clearMessagesLabel;  //
#ifndef Q_NO_DEBUG
    QLabel *snapshot;
#endif
    DocDisplayWindow *docDisplayWindow;    //
    QTranslator respeqt_translator, respeqt_qt_translator;
    QSystemTrayIcon trayIcon;
    Qt::WindowFlags oldWindowFlags;
    Qt::WindowStates oldWindowStates;
    QString lastMessage;
    int lastMessageRepeat;
    static MainWindow *instance;
    
    bool isClosing;

    QDialog *logWindow_;

    RomProvider *m_romProvider;

    QList<QAction*> recentFilesActions_;

    void setSession();  //
    void updateRecentFileActions();
    int containingDiskSlot(const QPoint &point);
    void bootExe(const QString &fileName);
    void mountFile(int no, const QString &fileName, bool prot);
    void mountDiskImage(int no);
    void mountFolderImage(int no);
    bool ejectImage(int no, bool ask = true);
    void loadNextSide(int no);
    void toggleHappy(int no, bool enabled);
    void toggleChip(int no, bool open);
    void toggleOSB(int no, bool open);
    void toggleToolDisk(int no, bool open);
    void toggleWriteProtection(int no, bool protectionEnabled);
    void openEditor(int no);
    void saveDisk(int no);
    void saveDiskAs(int no);
    void revertDisk(int no);
    QMessageBox::StandardButton saveImageWhenClosing(int no, QMessageBox::StandardButton previousAnswer, int number);
    void loadTranslators();
    void autoSaveDisk(int no);                                              //
    void setUpPrinterEmulationWidgets(bool enabled);

    bool firmwareAvailable(int no, QString &name, QString path);

    void createDeviceWidgets();
    SimpleDiskImage *installDiskImage(int no);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void enterEvent(QEvent *) override;
    void leaveEvent(QEvent *) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void logMessage(int type, const QString &msg);
    void newSlot (int slot);
    void fileMounted(bool mounted);
    void sendLogText (QString logText);
    void sendLogTextChange (QString logTextChange);
    void setFont(const QFont &font);

public:
    void doLogMessage(int type, const QString &msg);

private slots:
    void openRecent(); //

    void on_actionPlaybackCassette_triggered();
    void on_actionBootExe_triggered();
    void on_actionSaveSession_triggered();
    void on_actionOpenSession_triggered();
    void on_actionNewImage_triggered();
    void on_actionEjectAll_triggered();
    void on_actionOptions_triggered();
    void on_actionStartEmulation_triggered();
    void on_actionPrinterEmulation_triggered();
    void on_actionHideShowDrives_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionDocumentation_triggered();

    // Device widget events
    void mountDiskTriggered(int deviceId); //
    void mountFolderTriggered(int deviceId); //
    void ejectTriggered(int deviceId); //
    void nextSideTriggered(int deviceId); //
    void happyToggled(int deviceId, bool enabled); //
    void chipToggled(int deviceId, bool open); //
    void OSBToggled(int deviceId, bool open); //
    void toolDiskTriggered(int deviceId, bool open); //
    void protectTriggered(int deviceId, bool writeProtectEnabled); //
    void mountRecentTriggered(const QString &fileName);
    void editDiskTriggered(int deviceId); //
    void saveTriggered(int deviceId); //
    void autoSaveTriggered(int deviceId); //
    void saveAsTriggered(int deviceId); // MIA
    void revertTriggered(int deviceId); // MIA


    void bootOptionTriggered(); //
    void on_actionToggleMiniMode_triggered();
    void on_actionToggleShade_triggered();
    void on_actionLogWindow_triggered();

    void showHideDrives();
    void sioFinished(); //
    void sioStarted(); //
    void sioStatusChanged(QString status); //
    void textPrinterWindowClosed();
    void docDisplayWindowClosed(); //
    void deviceStatusChanged(int deviceNo); //
    void uiMessage(int t, const QString message);
    // TODO Check on Windows and Linux
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason); //
    void keepBootExeOpen();
    void saveWindowGeometry();
    void saveMiniWindowGeometry();
    void logChanged(QString text);
    void changeFonts();
};

#endif // MAINWINDOW_H
