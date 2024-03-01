#include "v4l2.h"

using namespace std;
int video_fd;
// 将帧缓冲映射到进程地址空间，使用数组进行存储
void *userbuf[BUFNUMS]; // 保存每一帧映射后的用户缓冲区起始地址
unsigned int userbuf_size[BUFNUMS]; // 存放每一帧的缓冲区大小，用于后面的释放
QString currentDir = QDir::currentPath(); // 获取当前目录

V4l2::V4l2(Ui::Widget * ui) : v_ui(ui)
{
    v4l2_init();
    make_album();
}
/* 错误处理函数 */
inline void fun_error(const char *str)
{
    perror(str);
    close(video_fd);
}

/* 拍照 */
void V4l2::take_photo()
{
    // 获取当前日期和时间，用来作为文件名
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = currentDir + "album/" + currentDateTime.toString("MM-dd_HH-mm-ss") + ".jpg";
    QFile file(fileName);

    // 从内核的输出队列中取出一个
    struct v4l2_buffer buffer;
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(video_fd, VIDIOC_DQBUF, &buffer) == 0)
    {
        int fd = open(fileName.toUtf8().constData(), O_RDWR | O_CREAT, 0777); // 打开并创建一个新文件
        write(fd, userbuf[buffer.index], buffer.bytesused);
        cout << fileName.toStdString() << "创建成功" << endl;
        close(fd);
    }
    // 将用完的缓冲区放回输入队列 (VIDIOC_QBUF)
    if (ioctl(video_fd, VIDIOC_QBUF, &buffer) < 0)
    {
        fun_error("返回队列失败！");
        exit(1);
    }
    // 清空label,相当于拍照效果提示
    // v_ui->label->clear();
}

/* 获取一帧图像存储为QPixmap，然后返回给Widget类，在Widget那里绘制 */
QPixmap V4l2::get_picture()
{
    QPixmap pix;
    struct v4l2_buffer buffer; // 用于获取内核队列的缓冲区图像
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    // 从内核的输出队列中取出一个，如果输出队列没有缓冲区就会阻塞等待
    if(ioctl(video_fd, VIDIOC_DQBUF, &buffer) == 0)
    {
        // 将二进制图像数据加载进pix对象
        pix.loadFromData((unsigned char *)userbuf[buffer.index], buffer.bytesused);
        // 对图像进行缩放，缩放成640*480,第三个参数是保持纵横比
        pix = pix.scaled(640, 480, Qt::KeepAspectRatio);
    }
    // 将用完的缓冲区放回输入队列 (VIDIOC_QBUF)
    if(ioctl(video_fd, VIDIOC_QBUF, &buffer) < 0)
    {
        fun_error("放回队列失败");
        exit(1); // 结束进程
    }
    return pix; // 返回pix
}

/* 初始化摄像头 */
void V4l2::v4l2_init()
{
    /* 1、打开设备 */
    if ((video_fd = open(DEVNAME, O_RDWR)) < 0)
        fun_error("video设备打开失败\n");

    /* 2、查询摄像头的能力 (VIDIOC_QUERYCAP：是否支持视频采集、内存映射等) */
    struct v4l2_capability vcap; // 通过该结构体来查询设备能力信息
    // 查询设备的能力信息并将结果存在v4l2_capability结构体中
    if (ioctl(video_fd, VIDIOC_QUERYCAP, &vcap) < 0)
        fun_error("查询设备信息失败\n");
    else
    {
        // 检查是否包含V4L2_CAP_VIDEO_CAPTURE标志，该标志表示设备支持视频捕获功能
        if ((V4L2_CAP_VIDEO_CAPTURE & vcap.capabilities) == 0)
            fun_error("设备不支持视频采集\n");
        if ((V4L2_MEMORY_MMAP & vcap.capabilities) == 0)
            fun_error("设备不支持mmap内存映射\n");
    }
    /* 3、查看摄像头所支持的像素格式 */
    struct v4l2_fmtdesc fmt;
    fmt.index = 0; // 编号从0开始设置
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 类型是camera设备

    cout << "摄像头支持格式如下:" << endl;
    while (ioctl(video_fd, VIDIOC_ENUM_FMT, &fmt) == 0)
    {
        cout << "v4l2_format" << fmt.index << ": " << fmt.description << endl;
        fmt.index++;
    }

    /* 4、设置采集出来的图像的帧格式 */
    struct v4l2_format vfmt;
    vfmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 帧类型
    vfmt.fmt.pix.width = 640; // 按LCD的像素来设置
    vfmt.fmt.pix.height = 480;
    vfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; // 设置输出类型：MJPG
    if (ioctl(video_fd, VIDIOC_S_FMT, &vfmt) < 0)
        fun_error("设置格式失败\n");
    // 检查设置参数是否生效
    if (ioctl(video_fd, VIDIOC_G_FMT, &vfmt) < 0) // VIDIOC_G_FMT宏表示获取设备的格式信息
        fun_error("获取设置格式失败\n");
    cout << "实际分辨率：" << vfmt.fmt.pix.width << "*" << vfmt.fmt.pix.height << "图像格式：" << vfmt.fmt.pix.pixelformat << endl;
    if (vfmt.fmt.pix.width == 640 && vfmt.fmt.pix.height == 480 && vfmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
        cout << "实际与设置的相同" << endl;
    else
        cout << "实际与设置的不相同" << endl;

    /* 5、申请缓冲区并映射到用户空间 */

    // 申请缓冲区
    struct v4l2_requestbuffers reqbuf;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; // 帧类型
    reqbuf.count = 5; // 5个帧缓冲
    reqbuf.memory = V4L2_MEMORY_MMAP;
    if (ioctl(video_fd, VIDIOC_REQBUFS, &reqbuf) < 0)
        fun_error("申请缓冲区失败\n");

    // 将帧逐个映射并加入到内核队列
    for (unsigned int i = 0; i < reqbuf.count; i++)
    {
        struct v4l2_buffer buf; // 可用于获取申请的缓冲区的信息，例如内存地址和大小
        buf.index = i;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (ioctl(video_fd, VIDIOC_QUERYBUF, &buf) < 0) // 获取申请的缓冲区的信息
            fun_error("获取缓冲区信息失败");
        // 映射
        userbuf[i] = mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, video_fd, buf.m.offset);
        userbuf_size[i] = buf.length;
        if (userbuf[i] == MAP_FAILED)
            fun_error("mmap failed\n");
        // 放入队列
        if (ioctl(video_fd, VIDIOC_QBUF, &buf) < 0)
            fun_error("入队失败\n");
    }

    /* 6、启动视频采集 */
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(video_fd, VIDIOC_STREAMON, &type) < 0)
        fun_error("开始采集失败\n");
}

/* 创建相册 */
void V4l2::make_album()
{
    QDir photoDir(currentDir + "album/");
    if (!photoDir.exists())
    {
        if (photoDir.mkdir(currentDir + "album/"))
            cout << "创建相册成功" << endl;
        else
            cout << "创建相册失败" << endl;
    }
    cout << "相册已存在" << endl;
}
