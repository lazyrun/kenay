#include "MindFLfull.h"

MindFLfull::MindFLfull(CardProcessing * const proc, Session * const session)
: Mind(proc, session)
{
}

Solution MindFLfull::preflopSolution()
{
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

   //���������� �������� �����
   int limpers = 0;
   int raisers = 0;
   foreach (Opp opp, oppList_)
   {
      //��� � ������� � ��� �� �� ������
      if (opp.hasCards() && opp.action() != Opp::Nope)
      {
         if (opp.action() == Opp::Call)
            limpers++;
         else if (opp.action() == Opp::Raise)
            raisers++;
      }
   }
   //������������ ������������ ��� �������� ����
   bool useTight = false;
   //������ ����
   if (preflopPos_ == HighJack || preflopPos_ == Button)
   {
      if ((limpers + raisers <= 3) && (limpers + raisers > 0) && preflopPos_ == Button)
      {
         //1, 2 ������� � 1 ������
         useTight = true;
      }
      else if ((limpers + raisers <= 2) && (limpers + raisers > 0) && preflopPos_ == HighJack)
      {
         useTight = true;
      }
      else if (limpers + raisers == 0)
      {
         //������ �������
         useTight = false;
      }
      else//������ ���� ���
      {
         //� ����������� �� ���������� �����
         if (tightPercent > 50.)
         {
            useTight = true;
         }
      }
   }
   else//�����-�� ������ ������� UTG, Middle, sb, bb
   {
      //� ����������� �� ���������� �����
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
      //���������� ����� �������
      QList<Opp> liveOpps;
      foreach (Opp opp, oppList_)
      {
         if (opp.isInGame())
         {
            liveOpps << opp;
         }
      }
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
      Q_ASSERT_X(dpos != -1, "preflopPosition", "Dealer position isn't found!");
      
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

Solution MindFLfull::tightPreflop()
{
   return Solution();
}

Solution MindFLfull::loosePreflop()
{
   return Solution();
}

