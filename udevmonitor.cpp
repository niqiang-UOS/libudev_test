#include "udevmonitor.h"
#include <QDebug>

UDevMonitor::UDevMonitor(QObject *parent) : QThread(parent)
{
}

void UDevMonitor::run()
{
    udev = udev_new();
    if (!udev) {
        fprintf(stderr, "udev_new() failed\n");
        return;
    }
    monitor_devices(udev);
    udev_unref(udev);
}

void UDevMonitor::monitor_devices(struct udev* udev)
{
    /*
     * 创建一个监视器，事件源可以是udev、kernel
     * 基于"kernel"的事件通知要早于"udev",但相关的设备结点未必创建完成
     * 所以一般应用的设计要基于"udev"进行监控
     * */
    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");

    /*
     * 增加一个基于设备类型的udev事件过滤器
     * 可以是/sys/class目录下的任意类型
     * */
    udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", nullptr);
    udev_monitor_filter_add_match_subsystem_devtype(mon, "block", nullptr);
    udev_monitor_filter_add_match_subsystem_devtype(mon, "input", nullptr);
    udev_monitor_filter_add_match_subsystem_devtype(mon, "sound", nullptr);
    udev_monitor_filter_add_match_subsystem_devtype(mon, "video4linux", nullptr);

    //启动监控过程
    udev_monitor_enable_receiving(mon);

    //获取一个文件描述符，基于返回的fd可以执行poll操作，简化程序设计
    int fd = udev_monitor_get_fd(mon);

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    while(select(fd+1,&fds,nullptr,nullptr,nullptr)>0){
        if (FD_ISSET(fd, &fds)) {
            //获取产生事件的设备映射
            struct udev_device* dev = udev_monitor_receive_device(mon);
            process_device(dev);
        }
    }
}

void UDevMonitor::process_device(struct udev_device* dev)
{
    if (dev) {
        qDebug() << "\n******************Print device Begin******************";
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
        qDebug() << "\n------------------Print device End------------------";
        udev_device_unref(dev);
    }

}
