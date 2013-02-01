#include "MindFLfull.h"
#include "GlobVars.h"

MindFLfull::MindFLfull(CardProcessing * const proc, Session * const session)
: Mind(proc, session)
{
   CGlobal::Instance().SetDbg(5);
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
   
   //для теста
   //return tightPreflop();
   CGlobal::Instance().ap1(QString("Tight opps: %1%%").arg(tightPercent));
   CGlobal::Instance().ap1(QString("Raisers: %1").arg(raisers_));
   CGlobal::Instance().ap1(QString("Limpers: %1").arg(limpers_));

   if (useTight)
   {
      CGlobal::Instance().ap1(QString("tightPreflop()"));
      return tightPreflop();
   }
   
   CGlobal::Instance().ap1(QString("loosePreflop()"));
   return tightPreflop();
   //return loosePreflop();
}

void MindFLfull::preflopPosition()
{
   if (proc_->isDealer())
   {
      CGlobal::Instance().ap1(QString("Position: Button"));
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
      CGlobal::Instance().ap1(QString("Opps In Game: %1").arg(liveOpps.count()));
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
         CGlobal::Instance().ap1(QString("Position: SmallBlind"));
         preflopPos_ = SmallBlind;
      }
      else if (me == 2)
      {
         CGlobal::Instance().ap1(QString("Position: BigBlind"));
         preflopPos_ = BigBlind;
      }
      else if (me == liveOpps.count())
      {
         CGlobal::Instance().ap1(QString("Position: HighJack"));
         preflopPos_ = HighJack;
      }
      else if (me < liveOpps.count() && me >= liveOpps.count() - 3)
      {
         CGlobal::Instance().ap1(QString("Position: Middle"));
         preflopPos_ = Middle;
      }
      else
      {
         CGlobal::Instance().ap1(QString("Position: UTG"));
         preflopPos_ = UTG;
      }
   }
}

int MindFLfull::tradeInPreflop()
{
   int r = 0;
   CGlobal::Instance().ap1(QString("Trade Round: %1").arg(r));
   return r;
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
      CGlobal::Instance().ap2(QString("sbTightNoRaise()")); 
      sol = sbTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("sbTightOneRaise()")); 
      sol = sbTightOneRaise();
   }
   else if (raisers_ >= 1)
   {
      CGlobal::Instance().ap2(QString("sbTightMoreRaise()"));
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("99+") 
              << parseRange("ATs+") 
              << parseRange("KJs+")
              << parseRange("AQo+");
   limpRange << parseRange("22+") 
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
   
   CGlobal::Instance().ap2(QString("suited: %1 nominal: %2").arg(suited).arg(nominal));
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Call"));
      sol.setAction(Solution::Call);
   }
   else if (hole_.isSuited())
   {
      CGlobal::Instance().ap1(QString("isSuited() -> Solution::Call"));
      sol.setAction(Solution::Call);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::sbTightOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("TT+") << "AKs" << "AKo";
   limpRange << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   CGlobal::Instance().ap2(QString("suited: %1 nominal: %2").arg(suited).arg(nominal));
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else if (raisers_ == 1 && limpers_ >= 1)
   {
      limpRange.clear();
      limpRange << parseRange("22+");
      if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList raiseRange;
   raiseRange << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("99+") 
              << parseRange("ATs+") 
              << parseRange("KJs+")
              << parseRange("AQo+");
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("TT+") << "AKs" << "AKo";
   limpRange << parseRange("22+")
             << parseRange("ATs+")
             << "JTs"
             << parseRange("QTs+")
             << parseRange("KTs+")
             << parseRange("AJo+")
             << "KQo";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList raiseRange;
   raiseRange << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("TT+") 
              << parseRange("AJs+") 
              << parseRange("AQo+");
   limpRange << parseRange("77+") 
             << parseRange("AJo+")
             << parseRange("JTs+")
             << parseRange("QTs+")
             << parseRange("KTs+")
             << parseRange("ATs+")
             << "KQo";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("TT+") << "AKs" << "AKo";
   limpRange << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList raiseRange;
   raiseRange << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("99+") 
              << parseRange("ATs+") 
              << parseRange("KJs+")
              << parseRange("AJo+")
              << "KQo";
   limpRange << parseRange("22+") 
             << parseRange("A2s+")
             << parseRange("K9s+")
             << parseRange("Q9s+")
             << parseRange("J9s+")
             << parseRange("ATo+")
             << parseRange("KJo+")
             << "T9s" << "98s";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("TT+") << "AKs" << "AKo";
   limpRange << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList raiseRange;
   raiseRange << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("99+") 
              << parseRange("A8s+") 
              << parseRange("KTs+")
              << parseRange("ATo+")
              << "QJs" << "KQo";
   limpRange << parseRange("22+") 
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
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRange("TT+") << "AKs" << "AKo";
   limpRange << parseRange("AJs+")
             << "AKo"
             << "KQs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      sol.setAction(Solution::Raise);
   }
   else if (limpRange.contains(suited) || limpRange.contains(nominal))
   {
      sol.setAction(Solution::Call);
   }
   else if (limpers_ == 2 && raisers_ == 1)
   {
      limpRange.clear();
      limpRange << parseRange("22+") << "QJs" << "JTs" << "T9s";
      if (limpRange.contains(suited) || limpRange.contains(nominal))
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
   QStringList raiseRange;
   raiseRange << parseRange("QQ+") << "AKs";
   
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
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

