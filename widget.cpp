#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    video1 = new V4l2(ui); // 把ui传入v4l2的构造函数

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Widget::video_show); // 连接定时器和图像显示
    timer->start(40); // 40毫秒触发一次，就是25帧（我的摄像头帧率就是25帧）
    // 连接拍照信号槽
    connect(ui->take_photo, &QPushButton::clicked, video1, &V4l2::take_photo);

    album = new Album();
    // 连接打开和关闭相册的信号槽
    connect(ui->album, &QPushButton::clicked, this, &Widget::open_album);
    connect(album->return_ui(), &QPushButton::clicked, this, &Widget::close_album);
}

Widget::~Widget()
{
    delete ui;
}

// 重写paintEvent
void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this); // 指定画图的对象
    // 使用painter绘图
    QRectF targetRect(20, 0, 640, 480); // 从窗口坐标(20,0)开始绘制，绘制大小是640*480
    QRectF sourceRect(0, 0, videoFrame.width(), videoFrame.height()); // 从QImage的原点截取，大小和QPixmap一样
    painter.drawImage(targetRect, videoFrame.toImage(), sourceRect); // 将QPixmap转换为QImage传入函数
}

/* 展示一帧图片 */
void Widget::video_show()
{
    // 获取当前时间戳
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 interval = currentTime - lastTime;
    cout << "时间差是" << interval << "ms" << endl;
    // 获取图像帧（QPixmap对象）
    videoFrame = video1->get_picture();
    update(); // 更新窗口（会调用paintEvent函数）
    // 更新时间差
    lastTime = currentTime;
}

/* 打开相册（跳转到相册界面） */
void Widget::open_album()
{
    album->update_fileList(); // 更新文件列表
    this->hide();
    timer->stop(); // 暂停定时器
    album->show_picture(); // 打开一个图片作为初始界面
    album->show(); // album是个界面类，直接调用show函数来显示
}
/* 关闭相册（返回主界面）*/
void Widget::close_album()
{
    album->hide();
    this->show();
    timer->start(40);
}
