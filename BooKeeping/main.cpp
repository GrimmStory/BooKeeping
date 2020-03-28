#include "mainwindow.h"
#include <QApplication>
#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString path = QApplication::applicationDirPath() + "/configure/IMConfig.ini";
    Global::IMConfig = new QSettings(path,QSettings::IniFormat);
    Global::IMConfig->setIniCodec("GB2312"); //设置编码防止出现中文乱码

#ifdef QT_NO_DEBUG
    Global::bookUrl = "https://bookkeeping.javed.club";
#else
    Global::bookUrl = "https://bookkeeping-test.javed.club";
#endif

    LoginDialog *loginDialog = new LoginDialog();
    if(loginDialog->exec() == QDialog::Accepted){
        QApplication::setQuitOnLastWindowClosed(false);
        QMainWindow window;
        MainWindow main(&window);
        main.show();
        return a.exec();
    }else{
        return 0;
    }
}

