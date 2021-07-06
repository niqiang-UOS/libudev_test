#ifndef UDEVENVIRONMENT_H
#define UDEVENVIRONMENT_H

#include <libudev.h>

#include <QObject>

class UDevEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit UDevEnvironment(QObject *parent = nullptr);

    // 获取当前所有设备
    void getAllDevice();
    // 获取当前所有事件
    void getAllQueue();
    // 获取所有硬件数据库信息
    void getAllHwdb();
    // 禁用设备
    void DisableDev(QString strDevPath);
    // 启用设备
    void EnableDev(QString strDevPath);
    // 移除设备
    void RemoveDev(QString strDevPath);

private:
    void getAllDevType(udev_enumerate* enumerate);
    void process_device(struct udev_device* dev);
signals:

};

#endif // UDEVENVIRONMENT_H
