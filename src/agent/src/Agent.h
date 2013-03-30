#ifndef AGENT_H
#define AGENT_H

#include <qt_windows.h>

typedef void (CALLBACK * showMessageCBFunc)(const char * hand);

typedef void (*ExecStart)(showMessageCBFunc);
typedef void (*ExecStop)();
typedef void (*ExecKill)();
typedef void (*ExecName)(char *, int);
typedef void (*ExecStartRoom)();

class QxtGlobalShortcut;
class Agent : public QObject
{
   Q_OBJECT
public:
   //! ctor.
   Agent(QObject * parent = 0);
   //! 
   void load(const QString & module);
public slots:
   //!
   void handleMessage(const QString & command);
private slots:
   //! Start Bot
   void start();
   //! Stop Bot
   void stop();
   //! Stop Bot
   void restart();
   //!
   void exit();
   //!
   void iconActivated(QSystemTrayIcon::ActivationReason);
   //!
   void startRoom();
private:
   void setupUi();
   //!
   void createTrayIcon();
   //!
   QStringList loadedModules_;
   //!
   QMenu * trayIconMenu_;
   //!
   QxtGlobalShortcut* startShortcut_;
   //!
   QxtGlobalShortcut* stopShortcut_;
   //!
   QxtGlobalShortcut* restartShortcut_;
   //!
   QString configPath_;
   QAction * stopAction_;
   QAction * startAction_;
   QAction * restartAction_;

   QList<ExecStart> f_startList_;
   QList<ExecStop> f_stopList_;
   QList<ExecKill> f_killList_;
   QList<ExecName> f_nameList_;
   ExecStartRoom f_room;
};

#endif
