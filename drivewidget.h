/*
 * drivewidget.h
 *
 * Copyright 2017 blind
 *
 */

#ifndef DRIVEWIDGET_H
#define DRIVEWIDGET_H

#include <QFrame>

namespace Ui {
class DriveWidget;
}

class SimpleDiskImage;

class DriveWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DriveWidget(int driveNum, QWidget *parent = 0);
    ~DriveWidget();

    int getDriveNumber() { return driveNo_; }
    void setup(bool happyShown, bool chipShown, bool nextSideShown, bool OSBShown, bool toolDiskShown);


    void showAsEmpty(bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden);
    void showAsFolderMounted(const QString &fileName, const QString &description, bool editEnabled);
    void showAsImageMounted(const QString &fileName, const QString &description, bool editEnabled, bool enableSave, bool leverOpen, bool happyEnabled, bool chipOpen,
                            bool translatorActive, bool toolDiskActive, bool severalSides, bool happyShown, bool chipShown, bool nextSideShown, bool OSBShown, bool toolDiskShown);

    void updateFromImage(SimpleDiskImage* diskImage, bool happyShown, bool chipShown, bool nextSideShown, bool OSBShown, bool toolDiskShown);
    bool isAutoSaveEnabled();
    bool isHappyEnabled();
    bool isChipEnabled();
    void setLabelToolTips(const QString &one, const QString &two, const QString &three);

    void triggerAutoSaveClickIfEnabled();
    void triggerHappyClickIfEnabled();
    void triggerChipClickIfEnabled();

signals:
    void actionMountDisk(int deviceId);
    void actionMountFolder(int deviceId);
    void actionEject(int deviceId);
    void actionNextSide(int deviceId);
    void actionToggleHappy(int deviceId, bool enabled);
    void actionToggleChip(int deviceId, bool open);
    void actionToggleOSB(int deviceId, bool open);
    void actionToolDisk(int deviceId, bool open);
    void actionWriteProtect(int deviceId,bool state);
    void actionMountRecent(int deviceId, const QString &fileName);
    void actionEditDisk(int deviceId);
    void actionSave(int deviceId);
    void actionAutoSave(int deviceId,bool enabled);
    void actionSaveAs(int deviceId);
    void actionRevert(int deviceId);
    void actionBootOptions(int deviceId);

public slots:
    void setFont(const QFont& font);

private slots:
    void on_actionMountFolder_triggered();
    void on_actionMountDisk_triggered();

    void on_actionEject_triggered();
    void on_actionNextSide_triggered();
    void on_actionToggleHappy_triggered(bool enabled);
    void on_actionToggleChip_triggered(bool open);
    void on_actionToggleOSB_triggered(bool open);
    void on_actionToolDisk_triggered(bool open);
    void on_actionWriteProtect_toggled(bool state);
    void on_actionEditDisk_triggered();
    void on_actionSave_triggered();
    void on_actionRevert_triggered();
    void on_actionSaveAs_triggered();
    void on_actionAutoSave_toggled(bool arg1);
    void on_actionBootOption_triggered();

private:
    Ui::DriveWidget *ui;
    int driveNo_;
};

#endif // DRIVEWIDGET_H
