/*! \file Session.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef Session_H
#define Session_H

#include "ImageProc.h"
#include "Opp.h"

/*!
\class Session
\brief 
*/
typedef QList<Opp::Action> ActionList;

class Session
{
public:   
   Session(CardProcessing * const proc, int cnt);
   //! сохранить статистику из текущей истории
   void saveStats(const QString & sessionId);
   //! Заполнить статистику оппа
   void stat(Opp & opp);
   //!
   int tableFormat() const {return cnt_; };
protected:   
   void saveToDB();
   CardProcessing * const proc_;
   QString sessionID_;
   QMap<CardProcessing::Street, QMap<int, ActionList> > history_;
   QMap<int, Opp> opps_;
   int cnt_;
};
   // активность оппа5 на префлопе
   //история
   /*
   сессия AsQd
     Префлоп
        опп1
           рейз, рейз
        опп2 
           фолд
        опп3
           фолд
        круг1
           опп1 - сб
           опп2 - бб
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг2
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг3
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
     Флоп
        круг1
           опп1 - сб
           опп2 - бб
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг2
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг3
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз


   */

#endif

