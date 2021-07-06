#include "udevenvironment.h"


#include<QDir>
#include<QDebug>

UDevEnvironment::UDevEnvironment(QObject *parent) : QObject(parent)
{

}

void UDevEnvironment::getAllDevType(udev_enumerate *enumerate)
{
    //可以监控  /sys/class下的所有类型
    QDir dir("/sys/class");
    if(!dir.exists())
        return ;

    //QStringList filters;
    //filters<<QString("*.jpeg")<<QString("*.txt")<<QString("*.lnk")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");
    //dir.setNameFilters(filters);  //设置文件名称过滤器，只为filters格式（后缀为.jpeg等图片格式）
    //dir.setSorting(QDir::Size | QDir::Reversed);//实现对文件输出的排序
    dir.setFilter(QDir::Dirs | QDir::NoSymLinks);//实现对文件的过滤

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        QString sType = fileInfo.baseName();

        char*  ch;
        QByteArray ba = sType.toLatin1();
        ch = ba.data();
        udev_enumerate_add_match_subsystem(enumerate, ch);
    }

}
// 获取当前所有设备
void UDevEnvironment::getAllDevice()
{
    struct udev* udev = udev_new();
    if (!udev) {
        qDebug() << "udev_new() failed\n";
        return;
    }

    struct udev_enumerate* enumerate = udev_enumerate_new(udev);
    getAllDevType(enumerate);

    //扫描/sys目录下所有与过滤器匹配的设备。扫描完成后的数据结构是一个链表
    udev_enumerate_scan_devices(enumerate);

    //获取链表的首个结点
    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    //遍历整个链表
    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* dev = udev_device_new_from_syspath(udev, path);
        process_device(dev);
    }

    udev_enumerate_unref(enumerate);
    udev_unref(udev);
}

// 获取当前所有事件
void UDevEnvironment::getAllQueue()
{
    struct udev* udev = udev_new();
    if (!udev) {
        qDebug() << "udev_new() failed\n";
        return;
    }

    struct udev_queue *dev_queue = udev_queue_new(udev);
    struct udev_list_entry *queueList =udev_queue_get_queued_list_entry(dev_queue);
    struct udev_list_entry* entry;
    udev_list_entry_foreach(entry, queueList) {
        const char* key = udev_list_entry_get_name(entry);
        const char* value = udev_list_entry_get_value(entry);
        qDebug() << "queue_Key:" << key << " queue_Value:" << value;
    }
}

// 获取所有硬件数据库信息
void UDevEnvironment::getAllHwdb()
{
    struct udev* udev = udev_new();
    if (!udev) {
        qDebug() << "udev_new() failed\n";
        return;
    }

    struct udev_hwdb *dev_db = udev_hwdb_new(udev);

}

void UDevEnvironment::process_device(struct udev_device* dev)
{
    if (dev)
    {
        qDebug() << "\n******************Environment device Begin******************";
        qDebug() << "syspath : " << udev_device_get_syspath(dev);
        struct udev_list_entry *dev_link, *dev_links;
        dev_links = udev_device_get_devlinks_list_entry(dev);
        udev_list_entry_foreach(dev_link, dev_links){
            qDebug() << udev_list_entry_get_name(dev_link) << " : " << udev_list_entry_get_value(dev_link);
        }

        struct udev_list_entry *dev_propertie, *dev_properties_entry;
        dev_properties_entry = udev_device_get_properties_list_entry(dev);
        udev_list_entry_foreach(dev_propertie, dev_properties_entry){
            qDebug() << udev_list_entry_get_name(dev_propertie) << " : " << udev_list_entry_get_value(dev_propertie);
        }


        struct udev_list_entry *dev_tag, *dev_tags;
        dev_tags = udev_device_get_tags_list_entry(dev);
        udev_list_entry_foreach(dev_tag, dev_tags){
            qDebug() << udev_list_entry_get_name(dev_tag) << " : " << udev_list_entry_get_value(dev_tag);
        }


        struct udev_list_entry *dev_sysattr, *dev_sysattrs;
        dev_sysattrs = udev_device_get_sysattr_list_entry(dev);
        udev_list_entry_foreach(dev_sysattr, dev_sysattrs){
            qDebug() << udev_list_entry_get_name(dev_sysattr) << " : " << udev_list_entry_get_value(dev_sysattr);
        }
        qDebug() << "\n------------------Environment device End------------------";
        udev_device_unref(dev);
    }

}
