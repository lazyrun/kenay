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
   for (int i = 1; i < 10; i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      oppList << opp;
      //получить статистику по оппу
      //session_->stat(opp.nick().hash());
   }
   return Solution();
}


