#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("登录");
    this->setWindowIcon(QIcon(":/images/images/icon.png"));
    this->installEventFilter(this);

    ui->labeHead->setMouseTracking(true);
    ui->labelChangeName->setMouseTracking(true);
    ui->labelChangeName->installEventFilter(this);
    ui->labeHead->installEventFilter(this);
    ui->widgetGround->setWindowOpacity(0.7);
    ui->widgetImel->show();
    ui->widgetPass->hide();
    ui->labelChangeName->setText("切换用户");
    ui->labelTitle->setText(Global::IMConfig->value("labelTitle/title").toString());
    ui->labelVersion->setText(Global::IMConfig->value("labelTitle/version").toString());
    ui->labelVersion->setStyleSheet("QLabel {background-color: transparent;}");
    ui->labelTitle->setStyleSheet("QLabel {background-color: transparent;}");

    initIcon();
    QFile fileInit(":/bookKeeping.qss");
    if(fileInit.open(QFile::ReadOnly)){
        qApp->setStyleSheet(QLatin1String(fileInit.readAll()));
        fileInit.close();
    }

//    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
//    effect->setOffset(3);
//    effect->setColor(QColor(43, 43, 43));
//    effect->setBlurRadius(1 0);
//    ui->widgetBackGround->setGraphicsEffect(effect);

    QString cpuId = Global::getWMIC("wmic cpu get processorid");
    QString serialNumber = Global::getWMIC("wmic diskdrive get serialnumber");
    UUID = cpuId.append(serialNumber);
    UUID = UUID.replace(" ", "");

    QString userName = Global::IMConfig->value("login/username").toString();
    if(userName.isNull() || userName.isEmpty()){
        showLoginMenu();
    }
    imelStatus = Global::IMConfig->value("login/start").toString();
    if(imelStatus != "true"){
        showLoginMenu();
    } else {
        mainlink.setUrl(Global::bookUrl + "/user/loginByImel");
        QJsonObject json;
        json.insert("account",userName);
        json.insert("imel",UUID);
        mainlink.setJson(json);
        mainlink.post();
    }
    connect(&mainlink, &Cvlink::imelLogin, this, &LoginDialog::imelResult);
    connect(&mainlink, &Cvlink::passwordLogin, this, &LoginDialog::passwordLogin);
}

/**
* @brief    析构函数
* @params
* @author   Joker
*/
LoginDialog::~LoginDialog()
{
    delete ui;
    delete manager;

}

void LoginDialog::mousePressEvent(QMouseEvent *event){
    this->relativePos = event->globalPos() - this->pos();
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->relativePos);
}

bool LoginDialog::eventFilter(QObject *obj, QEvent *event)
{

    if(obj == ui->labeHead){
        if(event->type()==QEvent::MouseButtonPress)
        {
            if(loginFlag != "success"){
                QMessageBox::warning(this, "警告", "此设备未登录过，请使用账户密码登录", "OK");
                showLoginMenu();
            } else {
                on_pushButtonLogin_clicked();
            }

        }
        if(event->type()==QEvent::MouseMove)
        {

        }
    } else if(obj == ui->labelChangeName){
        if(event->type()==QEvent::MouseButtonPress)
        {
            loginFlag = "failed";
            showLoginMenu();
        }
    }

   return QDialog::eventFilter(obj, event);
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *event){
    if(this->pos().y()>(QApplication::desktop()->availableGeometry().height()-260)){
        this->move(this->pos().x(),QApplication::desktop()->availableGeometry().height()-260);
    }
    event->ignore();
}

void LoginDialog::on_pushButtonLogin_clicked()
{
    if(loginFlag == "failed"){
        QString username = ui->lineEditUser->text().trimmed();
        QString password = ui->lineEditPass->text().trimmed();

        if(username.isEmpty() || password.isEmpty()){
            QMessageBox::warning(this, "警告", "账号或密码不能为空！", "OK");
            return;
        }

        userName = username;
        passWord = password;

        mainlink.setUrl(Global::bookUrl + "/user/loginByPassword");
        QJsonObject json;
        json.insert("account", username);
        json.insert("password", password);
        json.insert("imel", UUID);
        mainlink.setJson(json);
        mainlink.post();

    } else if(loginFlag == "success"){
        loginFlag = "success";
        if(userName!="" && passWord!=""){
            Global::IMConfig->setValue("login/username",userName);
            Global::IMConfig->setValue("login/password",passWord);
        }
        this->accept();
    }
}

void LoginDialog::on_pushButtonClose_clicked()
{
    this->close();
}

void LoginDialog::imelResult(bool result, QString msg)
{
    if(result){
        loginFlag = "success";
        ui->labelChangeName->show();
    } else {
        loginFlag = "failed";
        QMessageBox::warning(this, "错误", msg, "OK");
    }
}

void LoginDialog::passwordLogin(bool result, QString msg)
{
    if(result){
        loginFlag = "success";
        if(userName!="" && passWord!=""){
            Global::IMConfig->setValue("login/username",userName);
            Global::IMConfig->setValue("login/password",passWord);
        }
        this->accept();
        if(imelStatus == "" || imelStatus == "false"){
            Global::IMConfig->setValue("login/start", "true");
        }
    } else {
        loginFlag = "failed";
        QMessageBox::warning(this, "错误", msg, "OK");
    }

}

/**
* @brief    显示登录界面
* @params
* @author   Joker
*/
LoginDialog::showLoginMenu()
{
    loginFlag = "failed";
    ui->widgetPass->show();
    ui->widgetImel->hide();
}

void LoginDialog::initIcon()
{
    QPixmap *pixmap = new QPixmap(QApplication::applicationDirPath() + "/images/head.jpg");
    if(pixmap->isNull()){
        pixmap = new QPixmap(":/images/images/Head.png");
    }
    pixmap->scaled(ui->labeHead->size(), Qt::KeepAspectRatio);
    ui->labeHead->setScaledContents(true);
    ui->labeHead->setPixmap(*pixmap);


    QPixmap pixMap= pixmap->scaled(100,100, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pixMap =  Global::PixmapToRound(pixMap, 50);
    ui->labelLittelHead->setPixmap(pixMap);

    QPalette pal = ui->widgetBackGround->palette();
    pal.setBrush(QPalette::Background, QBrush(QPixmap(QApplication::applicationDirPath() + "/images/bg.jpg")));
    ui->widgetBackGround->setPalette(pal);
}
