#ifndef V4L2_H
#define V4L2_H

#include <QObject>
#include <QDir>
#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QDateTime>
#include <QTimer>
#include <QFile>

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <linux/fb.h>


#include "ui_widget.h"

#define DEVNAME "/dev/video2" // 设备节点，要根据具体情况修改
#define BUFNUMS 5 // 申请的缓冲区数量

class V4l2 : public QObject
{
    Q_OBJECT
public:
    explicit V4l2(Ui::Widget *);
    Ui::Widget *v_ui; // 在构造函数中会接收Widget的ui，通过v_ui来操作主窗口

private:
    void v4l2_init();
    void make_album();
public:
    QPixmap get_picture();

public slots:
    void take_photo();

signals:

};

#endif // V4L2_H
