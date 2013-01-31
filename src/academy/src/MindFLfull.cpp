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
   
   //дет теста
   return tightPreflop();
   
   //if (useTight)
   //{
   //   return tightPreflop();
   //}
   //
   //return loosePreflop();
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

//
/* ----------SB Tight----------------- */
//
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
   Solution sol;
   if (raisers_ == 0)
   {
       sol = sbTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      sol = sbTightOneRaise();
   }
   else if (raisers_ >= 1)
   {
      sol = sbTightMoreRaise();
   }

   //учесть что рейзер может быть лузовым - тогда играть шире
   //также учесть как долго я не играю

   return sol;
}

Solution MindFLfull::sbTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("99+") 
              << parseRange("ATs+") 
              << parseRange("KJs+")
              << parseRange("AQo+");
   limpRagne << parseRange("22+") 
             << parseRange("A2s+")
             << parseRange("K9s+")
             << parseRange("Q9s+")
             << parseRange("J9s+")
             << parseRange("ATo+")
             << "T9s" << "98s"
             << "87s" << "76s"
             << "65s" << "54s"
             << "JTo"
             << parseRange("QTo+")
             << parseRange("KTo+");
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else if (hole_.isSuited())
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::sbTightOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("TT+") << "AKs" << "AKo";
   limpRagne << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else if (raisers_ == 1 && limpers_ >= 1)
   {
      limpRagne.clear();
      limpRagne << parseRange("22+");
      if (limpRagne.contains(suited) || limpRagne.contains(nominal))
      {
         sol.setAction(Solution::Call);
      }
      else
      {
         sol.setAction(Solution::Fold);
      }
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::sbTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRagne;
   raiseRagne << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}


//
/* ----------BB Tight----------------- */
//
Solution MindFLfull::bbTight()
{
   Solution sol;
   if (raisers_ == 0)
   {
       sol = bbTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      sol = bbTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      sol = bbTightMoreRaise();
   }
   //учесть что рейзер может быть лузовым - тогда играть шире
   //также учесть как долго я не играю

   return sol;

}

Solution MindFLfull::bbTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("99+") 
              << parseRange("ATs+") 
              << parseRange("KJs+")
              << parseRange("AQo+");
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else
   {
      sol.setAction(Solution::Check);
   }
   return sol;
}

Solution MindFLfull::bbTightOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("TT+") << "AKs" << "AKo";
   limpRagne << parseRange("22+")
             << parseRange("ATs+")
             << "JTs"
             << parseRange("QTs+")
             << parseRange("KTs+")
             << parseRange("AJo+")
             << "KQo";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::bbTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRagne;
   raiseRagne << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------UTG Tight----------------- */
//
Solution MindFLfull::utgTight()
{
   Solution sol;
   if (raisers_ == 0)
   {
       sol = utgTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      sol = utgTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      sol = utgTightMoreRaise();
   }
   //учесть что рейзер может быть лузовым - тогда играть шире
   //также учесть как долго я не играю

   return sol;

}

Solution MindFLfull::utgTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("TT+") 
              << parseRange("AJs+") 
              << parseRange("AQo+");
   limpRagne << parseRange("77+") 
             << parseRange("AJo+")
             << parseRange("JTs+")
             << parseRange("QTs+")
             << parseRange("KTs+")
             << parseRange("ATs+")
             << "KQo";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::utgTightOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("TT+") << "AKs" << "AKo";
   limpRagne << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::utgTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRagne;
   raiseRagne << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------Middle Tight----------------- */
//
Solution MindFLfull::mTight()
{
   Solution sol;
   if (raisers_ == 0)
   {
       sol = mTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      sol = mTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      sol = mTightMoreRaise();
   }
   //учесть что рейзер может быть лузовым - тогда играть шире
   //также учесть как долго я не играю

   return sol;
}

Solution MindFLfull::mTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("99+") 
              << parseRange("ATs+") 
              << parseRange("KJs+")
              << parseRange("AJo+")
              << "KQo";
   limpRagne << parseRange("22+") 
             << parseRange("A2s+")
             << parseRange("K9s+")
             << parseRange("Q9s+")
             << parseRange("J9s+")
             << parseRange("ATo+")
             << parseRange("KJo+")
             << "T9s" << "98s";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::mTightOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("TT+") << "AKs" << "AKo";
   limpRagne << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::mTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRagne;
   raiseRagne << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------Button, HJ Tight----------------- */
//
Solution MindFLfull::buTight()
{
   Solution sol;
   if (raisers_ == 0)
   {
       sol = buTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      sol = buTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      sol = buTightMoreRaise();
   }
   //учесть что рейзер может быть лузовым - тогда играть шире
   //также учесть как долго я не играю

   return sol;
}

Solution MindFLfull::buTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("99+") 
              << parseRange("A8s+") 
              << parseRange("KTs+")
              << parseRange("ATo+")
              << "QJs" << "KQo";
   limpRagne << parseRange("22+") 
             << parseRange("A2s+")
             << parseRange("K9s+")
             << parseRange("Q9s+")
             << parseRange("J9s+")
             << parseRange("ATo+")
             << "T9s" << "98s"
             << "87s" << "76s"
             << "65s" << "54s"
             << "JTo"
             << parseRange("QTo+")
             << parseRange("KTo+");
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::buTightOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRagne;
   QStringList raiseRagne;
   raiseRagne << parseRange("TT+") << "AKs" << "AKo";
   limpRagne << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRagne.contains(suited) || limpRagne.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else if (limpers_ == 2 && raisers_ == 1)
   {
      limpRagne.clear();
      limpRagne << parseRange("22+") << "QJs" << "JTs" << "T9s";
      if (limpRagne.contains(suited) || limpRagne.contains(nominal))
      {
         sol.setAction(Solution::Call);
      }
      else
      {
         sol.setAction(Solution::Fold);
      }
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::buTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRagne;
   raiseRagne << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRagne.contains(suited) || raiseRagne.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else
   {
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
// Loose Preflop
//
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

