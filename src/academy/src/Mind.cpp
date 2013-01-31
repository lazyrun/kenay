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

QStringList Mind::parseRange(const QString & range)
{
   // "Ax", "Axs", "Axo" - ����� ����
   // "22+", "77+" - ����
   // "AT+", "23+" - �� ������ ����� �� �����
   // "AK", "AKs", "AKo" - ������ �����
   // "A9s+" - ����������� �� 9 �� �����
   QStringList cards;
   cards << "" << "" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" 
         << "T" << "J" << "Q" << "K" << "A";

   enum Nominal
   {
      T = 10,
      J = 11, 
      Q = 12,
      K = 13,
      A = 14
   };

   QStringList lst;
   QString left = range.left(1);
   QString right = range.mid(1, 1);
   if (range.contains("x", Qt::CaseInsensitive))
   {
      for (int i = 2; i <= A; i++)
      {
         QString right = cards.at(i);
         QString card = left + cards.at(i);
         if (right == left)
         {
            lst << card;
            continue;
         }
         if (range.contains("o"))
         {
            lst << card + "o";
         }
         else if (range.contains("s"))
         {
            lst << card + "s";
         }
         else
         {
            lst << card + "o" << card + "s";
         }
      }
   }
   else if (range.contains("+", Qt::CaseInsensitive))
   {
      //����
      if (left == right)
      {
         if (cards.contains(left))
         {
            int nom = cards.indexOf(left);
            for (int i = nom; i <= A; i++)
            {
               lst << cards.at(i) + cards.at(i);
            }
         }
      }
      else
      {
         //������
         // "AT+", "23+" - �� ������ ����� �� �����
         if (cards.contains(right))
         {
            int nom = cards.indexOf(right);
            for (int i = nom; i <= A; i++)
            {
               QString right = cards.at(i);
               QString card = left + cards.at(i);
               if (right == left)
               {
                  break;
               }

               if (range.contains("o"))
               {
                  lst << card + "o";
               }
               else if (range.contains("s"))
               {
                  lst << card + "s";
               }
               else
               {
                  lst << card + "o" << card + "s";
               }
            }
         }
      }
   }
   else
   {
      if (left != right)
      {
         if (!range.contains("o") && !range.contains("s"))
         {
            lst << range + "o" << range + "s";
         }
         else
         {
            lst << range;
         }
      }
      else
      {
         lst << range;
      }
   }

   return lst;
}
