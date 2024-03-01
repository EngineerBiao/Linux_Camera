#include "album.h"
#include "ui_album.h"

Album::Album(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Album)
{
    ui->setupUi(this);
    connect(ui->next, &QPushButton::clicked, this, &Album::next_photo);
    connect(ui->previous, &QPushButton::clicked, this, &Album::previous_photo);
    connect(ui->delete_2, &QPushButton::clicked, this, &Album::delete_photo);
}

Album::~Album()
{
    delete ui;
}

/* 更新相册图片文件列表 */
void Album::update_fileList()
{
    dir.setPath(currentDir + "album/"); // 设置目录路径
    // 使用过滤器，让entryList列出指定的文件
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot); // 只获取文件且不包括"."和".."
    fileList = dir.entryList(); // 获取文件列表
    cout << fileList.size() << endl;
}

/* 展示一张图片作为初始界面 */
void Album::show_picture()
{
    if (fileList.size() > 0)
    {
        QString filePath = dir.filePath(currentDir + "album/" + fileList[index]); // 获取文件路径
        QPixmap pix(filePath); // 创建图片对象
        ui->label->setPixmap(pix); // 在控件上显示图片
    }
    else
        without_photo();
}

/* 展示下一张图片 */
void Album::next_photo()
{
    if (fileList.size() > 0)
    {
        index++;
        if (index >= fileList.size())
            index = 0;
        QString filePath = dir.filePath(currentDir + "album/" + fileList[index]); // 获取文件路径
        QPixmap pix(filePath); // 创建图片对象
        ui->label->setPixmap(pix); // 在控件上显示图片
    }
    else
        without_photo();
}
/* 展示下一张照片 */
void Album::previous_photo()
{
    if (fileList.size() > 0)
    {
        index--;
        if (index < 0)
            index = fileList.size() - 1;
        QString filePath = dir.filePath(currentDir + "album/" + fileList[index]); // 获取文件路径
        QPixmap pix(filePath); // 创建图片对象
        ui->label->setPixmap(pix); // 在控件上显示图片
    }
    else
        without_photo();
}

/* 删除照片 */
void Album::delete_photo()
{
    if (fileList.size() > 0)
    {
        QFile file(currentDir + "album/" + fileList[index]);
        if (file.remove())
            cout << "文件删除成功" << endl;
        else
            cout << "文件删除失败" << endl;
        update_fileList(); // 更新图片文件列表
        previous_photo(); // 显示下一张图片
    }
    else
        without_photo();
}

/* 没有照片 */
void Album::without_photo()
{
    ui->label->setText("没有照片");
    QFont font("Arial", 24); // 使用Arial字体，大小为24
    ui->label->setAlignment(Qt::AlignCenter); // 让文字居中显示
    ui->label->setFont(font);
}

/* 返回界面的返回按钮指针 */
QPushButton *Album::return_ui()
{
    return ui->back;
}

