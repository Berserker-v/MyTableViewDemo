#include "mainwindow.h"
#include <QApplication>
#include <QDateTime>
#include <QTextCodec>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    //解决中文乱码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageOutput);
    MainWindow w;
    w.show();

    return a.exec();
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("DEBUG");
        break;

    case QtWarningMsg:
        text = QString("WARN");
        break;

    case QtCriticalMsg:
        text = QString("CRITICAL");
        break;

    case QtFatalMsg:
        text = QString("FATAL");
    }

    /* 截取源文件相对位置 */
    QString filepath = QString(context.file);
    int begin = filepath.indexOf('\\', 3);
    filepath = filepath.mid(begin + 1);

    QDateTime now = QDateTime::currentDateTime();
    QString context_info = QString("[%1:%2]").arg(filepath).arg(context.line);
    QString current_date_time = now.toString("[hh:mm:ss]");
    QString message = QString("%4-%1-%2:%3").arg(text).arg(context_info).arg(msg).arg(current_date_time);

    printf("%s\n", message.toLocal8Bit().data());
    fflush(stdout);
}
