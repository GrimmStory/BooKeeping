#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    this->setWindowTitle("记账助手");
    initMenu();
    initHead();
    QMap<QString, QString> map;
    map.insert("getUserInfo","true");
    map.insert("userName",Global::IMConfig->value("login/username").toString());
    mainlink.postHttpRequest(Global::bookUrl + "/lodgingHouse/login_user", map);
    mainlink.postHttpRequest(Global::bookUrl + "/user/getDetail", map);
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setToolTip("BOOK");
    trayIcon->setIcon(QIcon(":/images/images/desktop.png"));
    reSetAction = new QAction(tr("最大化"), this);
    connect(reSetAction, &QAction::triggered, this, MainWindow::showMenu);
    quitAction = new QAction(tr("退出"), this);
    connect(quitAction, &QAction::triggered, this, MainWindow::closeMenu);
    trayMenu = new QMenu();
    trayMenu->addAction(reSetAction);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(&mainlink, &Cvlink::queryUsers, this, &MainWindow::queryUsers);
    connect(&mainlink, &Cvlink::queryBookKeeping, this, &MainWindow::queryBookKeeping);
    connect(&mainlink, &Cvlink::bookResult, this, &MainWindow::bookResult);
    connect(&mainlink, &Cvlink::lastLoginTime, this, &MainWindow::lastLoginTime);

}



/**
* @brief    析构函数
* @params
* @author   Joker
*/
MainWindow::~MainWindow()
{
    delete ui;
    delete trayMenu;
    delete userInfoSetup;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->labelHead){
        if(event->type()==QEvent::MouseButtonPress)
        {
            qDebug() << "head!" << endl;

        }
        if(event->type()==QEvent::MouseMove)
        {

        }
    }

   return MainWindow::eventFilter(obj, event);
}

void MainWindow::initHead()
{
    QString path = QApplication::applicationDirPath().append("/images/user/head");
    QPixmap *pixmap = new QPixmap(path);
    if(pixmap->isNull()){
        pixmap = new QPixmap(":/images/images/defalut.jpg");
        pixmap->scaled(ui->labelHead->size(), Qt::KeepAspectRatio);
        ui->labelHead->setScaledContents(true);
        ui->labelHead->setPixmap(*pixmap);
    }
}

MainWindow::initMenu()
{
    userInfoSetup = new userInfoDialog();
    ui->widgetBook->hide();
    ui->widgetHistory->hide();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->Textlabel->setText("页");
    ui->treeWidgetHistory->setColumnCount(7);
    ui->treeWidgetHistory->setColumnWidth(0,40);
    ui->treeWidgetHistory->setColumnWidth(1,106);
    ui->treeWidgetHistory->setColumnWidth(2,106);
    ui->treeWidgetHistory->setColumnWidth(3,63);
    ui->treeWidgetHistory->setColumnWidth(4,64);
    QStringList lableList;
    lableList.append("序号");
    lableList.append("入账时间");
    lableList.append("账单日期");
    lableList.append("付款人");
    lableList.append("金额");
    lableList.append("参与人");
    lableList.append("备注");
    ui->treeWidgetHistory->setHeaderLabels(lableList);
    ui->lineEditPS->setAlignment(Qt::AlignTop);
    ui->pushButtonFirst->hide();
    ui->pushButtonLast->hide();
    QMenu *menu = ui->menuBar->addMenu("账单");
    QAction *bookAction = menu->addAction("入账");
    connect(bookAction, &QAction::triggered, this, &MainWindow::bookKeeping);
    QAction *outBookAction = menu->addAction("出账");
    connect(outBookAction, &QAction::triggered, this, &MainWindow::outBookKeeping);
    QAction *bookHistoryAction = menu->addAction("历史账单");
    connect(bookHistoryAction, &QAction::triggered, this, &MainWindow::historyKeeping);
    menu->addSeparator();
    QAction *quitAction = menu->addAction("退出");
    connect(quitAction, &QAction::triggered, this, [=]{
        qApp->quit();
    });
    QMenu *menu1 = ui->menuBar->addMenu("设置");
    QAction *userInfoSetup = menu1->addAction("个人信息");
    connect(userInfoSetup, &QAction::triggered, this, &MainWindow::userInfoSetupShow);
    globalNum = 1;
    pageSize = Global::IMConfig->value("mainwindow/pagesize").toString();
}


/**
* @brief    显示入账，隐藏其他界面
* @params
* @author   Joker
*/
void MainWindow::bookKeeping()
{
    ui->widgetBook->show();
    ui->widgetHistory->hide();
    ui->widgetOut->hide();
}

/**
* @brief    显示出账界面
* @params
* @author   Joker
*/
void MainWindow::outBookKeeping()
{
    ui->widgetBook->hide();
    ui->widgetHistory->hide();
    QString imageMD5 = Global::imageToBase64("C:/Users/Joker/Desktop/timg.jpg");
    QFile file("C:/Users/Joker/Desktop/temp.txt");
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        file.write(imageMD5.toUtf8());
    }
}

/**
* @brief    初始化所属宿舍成员
* @params
* @author   Joker
*/
void MainWindow::initOrg(QMap<int, QString> userMap)
{
    QTreeWidgetItem *itemRoom = new QTreeWidgetItem(ui->treeWidgetPerson);
    itemRoom->setText(0,room);

    foreach (int id, userMap.keys()) {
        QTreeWidgetItem *item = new QTreeWidgetItem(itemRoom);
        item->setData(0, Qt::UserRole, id);
        item->setText(0,userMap[id]);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        item->setCheckState(0,Qt::Unchecked);
    }

}
/**
* @brief    入账点击事件
* @params
* @author   Joker
*/
void MainWindow::on_pushButtonBook_clicked()
{
    QString dateTime = Global::getTime_t(ui->dateTimeEdit->dateTime());
    qDebug() << "dateTime:" << dateTime << endl;
    QString idList;
    QTreeWidgetItem *item = ui->treeWidgetPerson->topLevelItem(0);

    for(int i = 0; i < item->childCount(); i ++){
        if(item->child(i)->checkState(0) == Qt::Checked){
            idList.append(item->child(i)->data(0,Qt::UserRole).toString());
            idList.append(",");
        }
    }

    idList = idList.mid(0, idList.length() - 1);

    QString keepingMoney = ui->lineEditMoney->text().trimmed();
    if(keepingMoney.isNull() || keepingMoney.isEmpty()){
        QMessageBox::warning(this, "警告", "金额不能为空！", "OK");
    }

    QString keepingPS = ui->lineEditPS->text();
    QMap<QString, QString> bookKeepingMap;
    bookKeepingMap.insert("keepingTime", dateTime);
    bookKeepingMap.insert("participants", idList);
    bookKeepingMap.insert("money", keepingMoney);
    bookKeepingMap.insert("comment", keepingPS);
    bookKeepingMap.insert("credential","");
    bookKeepingMap.insert("lodgingHouseId",roomId);

    mainlink.postHttpRequest(Global::bookUrl + "/bookkeeping", bookKeepingMap);
}

void MainWindow::historyKeeping()
{
    ui->widgetBook->hide();
    ui->labelPage->setText(QString::number(globalNum));
    ui->widgetHistory->show();
    ui->widgetOut->hide();
    ui->treeWidgetHistory->clear();
    addFlag = true;
    globalNum = 1;
    ui->labelPage->setText(QString::number(globalNum));
    QMap<QString, QString> bookHistory;
    bookHistory.insert("pageNum", QString::number(globalNum));
    bookHistory.insert("pageSize", pageSize);
    mainlink.postHttpRequest(Global::bookUrl + "/bookkeeping/all", bookHistory);
}

void MainWindow::on_pushButtonFirst_clicked()
{
    QMap<QString, QString> bookHistory;
    bookHistory.insert("pageNum", "1");
    bookHistory.insert("pageSize", pageSize);
    mainlink.postHttpRequest(Global::bookUrl + "/bookkeeping/all", bookHistory);

}

void MainWindow::on_pushButtonPre_clicked()
{
    globalNum --;
    if(globalNum == 0){
        ui->treeWidgetHistory->clear();
        QMap<QString, QString> bookHistory;
        bookHistory.insert("pageNum", "1");
        bookHistory.insert("pageSize", pageSize);
        mainlink.postHttpRequest(Global::bookUrl + "/bookkeeping/all", bookHistory);
        globalNum = 1;
        addFlag = true;
        ui->labelPage->setText(QString::number(globalNum));
    } else {
        addFlag = true;
        ui->treeWidgetHistory->clear();
        QMap<QString, QString> bookHistory;
        bookHistory.insert("pageNum", QString::number(globalNum));
        bookHistory.insert("pageSize", pageSize);
        mainlink.postHttpRequest(Global::bookUrl + "/bookkeeping/all", bookHistory);
        ui->labelPage->setText(QString::number(globalNum));
    }

}

void MainWindow::on_pushButtonNext_clicked()
{
    if(addFlag){
        globalNum ++;
    }
    ui->treeWidgetHistory->clear();
    QMap<QString, QString> bookHistory;
    bookHistory.insert("pageNum", QString::number(globalNum));
    bookHistory.insert("pageSize", pageSize);
    mainlink.postHttpRequest(Global::bookUrl + "/bookkeeping/all", bookHistory);
    ui->labelPage->setText(QString::number(globalNum));
}

void MainWindow::on_pushButtonLast_clicked()
{

}

void MainWindow::showMenu()
{
    this->show();
}

void MainWindow::closeMenu()
{
    qApp->quit();
}

void MainWindow::queryUsers(QMap<int, QString> map, QString id, QString roomName)
{
    qDebug() << "this is test." << endl;
    roomId = id;
    room = roomName;
    initOrg(map);
}

void MainWindow::queryBookKeeping(QJsonArray dataArrary)
{
    if(dataArrary.size() < pageSize.toInt()){
        addFlag = false;
    }
    for(int i = 0; i < dataArrary.size(); i ++){
        QJsonObject data = dataArrary.at(i).toObject();
        qint8 id = i + 1;
        qint64 keepingTime = data["keepingTime"].toDouble();
        QString keepTime = Global::parseTime_t(keepingTime);

        QString comment = data["comment"].toString();
        qint64 entryTime = data["entryTime"].toDouble();
        QString enTime = Global::parseTime_t(entryTime);

        QJsonObject payerObject = data["payer"].toObject();
        QString payerName = payerObject["name"].toString();

        QJsonArray participantArray = data["participant"].toArray();
        QList<QString> participantList;
        for(int i = 0; i < participantArray.size(); i ++){
            QJsonObject tmepObject = participantArray.at(i).toObject();
            participantList.append(tmepObject["name"].toString());
        }

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QString::number(id));
        item->setText(1, keepTime);
        item->setToolTip(1, keepTime);
        item->setText(2, enTime);
        item->setToolTip(2, enTime);
        item->setText(3, payerName);
        item->setText(4, QString::number(data["money"].toDouble(),10,2));
        QString participantPerson = participantList.join(",");
        participantPerson.mid(0, participantPerson.length() - 1);
        item->setText(5, participantPerson);
        item->setToolTip(5, participantPerson);
        item->setText(6, comment);
        item->setToolTip(6, comment);
        ui->treeWidgetHistory->addTopLevelItem(item);
    }
}

void MainWindow::bookResult(bool bookResult)
{
    if(bookResult){
        QMessageBox::warning(this, "入账","入账成功","OK");
        QTreeWidgetItem *item = ui->treeWidgetPerson->topLevelItem(0);

        for(int i = 0; i < item->childCount(); i ++){
            if(item->child(i)->checkState(0) == Qt::Checked){
                item->child(i)->setCheckState(0, Qt::Unchecked);
            }
        }
        ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
        ui->lineEditMoney->clear();
        ui->lineEditPS->clear();
    }
}

void MainWindow::lastLoginTime(qint64 time)
{
    QString lastLoginTime = Global::parseTime_t(time);
    ui->labelLastTime->setText(lastLoginTime);
}

void MainWindow::userInfoSetupShow()
{
    userInfoSetup->show();
}


