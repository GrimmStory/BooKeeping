#include "userinfodialog.h"
#include "ui_userinfodialog.h"

userInfoDialog::userInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfoDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
//    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
    ui->changePassWidget->hide();
    ui->changeHeadWidget->hide();
    QPalette p(ui->setUpItem->palette());//设置背景色
    p.setColor(QPalette::Base, QColor("#f0f0f0"));
    ui->setUpItem->setPalette(p);
    QTreeWidgetItem *changePassItem = new QTreeWidgetItem(ui->setUpItem);
    changePassItem->setText(0, "密码修改");
    changePassItem->setData(0,Qt::UserRole,"changePass");
    changePassItem->setTextAlignment(0, Qt::AlignRight | Qt::AlignCenter);
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->setUpItem);
    item->setText(0, "头像修改");
    item->setData(0,Qt::UserRole,"changeHead");
    item->setTextAlignment(0, Qt::AlignRight | Qt::AlignCenter);

    connect(ui->setUpItem, &QTreeWidget::itemClicked, this, [=](QTreeWidgetItem * item){
        if(item->data(0, Qt::UserRole) == "changePass"){
            ui->changePassWidget->show();
        } else if(item->data(0, Qt::UserRole) == "changeHead"){
            ui->changeHeadWidget->show();
        }
    });
}

userInfoDialog::~userInfoDialog()
{
    delete ui;
}

void userInfoDialog::on_pushButtonConfirm_clicked()
{
    QString oldPassword = ui->oldPassLine->text().trimmed();
    QString newPassWord = ui->newPassLine->text().trimmed();
    QString confirmPassword = ui->confirmPassLine->text().trimmed();

    if(oldPassword == "" || newPassWord == "" || confirmPassword == ""){
        QMessageBox::warning(this, "错误", "内容不能为空！", "OK");
        return;
    }
    if(newPassWord != confirmPassword){
        QMessageBox::warning(this, "错误", "两次输入的密码不正确！", "OK");
        return;
    }

    QMap<QString, QString> map;
    map.insert("oldPassword", oldPassword);
    map.insert("newPassword", newPassWord);
    mainlink.postHttpRequest(Global::bookUrl + "/user/changePassword", map);
}


void userInfoDialog::on_pushButtonSelect_clicked()
{
    QString path = QFileDialog::getOpenFileName(NULL, "选择图片", "/", "Image Files(*.jpg *.png)", NULL, NULL);
    if(path != ""){
        ui->lineEditPath->setText(path);
    } else {
        QMessageBox::warning(this, "错误", "非法路径，请重新选择！", "OK");
        return;
    }

    QPixmap *pixmap = new QPixmap(path);
    pixmap->scaled(ui->labelHead->size(), Qt::KeepAspectRatio);
    ui->labelPreHead->setScaledContents(true);
    ui->labelPreHead->setPixmap(*pixmap);
}

void userInfoDialog::on_pushButtonHeadConfirm_clicked()
{
    QString path = ui->lineEditPath->text();
    QString imageMD5 = Global::imageToBase64(path);
    QFile file("C:/Users/Joker/Desktop/temp.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        file.write(imageMD5.toUtf8());
    }

    Global::base64ToImage(imageMD5, QApplication::applicationDirPath() + "/images/head.jpg");
    QMap<QString, QString> map;
    map.insert("avatar", imageMD5);
    mainlink.postHttpRequest(Global::bookUrl + "/user/updateDetail", map);
}
