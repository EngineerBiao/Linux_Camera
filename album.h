#ifndef ALBUM_H
#define ALBUM_H

#include <iostream>

#include <QWidget>
#include <QPushButton>
#include <QDir>
#include <QString>
#include <QFont>

extern QString currentDir; // 当前目录路径，在v4l2.cpp那里定义了
using namespace std;

namespace Ui {
class Album;
}

class Album : public QWidget
{
    Q_OBJECT

public:
    explicit Album(QWidget *parent = nullptr);
    ~Album();
    QPushButton *return_ui();
    void update_fileList();
    void show_picture();

private:
    Ui::Album *ui;
    QDir dir; // 目录对象
    QStringList fileList; // 存储文件列表
    int index = 0; // 显示的图片索引
    void without_photo();

private slots:
    void next_photo();
    void previous_photo();
    void delete_photo();
};

#endif // ALBUM_H
