#include "MindFLfull.h"
#include "GlobVars.h"

// ��������� ������� �� VPIP
#define MANIAK       44.
#define LOOSE        32.
#define MODERATE     24.
#define TIGHT        18.
#define EXTRATIGHT   1.

//����� �� PFR
#define PFR_PASSIVE 8.// ������, ������ ������
#define PFR_AGGRESIVE 12.

//����� �� �����
#define LIMP_PASSIVE 16.

MindFLfull::MindFLfull(CardProcessing * const proc, Session * const session)
: Mind(proc, session), 
  tightThreshold_(28.)
{
   pCareful_  = qMakePair(0., 18.);
   pTight_    = qMakePair(18., 24.);
   pModerate_ = qMakePair(24., 32.);
   pLoose_    = qMakePair(32., 44.);
   pManiak_   = qMakePair(44., 100.);

   ranCareful_    = "66+,A5s+,K9s+,Q9s+,J9s+,ATo+,KTo+,QTo+";
   ranTight_      = "66+,A2s+,K6s+,Q8s+,J8s+,T9s,A8o+,K9o+,QTo+,JTo";
   ranModerate_   = "55+,A2s+,K4s+,Q7s+,J8s+,T8s+,98s,A5o+,K8o+,Q9o+,J9o+,T9o";
   ranLoose_      = "22+,A2s+,K2s+,Q4s+,J6s+,T6s+,97s+,87s,A2o+,K6o+,Q8o+,J8o+,T8o+,98o";
   ranManiak_     = "22+,A2s+,K2s+,Q2s+,J4s+,T6s+,96s+,86s+,76s,65s,A2o+,K5o+,Q7o+,J7o+,T8o+,98o";

   CGlobal::Instance().SetDbg(2);
}

Solution MindFLfull::preflopSolution()
{
   CGlobal::Instance().ap2(QString("suited: %1 nominal: %2")
      .arg(hole_.suitedName()).arg(hole_.nominalName()));
   //���������� �������
   preflopPosition(); 
   //���������� ���� ��������
   int trade = tradeInPreflop();
   
   if (trade == 0)
   {
      return round0Solution();
   }
   else//�� ������, �.�. ��� ���� ����� ����
   {
      return roundASolution();
   }
   
   return Solution();
}

Solution MindFLfull::round0Solution()
{
   const qreal tightThreshold = 28.;
   //���������� �������� ����������
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
   //���������� ��������� ����������
   qreal tightPercent = qreal(tight) * 100. / qreal(live);

   //������������ ������������ ��� �������� ����
   useTight_ = false;
   //������ ����
   if (preflopPos_ == HighJack || preflopPos_ == Button)
   {
      if ((limpers_ + raisers_ <= 3) && (raisers_ > 0) && 
          preflopPos_ == Button)
      {
         //1, 2 ������� � 1 ������
         useTight_ = true;
      }
      else if ((limpers_ + raisers_ <= 3) && (raisers_ > 0) && 
               preflopPos_ == HighJack)
      {
         useTight_ = true;
      }
      else if (limpers_ + raisers_ == 0)
      {
         //������ �������
         useTight_ = false;
      }
      else//������ ���� ���
      {
         //� ����������� �� ���������� �����
         if (tightPercent > 50.)
         {
            useTight_ = true;
         }
      }
   }
   else//�����-�� ������ ������� UTG, Middle, sb, bb
   {
      //� ����������� �� ���������� �����
      if (tightPercent > 50.)
      {
         useTight_ = true;
      }
   }
   
   //��� �����
   //return tightPreflop();
   CGlobal::Instance().ap1(QString("Tight opps: %1%").arg(tightPercent));
   CGlobal::Instance().ap1(QString("Raisers: %1").arg(raisers_));
   CGlobal::Instance().ap1(QString("Limpers: %1").arg(limpers_));

   if (useTight_)
   {
      CGlobal::Instance().ap1(QString("tightPreflop()"));
      return tightPreflop();
   }
   
   CGlobal::Instance().ap1(QString("loosePreflop()"));
   return loosePreflop();
}

Solution MindFLfull::roundASolution()
{
   CGlobal::Instance().ap1(QString("roundASolution()"));
   CGlobal::Instance().ap1(QString("Raisers: %1").arg(raisers_));
   CGlobal::Instance().ap1(QString("Limpers: %1").arg(limpers_));

   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   limpRange << parseRangeList("88+,AQs,AQo,AJs");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap1(QString("Solution::Call"));
      sol.setAction(Solution::Call);
   }
   else if (raisers_ > 1)
   {
      if (limpRange.contains(suited) || limpRange.contains(nominal))
      {
         CGlobal::Instance().ap1(QString("Solution::Call"));
         sol.setAction(Solution::Call);
      }
      else
      {
         CGlobal::Instance().ap1(QString("Solution::Fold"));
         sol.setAction(Solution::Fold);
      }
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Call"));
      sol.setAction(Solution::Call);
   }
   return sol;
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
      //���������� ����� �������
      QList<Opp> liveOpps;
      foreach (Opp opp, oppList_)
      {
         if (opp.isInGame())
         {
            liveOpps << opp;
         }
      }
      CGlobal::Instance().ap1(QString("Opps In Game: %1").arg(liveOpps.count()));
      //������� ������
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
   int r = session_->preflopRound();
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
   �� ���� �����
     ����
     ����
   ��� ����
     ����
     ����
   ��� ���� � ������
     ����
     ����
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

   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::sbTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AQo+");
   limpRange << parseRangeList("22+,A2s+,K9s+,Q9s+,J9s+,ATo+,T9s,98s,87s,76s,65s,54s,JTo,QTo+,KTo+");
   
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
      CGlobal::Instance().ap1(QString("isSuited()==true -> Solution::Call"));
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
   raiseRange << parseRangeList("TT+,AKs,AKo");
   limpRange << parseRangeList("AJs+,AKo,KQs");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else if (raisers_ == 1 && limpers_ >= 1)
   {
      CGlobal::Instance().ap1(QString("(raisers_ == 1 && limpers_ >= 1)"));
      limpRange.clear();
      limpRange << parseRange("22+");
      if (limpRange.contains(suited) || limpRange.contains(nominal))
      {
         CGlobal::Instance().ap1(QString("Solution::Call"));
         sol.setAction(Solution::Call);
      }
      else
      {
         CGlobal::Instance().ap1(QString("Solution::Fold"));
         sol.setAction(Solution::Fold);
      }
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::sbTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
      CGlobal::Instance().ap2(QString("bbTightNoRaise()"));  
      sol = bbTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("bbTightOneRaise()"));
      sol = bbTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      CGlobal::Instance().ap2(QString("bbTightMoreRaise()"));
      sol = bbTightMoreRaise();
   }
   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;

}

Solution MindFLfull::bbTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AQo+");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Check"));
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
   raiseRange << parseRangeList("TT+,AKs,AKo");
   limpRange << parseRangeList("22+,ATs+,JTs,QTs+,KTs+,AJo+,KQo");
   
   //������� � ������ ������������� �����
   Solution sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::bbTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
      CGlobal::Instance().ap2(QString("utgTightNoRaise()"));   
      sol = utgTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("utgTightOneRaise()"));   
      sol = utgTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      CGlobal::Instance().ap2(QString("utgTightMoreRaise()")); 
      sol = utgTightMoreRaise();
   }
   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::utgTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("TT+,AJs+,AQo+");
   limpRange << parseRangeList("77+,AJo+,JTs,QTs+,KTs+,ATs+,KQo");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
   raiseRange << parseRangeList("TT+,AKs,AKo");
   limpRange << parseRangeList("AJs+,AKo,KQs");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::utgTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
      CGlobal::Instance().ap2(QString("mTightNoRaise()"));
      sol = mTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("mTightOneRaise()"));
      sol = mTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      CGlobal::Instance().ap2(QString("mTightMoreRaise()"));
      sol = mTightMoreRaise();
   }
   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::mTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AJo+,KQo");
   limpRange << parseRangeList("22+,A2s+,K9s+,Q9s+,J9s+,ATo+,KJo+,T9s,98s");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
   raiseRange << parseRangeList("TT+,AKs,AKo");
   limpRange << parseRangeList("AJs+,AKo,KQs");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::mTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
      CGlobal::Instance().ap2(QString("buTightNoRaise()")); 
      sol = buTightNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("buTightOneRaise()")); 
      sol = buTightOneRaise();
   }
   else if (raisers_ > 1)
   {
      CGlobal::Instance().ap2(QString("buTightMoreRaise()")); 
      sol = buTightMoreRaise();
   }
   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::buTightNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,A8s+,KTs+,ATo+,QJs,KQo");
   limpRange << parseRangeList("22+,A2s+,K9s+,Q9s+,J9s+,ATo+,T9s,98s,87s,76s,65s,54s,JTo,QTo+,KTo+");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
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
   raiseRange << parseRangeList("TT+,AKs,AKo");
   limpRange << parseRangeList("AJs+,AKo,KQs");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else if (limpers_ == 2 && raisers_ == 1)
   {
      CGlobal::Instance().ap1(QString("(limpers_ == 2 && raisers_ == 1)"));
      limpRange.clear();
      limpRange << parseRangeList("22+,QJs,JTs,T9s");
      CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));
      if (limpRange.contains(suited) || limpRange.contains(nominal))
      {
         CGlobal::Instance().ap1(QString("Solution::Call"));
         sol.setAction(Solution::Call);
      }
      else
      {
         CGlobal::Instance().ap1(QString("Solution::Fold"));
         sol.setAction(Solution::Fold);
      }
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::buTightMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
// Loose Preflop
//
Solution MindFLfull::loosePreflop()
{
   switch (preflopPos_)
   {
      case SmallBlind: 
         return sbLoose();
      case BigBlind: 
         return bbLoose();
      case UTG: 
         return utgLoose();
      case Middle: 
         return mLoose();
      case HighJack:
      case Button:
         return buLoose();
   }
   return Solution();
}

//
/* ----------UTG Loose----------------- */
//
Solution MindFLfull::utgLoose()
{
   Solution sol;
   if (raisers_ == 0)
   {
      CGlobal::Instance().ap2(QString("utgLooseNoRaise()")); 
      sol = utgLooseNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("utgLooseOneRaise()")); 
      sol = utgLooseOneRaise();
   }
   else if (raisers_ >= 1)
   {
      CGlobal::Instance().ap2(QString("utgLooseMoreRaise()"));
      sol = utgLooseMoreRaise();
   }

   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::utgLooseNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AJo+,KQo");
   limpRange << parseRangeList("22+,A2s+,K9s+,Q9s+,J9s+,ATo+,KJo+,T9s,98s");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::utgLooseOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,AKs,AQs,AKo,AQo");
   limpRange << parseRangeList("22+,ATs+,KTs+,QJs,JTs");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::utgLooseMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   limpRange << parseRangeList("TT+,AJs+,KQs,AKo");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------Middle Loose----------------- */
//
Solution MindFLfull::mLoose()
{
   Solution sol;
   if (raisers_ == 0)
   {
      CGlobal::Instance().ap2(QString("mLooseNoRaise()")); 
      sol = mLooseNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("mLooseOneRaise()")); 
      sol = mLooseOneRaise();
   }
   else if (raisers_ >= 1)
   {
      CGlobal::Instance().ap2(QString("mLooseMoreRaise()"));
      sol = mLooseMoreRaise();
   }

   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::mLooseNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AJo+,KQo");
   limpRange << parseRangeList("22+,A2s+,K9s+,Q9s+,J9s+,ATo+,KJo+,T9s,98s");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::mLooseOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,AKs,AQs,AKo,AQo");
   limpRange << parseRangeList("22+,ATs+,KTs+,QJs,JTs");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::mLooseMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   limpRange << parseRangeList("TT+,AJs+,KQs,AKo");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------Button, HJ Loose----------------- */
//
Solution MindFLfull::buLoose()
{
   Solution sol;
   if (raisers_ == 0)
   {
      CGlobal::Instance().ap2(QString("buLooseNoRaise()")); 
      sol = buLooseNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("buLooseOneRaise()")); 
      sol = buLooseOneRaise();
   }
   else if (raisers_ >= 1)
   {
      CGlobal::Instance().ap2(QString("buLooseMoreRaise()"));
      sol = buLooseMoreRaise();
   }

   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::buLooseNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("88+,A8s+,K9s+,QTs+,JTs,AJo+,KQo");
   limpRange << parseRangeList("22+,A2s+,K2s+,Q8s+,J7s+,ATo+,KTo+,QTo+,JTo,T9s,"
                               "98s,87s,76s,65s,54s,43s,T8s,97s,86s,75s,64s,53s");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::buLooseOneRaise()
{
   if (limpers_ + raisers_ < 4) 
   {
      CGlobal::Instance().ap1(QString("(limpers_ + raisers_ < 4) => buTightOneRaise()"));
      return buTightOneRaise();
   }
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("TT+,AJs+,AKo,KQs");
   limpRange << parseRangeList("22+,A2s+,KTs+,QTs+,JTs,T9s,98s,87s,76s,AKo,AQo");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::buLooseMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   limpRange << parseRangeList("TT+,AJs+,AKo,KQs");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------SB Loose----------------- */
//
Solution MindFLfull::sbLoose()
{
   Solution sol;
   if (raisers_ == 0)
   {
      CGlobal::Instance().ap2(QString("sbLooseNoRaise()")); 
      sol = sbLooseNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("sbLooseOneRaise()")); 
      sol = sbLooseOneRaise();
   }
   else if (raisers_ >= 1)
   {
      CGlobal::Instance().ap2(QString("sbLooseMoreRaise()"));
      sol = sbLooseMoreRaise();
   }

   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::sbLooseNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AQo+");
   limpRange << parseRangeList("22+,A2s+,K2s+,Q8s+,J7s+,ATo+,KTo+,QTo+,JTo,"
               "T9s,98s,87s,76s,65s,54s,43s,T8s,97s,86s,75s,64s,53s");
   
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
      CGlobal::Instance().ap1(QString("hole_.isSuited() == true"));
      CGlobal::Instance().ap1(QString("Solution::Call"));
      sol.setAction(Solution::Call);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::sbLooseOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("TT+,AJs+,AKo,KQs");
   limpRange << parseRangeList("22+,ATs+,KTs+,AKo,AQo");
   
   Solution sol;
   //������� � ������ ������������� �����
   sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;

   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::sbLooseMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   limpRange << parseRangeList("TT+,AJs+,KQs,AKo");
   
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

//
/* ----------BB Loose----------------- */
//
Solution MindFLfull::bbLoose()
{
   Solution sol;
   if (raisers_ == 0)
   {
      CGlobal::Instance().ap2(QString("bbLooseNoRaise()"));  
      sol = bbLooseNoRaise();
   }
   else if (raisers_ == 1)
   {
      CGlobal::Instance().ap2(QString("bbLooseOneRaise()"));
      sol = bbLooseOneRaise();
   }
   else if (raisers_ > 1)
   {
      CGlobal::Instance().ap2(QString("bbLooseMoreRaise()"));
      sol = bbLooseMoreRaise();
   }
   //������ ��� ������ ����� ���� ������� - ����� ������ ����
   //����� ������ ��� ����� � �� �����

   return sol;
}

Solution MindFLfull::bbLooseNoRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("99+,ATs+,KJs+,AQo+");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));
   Solution sol;
   if (raiseRange.contains(suited) || raiseRange.contains(nominal))
   {
      CGlobal::Instance().ap1(QString("Solution::Raise"));
      sol.setAction(Solution::Raise);
   }
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Check"));
      sol.setAction(Solution::Check);
   }
   return sol;
}

Solution MindFLfull::bbLooseOneRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("TT+,AJs+,AKo,KQs");
   limpRange << parseRangeList("22+,ATs+,JTs,QTs+,KTs+,AJo+,KQo");
   
   //������� � ������ ������������� �����
   Solution sol = pfrVpipSolution();
   if (sol.action() != Solution::Nope)
      return sol;
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
   CGlobal::Instance().ap2(QString("Limp Range: %1").arg(limpRange.join(", ")));

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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::bbLooseMoreRaise()
{
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;
   raiseRange << parseRangeList("QQ+,AKs");
   limpRange << parseRangeList("TT+,AJs+,KQs,AKo");
   
   CGlobal::Instance().ap2(QString("Raise Range: %1").arg(raiseRange.join(", ")));
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
   else
   {
      CGlobal::Instance().ap1(QString("Solution::Fold"));
      sol.setAction(Solution::Fold);
   }
   return sol;
}

Solution MindFLfull::pfrVpipSolution()
{
   //������� ��� ������ �������
   //raisers_ == 1
   QString suited = hole_.suitedName();
   QString nominal = hole_.nominalName();
   QStringList limpRange;
   QStringList raiseRange;

   CGlobal::Instance().ap2(QString("pfrVpipSolution()")); 
   //��� ������
   qreal raiserVpip = 0., raiserPfr = 0.;
   QList<Opp> beforeLimp, afterLimp, raisers;
   bool after = false;
   foreach (Opp opp, oppList_)
   {
      if (opp.action() == Opp::Raise || opp.action() == Opp::Bet)
      {
         raiserVpip = opp.vpip();
         raiserPfr = opp.pfr();
         after = true;
      }
      else if (opp.action() == Opp::Call)
      {
         if (!after)
         {
            beforeLimp << opp;
         }
         else
         {
            afterLimp << opp;
         }
      }
   }
   
   CGlobal::Instance().ap1(QString("Raiser VPIP %1").arg(raiserVpip));
   CGlobal::Instance().ap1(QString("Raiser PFR %1").arg(raiserPfr));
   
   Solution sol;
   sol.setAction(Solution::Nope);
   //������ ��������� ��� ������� ����� ������������
   //��������� ������ - ��� ������� ����� ������ �������� ��������
   //����� ����� ���������
   if (raiserPfr < PFR_PASSIVE)
   {
      CGlobal::Instance().ap1(QString("DANGER: raiserPfr < PFR_PASSIVE"));
      //88+,ATs+,KTs+,QJs,AJo+
      raiseRange.clear();
      limpRange.clear();
      //����� ����� ������ ������� � ���������� �����
      raiseRange << parseRangeList("QQ+,AKs");
      
      if (raiseRange.contains(suited) || raiseRange.contains(nominal))
      {
         CGlobal::Instance().ap1(QString("Solution::Raise"));
         sol.setAction(Solution::Raise);
      }
      else
      {
         CGlobal::Instance().ap1(QString("Solution::Fold"));
         sol.setAction(Solution::Fold);
      }
      return sol;
   }
   //��������� ������ 
   //��� ������� ����� ������ �������� ����
   raiseRange.clear();
   limpRange.clear();
   foreach (Opp opp, afterLimp)
   {
      if (opp.limp() < LIMP_PASSIVE)
      {
         CGlobal::Instance().ap1(QString("DANGER: opp.limp() < LIMP_PASSIVE"));

         raiseRange << parseRangeList("QQ+,AKs");
         limpRange << parseRangeList("JJ+,AQs+");
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
         else
         {
            CGlobal::Instance().ap1(QString("Solution::Fold"));
            sol.setAction(Solution::Fold);
         }
         return sol;
      }
   }
   

   //������ �������� ����� ��������� �� ��� ������
   //1) ��� ����� ������ � ��������
   //��� ���� �������� �� ������ ���� � ��� ��� ���� �� �������
   raiseRange.clear();
   limpRange.clear();
   if (raiserPfr > PFR_AGGRESIVE)
   {
      if (limpers_ <= 2)
      {
         //���������� ��������� ������ ������� ����� �������
         bool isLooseLimpers = true;
         foreach (Opp opp, afterLimp)
         {
            if (opp.vpip() < LOOSE)
            {
               isLooseLimpers = false;
            }
         }
         if (afterLimp.isEmpty() || isLooseLimpers)
         {
            CGlobal::Instance().ap1(QString("RELAX: raiserPfr > PFR_AGGRESIVE &&"
               " (limpers_ <= 2 && (afterLimp.isEmpty() || isLooseLimpers))"));
            //������ �������� ��������
            raiseRange << parseRangeList("JJ+,ATs+,KJs+,AQo+");
            limpRange  << parseRangeList("TT+,A9s+,KTs+,QTs+,AJo+,KQo");
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
         }
         return sol;
      }
   }
   //else if (raiserVpip > MANIAK )
   //{
   //   CGlobal::Instance().ap1(QString("Raiser is MANIAK"));
   //   raiseRange.clear();
   //   limpRange.clear();
   //   raiseRange << parseRangeList("88+,ATs+,KTs+,QJs,AJo+");
   //   limpRange  << parseRangeList("55+,A5s+,K9s+,Q9s+,J9s+,T9s,A9o+,KTo+,QTo+,JTo");

   //}
   //else if (raiserVpip > LOOSE)
   //{
   //   CGlobal::Instance().ap1(QString("Raiser is LOOSE"));
   //   raiseRange.clear();
   //   limpRange.clear();
   //   raiseRange << parseRangeList("99+,ATs+,KTs+,AQo+");
   //   limpRange  << parseRangeList("55+,A9s+,K9s+,QTs+,ATo+,KQo,QJo");
   //}
   return sol;
}

//
// FLOP
//

Solution MindFLfull::flopSolution()
{
   QStringList board = proc_->board();
   CGlobal::Instance().ap1(board.join(" "));
   //proc_->save("sshot/acad_fr/flop/" + hole_.fullName() + ".bmp");
   Solution sol;
   sol.setAction(Solution::Nope);
   return sol;
}

Solution MindFLfull::turnSolution()
{
   return Solution();
}

Solution MindFLfull::riverSolution()
{
   return Solution();
}

