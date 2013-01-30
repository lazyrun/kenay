#include "Mind.h"

Mind::Mind(CardProcessing * const proc, Session * const session) 
: proc_(proc), session_(session)
{
}

Solution Mind::think()
{
   //1) ���������� ���� �����
   HoleCards hole(proc_->holeCard("first"), proc_->holeCard("second"));
   hole_ = hole;
   
   //��������� ����������
   session_->saveStats(hole.fullName());

   for (int i = 1; i < session_->tableFormat(); i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      //�������� ���������� �� ����
      session_->stat(opp);
      oppList_ << opp;
   }
   CardProcessing::Street street = proc_->street();
   switch (street)
   {
      case CardProcessing::Preflop:
      {
         return preflopSolution();
      }
      case CardProcessing::Flop:
      {
         return flopSolution();
      }
      case CardProcessing::Turn:
      {
         return turnSolution();
      }
      case CardProcessing::River:
      {
         return riverSolution();
      }
   }
   return Solution();
}

