#include "mainwindow.h"
#include <QApplication>
#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = QApplication::applicationDirPath() + "/configure/IMConfig.ini";
    Global::IMConfig = new QSettings(path,QSettings::IniFormat);
    Global::IMConfig->setIniCodec("GB2312"); //设置编码防止出现中文乱码

    Global::bookUrl = Global::IMConfig->value("login/path").toString();

    LoginDialog *loginDialog = new LoginDialog();
    if(loginDialog->exec() == QDialog::Accepted){
        QApplication::setQuitOnLastWindowClosed(false);
        QMainWindow window;
        MainWindow main(&window);
        main.show();
        int e = a.exec();
        if(e == 773){
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
            return 0;
        }
        return e;
    }else{
        return 0;
    }
}

