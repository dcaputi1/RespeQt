#ifndef TEXTPRINTER_H
#define TEXTPRINTER_H

#include "baseprinter.h"

class TextPrinter : public BasePrinter
{
    Q_OBJECT
public:
    TextPrinter(SioWorker *worker);

    virtual void handleCommand(quint8 command, quint16 aux);
private:
    int m_lastOperation;
    static bool conversionMsgdisplayedOnce;
signals:
    void print(const QString &text);
};

#endif // TEXTPRINTER_H