#include "MindFLfull.h"

MindFLfull::MindFLfull(CardProcessing * const proc, Session * const session)
: Mind(proc, session)
{
}

Solution MindFLfull::think()
{
   //1) посмотреть свои карты
   HoleCards hole(proc_->holeCard("first"), proc_->holeCard("second"));
   //сохранить статистику
   session_->saveStats(hole.fullName());
   //определение стиля стола (тайтовый стол или лузовый)
   //посчитать коэффициенты тайтовости/лузовости для каждого игрока
   QList<Opp> oppList;
   int tight = 0;
   for (int i = 1; i < 10; i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      //получить статистику по оппу
      session_->stat(opp);
      if (opp.vpip() > 1. && opp.vpip() < 20. )
         tight++;
      oppList << opp;
   }
   if (tight > 4)
   {
      //стол тайтовый
   }
   else
   {
      //стол лузовый
   }
   return Solution();
}


