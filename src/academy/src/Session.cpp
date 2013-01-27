#include "Session.h"

Session::Session(CardProcessing * proc): proc_(proc)
{
}

void Session::saveStats(const QString & session)
{
   if (session != sessionID_ && !sessionID_.isEmpty())
   {
      //новая сессия
      //сохранить в БД предыдущую сессию
      saveToDB();
      //начать запись новой сессии
      history_.clear();
   }
   sessionID_ = session;
   CardProcessing::Street street = proc_->street();
   QMap<int, ActionList> & oppHist = history_[street];

   //nopeMap_.clear();
   opps_.clear();
   for (int i = 1; i < 6; i++)
   {
      Opp opp = proc_->opp(QString::number(i));
      Opp::Action act = opp.action();
      //if (street == CardProcessing::Preflop && 
      //   (act == Opp::Nope || act == Opp::SmallBlind || act == Opp::BigBlind))
      //{
      //   nopeMap_.insert(opp.nick().hash(), opp);
      //}
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
            //записи нет
            //создать
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
                  //ББ не проверяем, т.к. никак не понять чекал ли юзер или рейзил
         {
            //взять текущего игрока на этом месте
            Opp currentOpp = proc_->opp(QString::number(oppId));
            //приплюсовать к стеку сумму нынешней ставки
            qreal currentStack = currentOpp.stack();
            //if (currentOpp.action() == Opp::SmallBlind ||
            //    currentOpp.action() == Opp::BigBlind)
            {
               currentStack += currentOpp.bet();
            }
            if (currentOpp.nick() == opp.nick())
            {
               //убеждаемся, что на том месте всё тот же игрок
               if (qFuzzyCompare(opp.stack(), currentStack))
               {
                  //стек не изменился - юзер сфолдил 
                  cnt++;
                  fold++;
               }
               else
               {
                  //поучаствовал в раздаче
                  //но неизвстно каким именно образом, лимпом или рейзом
                  cnt++;
                  vpip++;
               }

            }
         }

         //сохраним новые значения
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
