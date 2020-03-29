#include "userinfodialog.h"
#include "ui_userinfodialog.h"

userInfoDialog::userInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfoDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("个人设置");
    //    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
    ui->changePassWidget->hide();
    ui->changeHeadWidget->hide();
    QPalette p(ui->setUpItem->palette());//设置背景色
    p.setColor(QPalette::Base, QColor("#f0f0f0"));
    ui->setUpItem->setPalette(p);
    ui->setUpItem->setFocusPolicy(Qt::NoFocus);
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
            ui->changeHeadWidget->hide();
        } else if(item->data(0, Qt::UserRole) == "changeHead"){
            ui->changeHeadWidget->show();
            ui->changePassWidget->hide();
            ui->labelPreHead->hide();
            ui->pushButtonHeadConfirm->hide();
        }
    });
    connect(&mainlink, &Cvlink::changePasswordResult, this, &userInfoDialog::changePassResult);
    connect(&mainlink, &Cvlink::changeHeadResult, this, &userInfoDialog::changeHeadResult);
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
    ui->labelPreHead->show();
    ui->pushButtonHeadConfirm->show();
}

void userInfoDialog::on_pushButtonHeadConfirm_clicked()
{
    QString path = ui->lineEditPath->text();


    QPixmap pixmap(path);
    pixmap = pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QString tempPath = QApplication::applicationDirPath() + "/images/head.jpg";
    QFile files(tempPath);
    files.open(QIODevice::WriteOnly);
    pixmap.save(&files, "JPG");

    QString imageMD5 = Global::imageToBase64(tempPath);
    QFile file("C:/Users/Joker/Desktop/temp.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        file.write(imageMD5.toUtf8());
    }

    Global::base64ToImage(imageMD5, QApplication::applicationDirPath() + "/images/head.jpg");
    QMap<QString, QString> map;
    map.insert("avatar", imageMD5);
    mainlink.postHttpRequest(Global::bookUrl + "/user/updateDetail", map);
}

void userInfoDialog::changePassResult(bool result, QString msg)
{
    if(result){
        QString close = "false";
        Global::IMConfig->setValue("login/start", close);
        close = "";
        Global::IMConfig->setValue("login/password", close);
        QMessageBox::warning(this, "警告", "密码设置成功，请重启应用！", "重 启");

        qApp->exit(RETCODE_RESTART);
    } else {
        QMessageBox::warning(this, "错误", msg, "OK");
    }
}

void userInfoDialog::changeHeadResult(bool result)
{
    if(result){
        QMessageBox::warning(this, "系统", "头像设置成功！", "OK");
        ui->labelPreHead->hide();
        ui->pushButtonHeadConfirm->hide();
        ui->lineEditPath->clear();
        emit changeHead();
    }
}
