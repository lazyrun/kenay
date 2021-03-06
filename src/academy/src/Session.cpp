#include "Session.h"
#include "GlobVars.h"

Session::Session(CardProcessing * const proc, int cnt): proc_(proc)
{
   cnt_ = cnt;
   preflopRound_ = 0;
}

void Session::saveStats(const QString & session)
{
   CardProcessing::Street street = proc_->street();
   if (session != sessionID_ && !sessionID_.isEmpty())
   {
      CGlobal::Instance().ap1(QString("<<< END SESSION: %1 >>>\n").arg(sessionID_));
      CGlobal::Instance().ap1(QString("<<< BEGIN SESSION: %1 >>>").arg(session));
      //����� ������
      //��������� � �� ���������� ������
      saveToDB();
      //������ ������ ����� ������
      history_.clear();
      //��������� ������� 1-�� ������ �� ��������
      preflopRound_ = 0;
   }
   else if (sessionID_.isEmpty())
   {
      preflopRound_ = 0;
   }
   else
   {
      if (street == CardProcessing::Preflop)
         preflopRound_ = 1;
   }
   
   sessionID_ = session;

   QMap<int, ActionList> & oppHist = history_[street];

   opps_.clear();
   for (int i = 1; i < cnt_; i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      Opp::Action act = opp.action();
      oppHist[i].append(act);
      opps_.insert(i, opp);
   }
}

void Session::saveToDB()
{
   if (history_.isEmpty())
      return;

   const QMap<int, ActionList> & oppActions = history_[CardProcessing::Preflop];
   const QMap<int, Opp> & opps = opps_;
   QSqlDatabase db = QSqlDatabase::database("STAT_DB");
   db.transaction();
   foreach (int oppId, opps.keys())
   {
      const Opp & opp = opps.value(oppId);
      const ActionList & actions = oppActions.value(oppId);
      QString nick = opp.nick().hash();
      
      QSqlQuery sel_query(db);
      sel_query.prepare("SELECT * FROM PREFLOP WHERE NICK=:nick");
      sel_query.bindValue(":nick", nick);
      int cnt = 0, fold = 0, pfr = 0, vpip = 0, limp = 0, id = -1;
      if (sel_query.exec())
      {
         if (sel_query.first())
         {
            QSqlRecord rec = sel_query.record();
            id = rec.value("ID").toInt();
            cnt = rec.value("CNT").toInt();
            fold = rec.value("FOLD").toInt();
            pfr = rec.value("PFR").toInt();
            vpip = rec.value("VPIP").toInt();
            limp = rec.value("LIMP").toInt();
            sel_query.finish();
            sel_query.clear();
         }
         else
         {
            //������ ���
            //�������
            cnt = fold = pfr = vpip = limp = 0;
            QSqlQuery ins_query(db);
            ins_query.prepare("INSERT INTO PREFLOP (NICK, CNT, FOLD, VPIP, PFR, LIMP) "
               "VALUES(:nick, 0, 0, 0, 0, 0)");
            ins_query.bindValue(":nick", nick);
            ins_query.exec();
            id = ins_query.lastInsertId().toInt();
         }
      }

      if (actions.count() > 0)
      {
         if (actions.at(0) == Opp::Fold)
         {
            cnt++;
            fold++;
         }
         else if (actions.at(0) == Opp::Raise)
         {
            cnt++;
            pfr++;
            vpip++;
         }
         else if (actions.at(0) == Opp::Call)
         {
            cnt++;
            limp++;
            vpip++;
         }
         else if (actions.at(0) == Opp::Nope       || 
                  actions.at(0) == Opp::SmallBlind)
                  //actions.at(0) == Opp::BigBlind) 
                  //�� �� ���������, �.�. ����� �� ������ ����� �� ���� ��� ������
         {
            //����� �������� ������ �� ���� �����
            Opp currentOpp = proc_->opp(QString::number(oppId));
            //������������ � ����� ����� �������� ������
            qreal currentStack = currentOpp.stack();
            //����� ��� �� ������, �� ������� ��� ����������
            if (currentStack <= 0.001 && actions.at(0) == Opp::Nope)
            {
               continue;
            }
            currentStack += currentOpp.bet();

            if (currentOpp.nick() == opp.nick())
            {
               //����������, ��� �� ��� ����� �� ��� �� �����
               if (qFuzzyCompare(opp.stack(), currentStack))
               {
                  //���� �� ��������� - ���� ������� 
                  cnt++;
                  fold++;
               }
               else
               {
                  //������������ � �������
                  //�� ��������� ����� ������ �������, ������ ��� ������
                  cnt++;
                  vpip++;
               }

            }
         }

         //�������� ����� ��������
         QSqlQuery upd_query(db);
         upd_query.prepare("UPDATE PREFLOP SET CNT=:cnt, "
            "FOLD=:fold, VPIP=:vpip, PFR=:pfr, LIMP=:limp "
            "WHERE ID=:id");
         upd_query.bindValue(":id", id);
         upd_query.bindValue(":cnt", cnt);
         upd_query.bindValue(":fold", fold);
         upd_query.bindValue(":vpip", vpip);
         upd_query.bindValue(":pfr", pfr);
         upd_query.bindValue(":limp", limp);

         upd_query.exec();
      }
   }
   db.commit();
}

void Session::stat(Opp & opp)
{
   QString nick = opp.nick().hash();
   QString sel_sql = "SELECT CNT, VPIP, PFR, FOLD, LIMP FROM PREFLOP WHERE NICK=:nick";
   QSqlDatabase db = QSqlDatabase::database("STAT_DB");
   QSqlQuery sel_query(db);
   sel_query.prepare(sel_sql);
   sel_query.bindValue(":nick", nick);
   int cnt = 0, fold = 0, pfr = 0, vpip = 0, limp = 0;
   if (sel_query.exec())
   {
      if (sel_query.first())
      {
         QSqlRecord rec = sel_query.record();
         cnt  = rec.value("CNT").toInt();
         vpip = rec.value("VPIP").toInt();
         fold = rec.value("FOLD").toInt();
         pfr  = rec.value("PFR").toInt();
         limp = rec.value("LIMP").toInt();
         sel_query.finish();
         sel_query.clear();
      }
      else
      {
         //������ ���
         return;
      }
      //���������� ����� ����� � ������-�� ��������
      if (cnt > 20.)
      {
         //�������������� � ���������� ���������
         opp.setVpip(qreal(vpip) * 100. / qreal(cnt));
         opp.setFold(qreal(fold) * 100. / qreal(cnt));

         int vp_cnt = pfr + limp;
         qreal pfr_prc =  qreal(pfr) * 100. / qreal(vp_cnt);
         qreal lmp_prc =  qreal(limp) * 100. / qreal(vp_cnt);
         
         qreal pfr_cnt = vpip * pfr_prc / 100.;
         qreal lmp_cnt = vpip * lmp_prc / 100.;

         opp.setPfr(qreal(pfr_cnt) * 100. / qreal(cnt));
         opp.setLimp(qreal(lmp_cnt) * 100. / qreal(cnt));
      }
   }
}

