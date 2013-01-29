#include "MindFLfull.h"

MindFLfull::MindFLfull(CardProcessing * const proc, Session * const session)
: Mind(proc, session)
{
}

Solution MindFLfull::think()
{
   //1) ���������� ���� �����
   HoleCards hole(proc_->holeCard("first"), proc_->holeCard("second"));
   //��������� ����������
   session_->saveStats(hole.fullName());
   //����������� ����� ����� (�������� ���� ��� �������)
   //��������� ������������ ����������/��������� ��� ������� ������
   QList<Opp> oppList;
   int tight = 0;
   for (int i = 1; i < 10; i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      //�������� ���������� �� ����
      session_->stat(opp);
      if (opp.vpip() > 1. && opp.vpip() < 20. )
         tight++;
      oppList << opp;
   }
   if (tight > 4)
   {
      //���� ��������
   }
   else
   {
      //���� �������
   }
   return Solution();
}


