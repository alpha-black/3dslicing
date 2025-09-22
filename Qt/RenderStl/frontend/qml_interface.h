#ifndef QML_INTERFACE_H
#define QML_INTERFACE_H

#include "middleware/shared_data_handler.h"
#include <QObject>

class QMLInterfaceHandler : public QObject
{
    Q_OBJECT
public:
    QMLInterfaceHandler();
    void setDataHandler(SharedDataHandler *handler) {
        dataHandler = handler;
    }

public slots:
    void fileOpenedSignalHandler(QString fileName);
    void generateGCodeSignalHandler(QString outFileName, int printer_id);
    void generateSupportSigHandler();
    void setSupportAndQualitySigH(int support_, int quality_);
private:
    SharedDataHandler *dataHandler;
};

#endif // QML_INTERFACE_H
