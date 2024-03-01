#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QDateTime>

#include "v4l2.h"
#include "album.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    V4l2 *video1;
    QTimer *timer;
    Album *album;
    QPixmap videoFrame; // 存储一帧图像
    qint64 lastTime = 0;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void video_show();
    void open_album();
    void close_album();
};
#endif // WIDGET_H
