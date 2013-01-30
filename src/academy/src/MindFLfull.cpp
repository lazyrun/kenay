#include "MindFLfull.h"

MindFLfull::MindFLfull(CardProcessing * const proc, Session * const session)
: Mind(proc, session)
{
}

Solution MindFLfull::preflopSolution()
{
   //определить позицию
   preflopPosition();
   //определить круг торговли
   int trade = tradeInPreflop();
   
   if (trade == 0)
   {
      return round0Solution();
   }
   else//не первый, т.е. был рейз после меня
   {
   }
   
   return Solution();
}

Solution MindFLfull::round0Solution()
{
   const qreal tightThreshold = 28.;
   //количество тайтовых оппонентов
   int tight = 0, live = 0;
   foreach (Opp opp, oppList_)
   {
      if (!opp.isInGame())
         continue;
      if (opp.vpip() > 1. && opp.vpip() < tightThreshold )
      {
         tight++;
      }
      live++;
   }
   //процентное выражение тайтовости
   qreal tightPercent = qreal(tight) * 100. / qreal(live);

   //количество активных оппов
   limpers_ = 0;
   raisers_ = 0;
   foreach (Opp opp, oppList_)
   {
      //опп с картами и уже чо то сказал
      if (opp.hasCards() && opp.action() != Opp::Nope)
      {
         if (opp.action() == Opp::Call)
            limpers_++;
         else if (opp.action() == Opp::Raise)
            raisers_++;
      }
   }
   //использовать рекомендации для тайтовой игры
   bool useTight = false;
   //первый круг
   if (preflopPos_ == HighJack || preflopPos_ == Button)
   {
      if ((limpers_ + raisers_ <= 3) && (limpers_ + raisers_ > 0) && 
          preflopPos_ == Button)
      {
         //1, 2 лимпера и 1 рейзер
         useTight = true;
      }
      else if ((limpers_ + raisers_ <= 2) && (limpers_ + raisers_ > 0) && 
               preflopPos_ == HighJack)
      {
         useTight = true;
      }
      else if (limpers_ + raisers_ == 0)
      {
         //играть лузовее
         useTight = false;
      }
      else//больше трех уже
      {
         //в зависимости от тайтовости стола
         if (tightPercent > 50.)
         {
            useTight = true;
         }
      }
   }
   else//какие-то другие позиции UTG, Middle, sb, bb
   {
      //в зависимости от тайтовости стола
      if (tightPercent > 50.)
      {
         useTight = true;
      }
   }
   
   if (useTight)
   {
      return tightPreflop();
   }
   
   return loosePreflop();
}

void MindFLfull::preflopPosition()
{
   if (proc_->isDealer())
   {
      preflopPos_ = Button;
   }
   else 
   {
      //количество живых игроков
      QList<Opp> liveOpps;
      foreach (Opp opp, oppList_)
      {
         if (opp.isInGame())
         {
            liveOpps << opp;
         }
      }
      //позиция дилера
      int dpos = -1;
      for (int i = 0; i < liveOpps.count(); i++)
      {
         if (liveOpps.at(i).isDealer())
         {
            dpos = i;
            break;
         }
      }
      Q_ASSERT_X(dpos != -1, "preflopPosition", 
         "Dealer position isn't found!");
      
      int me = liveOpps.count() - dpos;
      if (me == 1)
      {
         preflopPos_ = SmallBlind;
      }
      else if (me == 2)
      {
         preflopPos_ = BigBlind;
      }
      else if (me == liveOpps.count())
      {
         preflopPos_ = HighJack;
      }
      else if (me < liveOpps.count() && me >= liveOpps.count() - 3)
      {
         preflopPos_ = Middle;
      }
      else
      {
         preflopPos_ = UTG;
      }
   }
}

int MindFLfull::tradeInPreflop()
{
   return 0;
}

Solution MindFLfull::tightPreflop()
{
   switch (preflopPos_)
   {
      case SmallBlind: 
         return sbTight();
      case BigBlind: 
         return bbTight();
      case UTG: 
         return utgTight();
      case Middle: 
         return mTight();
      case HighJack:
      case Button:
         return buTight();
   }
   return Solution();
}

Solution MindFLfull::sbTight()
{
   /*
   Не было рейза
     лимп
     рейз
   Был рейз
     лимп
     рейз
   Был рейз и ререйз
     лимп
     рейз
   */

   if (raisers_ == 0)
   {
      return sbTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      return sbTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      return sbTightMoreRaise();
   }

   return Solution();
}

Solution MindFLfull::sbTightNoRaise()
{
   
   return Solution();
}

Solution MindFLfull::sbTightOneRaise()
{
   //учесть что рейзер может быть лузовым - тогда играть шире
   //также учесть как долго я не играю
   return Solution();
}

Solution MindFLfull::sbTightMoreRaise()
{
   return Solution();
}

Solution MindFLfull::bbTight()
{
   return Solution();
}

Solution MindFLfull::utgTight()
{
   return Solution();
}

Solution MindFLfull::mTight()
{
   return Solution();
}

Solution MindFLfull::buTight()
{
   return Solution();
}

Solution MindFLfull::loosePreflop()
{
   return Solution();
}

Solution MindFLfull::flopSolution()
{
   return Solution();
}

Solution MindFLfull::turnSolution()
{
   return Solution();
}

Solution MindFLfull::riverSolution()
{
   return Solution();
}

