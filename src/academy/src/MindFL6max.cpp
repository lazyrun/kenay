#include "MindFL6max.h"

MindFL6max::MindFL6max(CardProcessing * const proc, Session * const session)
: Mind(proc, session)
{
}

Solution MindFL6max::think()
{
   //1) ���������� ���� �����
   HoleCards hole(proc_->holeCard("first"), proc_->holeCard("second"));
   //��������� ����������
   session_->saveStats(hole.fullName());
   //����������� ����� ����� (�������� ���� ��� �������)
   //��������� ������������ ����������/��������� ��� ������� ������
   QList<Opp> oppList;
   for (int i = 1; i < 6; i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      oppList << opp;
      //�������� ���������� �� ����
      session_->stat(opp.nick().hash());
   }
   return Solution();
}


