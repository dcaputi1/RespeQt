/*
 * rcl.h
 *
 * Copyright 2015, 2017 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef RCL_H
#define RCL_H

#include "sioworker.h"

// RespeQt Time Server //
class RCl: public SioDevice
{
    Q_OBJECT

public:
    QString g_fileFilter;
    RCl(SioWorkerPtr worker): SioDevice(worker), mutex() {}
    void handleCommand(quint8 command, const quint8 aux1, const quint8 aux2) override;
    QString toAtariFileName(QString dosFileName);
    QString toDosFileName(QString atariFileName);

public slots:
    void gotNewSlot (int slot);                         // 
    void fileMounted (bool mounted);                    // 

signals:
    void findNewSlot (int startFrom, bool createOne);
    void mountFile (int no, const QString fileName);
    void toggleAutoCommit (int no, bool st);
    void toggleHappy (int no, bool st);
    void toggleChip (int no, bool st);
    void bootExe (const QString fileName);

private:
    QString imageFileName;
    static char rclSlotNo;
    QMutex mutex;
};

#endif // RCL_H
