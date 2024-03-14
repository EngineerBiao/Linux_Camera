## 一、项目简介

运行在ARM-Linux开发板上的嵌入式相机项目，基于V4L2框架、C++和QT，实现了拍照和相册的功能。

### 硬件

1. 正点原子IMX6ULL开发板（用的出厂系统）（如果不是出厂系统，需要自己移植QT和触摸屏驱动，然后还需要手动配置内核开启v4l2驱动）
2. 正点原子4.3寸电容触摸屏（800*480）
3. 海康威视USB摄像头（1920*1080）（输出格式：MJPEG、YUYV、NV12）

### 快速开始

1、修改设备节点。进入v4l2.h文件，根据自己摄像头的设备节点来更改DEVNAME宏

![image-20240314215156989](https://biao-tu.oss-cn-shenzhen.aliyuncs.com/images/202403142151019.png)

2、交叉编译QT工程（具体方法看正点原子文档），通过U盘或网络协议将可执行文件传到开发板上，直接运行即可。



## 二、技术选型

* OpenGL + QT

后两种是用CPU进行渲染，这种是使用GPU进行渲染，效率和性能更高

* v4l2 + QT 和 OpenCV + QT

OpenCV比v4l2更加方便和简单，能进行图像处理和计算机视觉，但对于摄像头数据的获取没有v4l2全面。

使用v4l2或OpenCV获取到图像数据后，需要把图像存为QImage类型。然后接下来有两种方法：

1. 使用QLabel来显示QImage
2. 用QPainter直接将图像绘制到Widget上

第一种简单易用，但是不能处理图像而且QLabel显示频繁更新的图像的性能很差。第二种性能更强而且能处理图像。

对于第二种方法，可以使用QPainter+QImage或QPainter+QPixmap。建议使用QPainter+QPixmap。

**综上**：我选择了QPainter + QPixmap的方案



## 三、图像与视频基础

1. 视频由很多连续的图像构成，一帧就是一张图像，视频帧率（FPS）就是一秒内显示的帧数。

假设两帧之间的间隔时间是t，单位是毫秒

有帧率 = 1 / t * 1000。若t = 10ms，则帧率Frame Rate = 1 / 10 * 1000 = 100 (fps)

2. 分辨率：就是图像的像素点数量。由长乘宽来表示。

### 1. 颜色存储

RGB和YUV是描述和存储图像的颜色信息的两种方式，也叫做色彩空间

* RGB：通过红蓝绿3个颜色的不同强度来表示颜色，是无损的颜色表示方式，应用于电脑显示器、数码相机等
* YUV：用亮度和色度差来表示颜色，是有损的方式。在视频编码中，常常使用YUV格式，因为YUV可以减小文件大小

在视频编码和传输中，通常会用YUV格式，而在图形处理和显示领域，RGB格式更为常见。RGB和YUV之间的色彩空间转换是图像和视频处理中的一个常见任务。

### 2. 图像压缩格式

* MJPG和JPEG

JPEG：简写是JPG。适用于静态图像的压缩，是一种有损压缩格式

MJPG：Motion JPEG。对视频的每一帧压缩成JPEG，用于视频流的压缩

* BMP

一种图像文件格式，没有经过压缩

### 3. 视频编码

由于YUV和RGB形式的视频数据太大了，所以需要进行视频编码，有利于存储和传输。

视频编码：将视频像素数据（RGB、YUV等）压缩成视频码流。

常见的视频编码方式有YUY2、MJPG等，例如本项目使用的摄像头输出格式是YUY2，这就是一种YUV的编码格式。

视频编解码的大致流程

![image-20240130140707583](https://biao-tu.oss-cn-shenzhen.aliyuncs.com/images/202401301407617.png)



## 四、项目框架

### 摄像头显示数据到LCD的流程

![image-20240220221811135](https://biao-tu.oss-cn-shenzhen.aliyuncs.com/images/202403142124777.png)

说明：摄像头读取数据存到VideoBuf，然后转换成LCD的格式（RGB）存到ConerBuf。再缩放成LCD的分辨率存到ZoomBuf。然后后面的合并和发送都是自动进行的，最终图像就显示在LCD上。



### 工程结构

Linux_Camera/
|— src/
|	|— main.cpp     # 主程序入口
|	|— widget.cpp  # 主界面程序 
|	|— v4l2.cpp      # 摄像头控制程序
|	|— album.cpp  # 相册界面程序
|
|— inc/
|	|— widget.h，v4l2.h, album.h
|
|— froms/
|	|— widget.ui  # 主界面ui
|	|— album.ui   # 相册界面ui
|
|— Linux_Carmera.pro  # qmake配置文件
|— README



## 五、功能扩展

1. 实现视频流编码和解码功能，以支持远程视频传输和监控
2. 添加网络功能，支持远程控制、远程配置和固件升级等功能
3. 增加视频录制和播放功能



## 六、参考资料

1. [Linux相机小项目: 基于V4L2框架实现了一下驱动USB摄像头，实现拍照、相册功能 (gitee.com)](https://gitee.com/chenshao777/linux-camera-mini-project)
2. [Qt 性能优化：绘制视频方案选择 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/642345388)
