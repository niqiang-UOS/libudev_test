#ifndef UDEVMONITOR_H
#define UDEVMONITOR_H

#include <libudev.h>

#include <QObject>
#include <QThread>


class UDevMonitor : public QThread
{
    Q_OBJECT
public:
    explicit UDevMonitor(QObject *parent = nullptr);

signals:

protected:
    virtual void run() override;
    // 监控设备事件
    void monitor_devices(struct udev* udev);
    void process_device(struct udev_device* dev);


private:
    struct udev* udev;
    QString m_sSerialID, m_sSyspath, m_sDevType;
    bool m_bGetDevInfo;
};

#endif // UDEVMONITOR_H
