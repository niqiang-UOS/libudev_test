#ifndef LOG_H
#define LOG_H
 
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
 
//默认调试级别为warning，即小于warning级别的都不会写入日志文件
//只有release版本的时候，才会输出到日志，debug版本正常输出到终端。
namespace QT_LOG
{
    //默认文件名为当前时间命名的log文件
	static int m_LogLevel = 1;
	static QString m_LogFile = QString("%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
	QMutex m_LogMutex;
 
    void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	{
        // 加锁
        static QMutex mutex;
        mutex.lock();

        QByteArray localMsg = msg.toLocal8Bit();

        QString strMsg("");
        switch(type)
        {
        case QtDebugMsg:
            strMsg = QString("Debug:");
            break;
        case QtWarningMsg:
            strMsg = QString("Warning:");
            break;
        case QtCriticalMsg:
            strMsg = QString("Critical:");
            break;
        case QtFatalMsg:
            strMsg = QString("Fatal:");
            break;
        }

        // 设置输出信息格式
        //QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
        //QString strMessage = QString("%1:%2 --%3:%4:%5")
        //        .arg(strDateTime).arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);

        QString strDateTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        QString strMessage = QString("%1:%2").arg(strDateTime).arg(localMsg.constData());
        // 输出信息至文件中（读写、追加形式）
        QFile file(m_LogFile);
        file.open(QIODevice::ReadWrite | QIODevice::Append);
        QTextStream stream(&file);
        stream << strMessage << "\r\n";
        file.flush();
        file.close();

        // 解锁
        mutex.unlock();
	}

    //默认调试级别为warning及以上才会写入日志文件，默认log文件名为程序启动时间命名的log文件
	void logInit(QString logFile = "",int logLevel = 1)
	{
		#ifndef _DEBUG  //实现debug版本的时候，输出到终端；release版本的时候输出到日志文件
			if ((logLevel < 0) || (logLevel > 3))
			{
				m_LogLevel = 1;
			}
			else
			{
				m_LogLevel = logLevel;
			}
			
			if (!logFile.isEmpty())
			{
				m_LogFile = logFile;
			}
 

            qInstallMessageHandler(customMessageHandler);
		#endif 
	}
};
 
 
#endif // LOG_H
