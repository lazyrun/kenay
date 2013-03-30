#include <stdio.h>

//#include "tut.h"
#include "TstUtils.h"
#include "MindFL6max.h"
#include "MindFLfull.h"
#include "HoleCards.h"
#include "DBManager.h"
#include "Session.h"
#include "ProcAcad.h"

namespace tut
{
   // Определение типов
   struct logic_shared_data
   {
   };

   typedef test_group<logic_shared_data, 20> tg_logic;// 20 tests
   typedef tg_logic::object testlogic;

   tg_logic group_logic("LOGIC");
   
   // Определение тестов
   template<>   template<>
   void testlogic::test<1>()
   {
      //проверка HoleCards
      {
      HoleCards hole("Qs", "Ad");
      tensure(__FILE__, __LINE__, hole.fullName() == "AdQs");
      tensure(__FILE__, __LINE__, hole.suitedName() == "AQo");
      tensure(__FILE__, __LINE__, !hole.isSuited());
      }
      {
      HoleCards hole("Kd", "4d");
      tensure(__FILE__, __LINE__, hole.fullName() == "Kd4d");
      tensure(__FILE__, __LINE__, hole.suitedName() == "K4s");
      tensure(__FILE__, __LINE__, hole.isSuited());
      }
      {
      HoleCards hole("7c", "Qs");
      tensure(__FILE__, __LINE__, hole.fullName() == "Qs7c");
      tensure(__FILE__, __LINE__, hole.suitedName() == "Q7o");
      tensure(__FILE__, __LINE__, !hole.isSuited());
      }
      {
      HoleCards hole("Tc", "Ts");
      tensure(__FILE__, __LINE__, hole.fullName() == "TcTs");
      tensure(__FILE__, __LINE__, hole.suitedName() == "TTo");
      tensure(__FILE__, __LINE__, !hole.isSuited());
      }
      {
      HoleCards hole("Qs", "As");
      tensure(__FILE__, __LINE__, hole.fullName() == "AsQs");
      }
   }
   
   template<>   template<>
   void testlogic::test<2>()
   {
      //проверка сохранения статистики
      DBManager db("stat/stat_acad6max.db");
      tensure(__FILE__, __LINE__, db.isGood());
      //очистить таблицу
      tensure(__FILE__, __LINE__, db.clearTable("PREFLOP"));

      ProcAcad proc("map/acad_6max.xml");
      Session ses(&proc, 6);

      QDir test_dir("sshot/acad_6max");
      tensure(__FILE__, __LINE__, test_dir.exists());
      
      QFileInfoList files = test_dir.entryInfoList(QDir::Files);
      foreach (QFileInfo fi, files)
      {
         QString s = fi.absoluteFilePath();
         QImage imgTable(fi.absoluteFilePath());
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         QString sid = proc.holeCard("first") + proc.holeCard("second");
         ses.saveStats(sid);
      }  

      ses.saveStats("save");
   }
   
   template<>   template<>
   void testlogic::test<3>()
   {
      ProcAcad proc("map/acad_6max.xml");
      Session ses(&proc, 6);
      MindFL6max mind(&proc, &ses);
      {
      QImage imgTable("sshot/acad_6max/acad_1.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      if (proc.hasFold())
      {
         Solution sol = mind.think();
      }
      }
   }

   template<>   template<>
   void testlogic::test<4>()
   {
      DBManager db("stat/stat_acad6max.db");
      tensure(__FILE__, __LINE__, db.isGood());

      ProcAcad proc("map/acad_6max.xml");
      Session ses(&proc, 6);
      {
      QImage imgTable("sshot/acad_6max/acad_1.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      
      Opp opp = proc.opp("1");
      ses.stat(opp);
      
      }
   }
   template<>   template<>
   void testlogic::test<5>()
   {
      DBManager db("stat/stat_acadfr.db");
      tensure(__FILE__, __LINE__, db.isGood());

      ProcAcad proc("map/acad_fr.xml");
      Session ses(&proc, 10);
      {
      QImage imgTable("sshot/acad_fr/acad_1.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      
      for (int i = 1; i < 10; i++)
      {
         Opp opp = proc.opp(QString::number(i));
         ses.stat(opp);
         qDebug() << opp.vpip() << "/" << opp.pfr() << "/" << opp.limp();
      }
      MindFLfull mind(&proc, &ses);
      Solution sol = mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::HighJack);
      tensure(__FILE__, __LINE__, p.limpers() == 2);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_2.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      MindFLfullPrivate p(&mind);
      mind.think();
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Middle);
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_3.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Middle);
      tensure(__FILE__, __LINE__, p.limpers() == 0);
      tensure(__FILE__, __LINE__, p.raisers() == 0);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_4.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Middle);
      tensure(__FILE__, __LINE__, p.limpers() == 0);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_5.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::UTG);
      tensure(__FILE__, __LINE__, p.limpers() == 2);
      tensure(__FILE__, __LINE__, p.raisers() == 0);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_6.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::UTG);
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 0);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_7.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::UTG);
      tensure(__FILE__, __LINE__, p.limpers() == 0);
      tensure(__FILE__, __LINE__, p.raisers() == 0);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_8.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::BigBlind);
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_9.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::SmallBlind);
      tensure(__FILE__, __LINE__, p.limpers() == 2);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_10.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Button);
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 0);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_11.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Middle);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_12.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Middle);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_13.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::Middle);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_14.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::HighJack);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_15.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::UTG);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_16.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::SmallBlind);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_17.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      mind.think();
      MindFLfullPrivate p(&mind);
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::UTG);
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }

   }
   
   template<>   template<>
   void testlogic::test<6>()
   {
      ProcAcad proc("map/acad_fr.xml");
      Session ses(&proc, 10);
      {
      MindFLfull mind(&proc, &ses);
      MindFLfullPrivate p(&mind);
      QString range = p.parseRange("77+").join(", ");
      tensure(__FILE__, __LINE__, range == "77, 88, 99, TT, JJ, QQ, KK, AA");

      range = p.parseRange("ATs+").join(", ");
      tensure(__FILE__, __LINE__, range == "ATs, AJs, AQs, AKs");

      range = p.parseRange("KJs+").join(", ");
      tensure(__FILE__, __LINE__, range == "KJs, KQs");

      range = p.parseRange("AQo+").join(", ");
      tensure(__FILE__, __LINE__, range == "AQo, AKo");

      range = p.parseRange("JTs+").join(", ");
      tensure(__FILE__, __LINE__, range == "JTs");
      
      range = p.parseRange("QTs+").join(", ");
      tensure(__FILE__, __LINE__, range == "QTs, QJs");

      range = p.parseRange("KTs+").join(", ");
      tensure(__FILE__, __LINE__, range == "KTs, KJs, KQs");

      range = p.parseRange("ATs+").join(", ");
      tensure(__FILE__, __LINE__, range == "ATs, AJs, AQs, AKs");

      range = p.parseRange("AJo+").join(", ");
      tensure(__FILE__, __LINE__, range == "AJo, AQo, AKo");

      range = p.parseRange("A2s+").join(", ");
      tensure(__FILE__, __LINE__, range == "A2s, A3s, A4s, A5s, A6s, A7s, A8s, A9s, ATs, AJs, AQs, AKs");

      range = p.parseRange("K9s+").join(", ");
      tensure(__FILE__, __LINE__, range == "K9s, KTs, KJs, KQs");

      range = p.parseRange("Q9s+").join(", ");
      tensure(__FILE__, __LINE__, range == "Q9s, QTs, QJs");

      range = p.parseRange("J9s+").join(", ");
      tensure(__FILE__, __LINE__, range == "J9s, JTs");

      range = p.parseRange("J9s+").join(", ");
      tensure(__FILE__, __LINE__, range == "J9s, JTs");

      }
   }
   
   template<>   template<>
   void testlogic::test<7>()
   {
      DBManager db("stat/stat_acadfr.db");
      tensure(__FILE__, __LINE__, db.isGood());

      ProcAcad proc("map/acad_fr.xml");
      Session ses(&proc, 10);
      {
      QImage imgTable("sshot/acad_fr/acad_19.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      MindFLfullPrivate p(&mind);
      mind.think();
      tensure(__FILE__, __LINE__, p.preflopPos() == Mind::SmallBlind);
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 0);
      }
      {
      QImage imgTable("sshot/acad_fr/acad_20.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      MindFLfullPrivate p(&mind);
      mind.think();
      tensure(__FILE__, __LINE__, p.limpers() == 1);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      }
   }

   template<>   template<>
   void testlogic::test<8>()
   {
      DBManager db("stat/stat_acadfr.db");
      tensure(__FILE__, __LINE__, db.isGood());

      ProcAcad proc("map/acad_fr.xml");
      Session ses(&proc, 10);
      {
      QImage imgTable("sshot/acad_fr/flop/3c3s.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      MindFLfull mind(&proc, &ses);
      MindFLfullPrivate p(&mind);
      mind.think();
      tensure(__FILE__, __LINE__, p.street() == CardProcessing::Flop);
      tensure(__FILE__, __LINE__, p.limpers() == 0);
      tensure(__FILE__, __LINE__, p.raisers() == 1);
      tensure(__FILE__, __LINE__, p.board().join("") == "8dAd7h");

      imgTable.load("sshot/acad_fr/flop/3d3c.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      mind.think();
      tensure(__FILE__, __LINE__, p.board().join("") == "2s4sAs");

      imgTable.load("sshot/acad_fr/flop/4c4s.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      mind.think();
      tensure(__FILE__, __LINE__, p.board().join("") == "Jd7dJs");

      imgTable.load("sshot/acad_fr/flop/5d5s.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      mind.think();
      tensure(__FILE__, __LINE__, p.board().join("") == "TsThKs");

      imgTable.load("sshot/acad_fr/flop/5s3s.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      mind.think();
      tensure(__FILE__, __LINE__, p.board().join("") == "6cKhJc");

      }
   }

   template<>   template<>
   void testlogic::test<9>()
   {
      MindFLfull mind(0, 0);
      MindFLfullPrivate p(&mind);
      {
      HoleCards hole("Qs", "Qd");
      QStringList board;
      board << "Qc" << "Qd" << "Ts";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Care);
      }
      {
      HoleCards hole("Qs", "Ts");
      QStringList board;
      board << "3s" << "As" << "6s";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Flash);
      }
      {
      HoleCards hole("Qs", "Ts");
      QStringList board;
      board << "Qd" << "Td" << "Qc";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::FullHouse);
      }
      {
      HoleCards hole("Qs", "Ts");
      QStringList board;
      board << "Ad" << "Kd" << "Jc";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Straight);
      }
      {
      HoleCards hole("Qs", "Ts");
      QStringList board;
      board << "Qd" << "Kd" << "Jc";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb != Comb::Straight);
      }
      {
      HoleCards hole("Qs", "Ts");
      QStringList board;
      board << "Jd" << "Kd";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb != Comb::Straight);
      }
      {
      HoleCards hole("8s", "6s");
      QStringList board;
      board << "7d" << "9d" << "Tc";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Straight);
      }
      {
      HoleCards hole("8s", "6s");
      QStringList board;
      board << "7d" << "9d" << "2c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb != Comb::Straight);
      }
      {
      HoleCards hole("As", "Ac");
      QStringList board;
      board << "Ad" << "Kd" << "Qc";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb != Comb::Straight);
      }
      {
      HoleCards hole("As", "2c");
      QStringList board;
      board << "5d" << "4d" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Straight);
      }
      {
      HoleCards hole("As", "2s");
      QStringList board;
      board << "5s" << "4s" << "3s";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::StraightFlash);
      }
      {
      HoleCards hole("As", "7s");
      QStringList board;
      board << "5s" << "4s" << "3s";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb != Comb::StraightFlash);
      }
      {
      HoleCards hole("6s", "7s");
      QStringList board;
      board << "5s" << "4s" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb != Comb::StraightFlash);
      }
      {
      HoleCards hole("6s", "7s");
      QStringList board;
      board << "6s" << "6s" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::ThreeOfKind);
      }
      {
      HoleCards hole("Js", "Jc");
      QStringList board;
      board << "5s" << "Jd" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::ThreeOfKind);
      }
      {
      HoleCards hole("8s", "Ts");
      QStringList board;
      board << "Ts" << "8s" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::TwoPair);
      }
      {
      HoleCards hole("8s", "8d");
      QStringList board;
      board << "5s" << "5d" << "9c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::TwoPair);
      }
      {
      HoleCards hole("As", "Ad");
      QStringList board;
      board << "5s" << "4s" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Pair);
      }
      {
      HoleCards hole("As", "Qd");
      QStringList board;
      board << "Ad" << "Ts" << "Kc";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Pair);
      }
      {
      HoleCards hole("As", "Qd");
      QStringList board;
      board << "Kd" << "Ks" << "3c";
      p.setHole(hole);
      p.setBoard(board);
      Comb::Combs comb = p.comb();
      tensure(__FILE__, __LINE__, comb == Comb::Pair);
      }
   }
}

//вычисление аутов
//анализ стола
/*
   Дано:
   AsQs
   Флоп
   Ts5d2d

   Посчитать:
   Текущую комбинацию.
   Возможность собрать комбинацию на одной, двух улицах.
   По убыванию:
   Каре -      0 (нет возможности)
   Фулл-хаус - 0
   Флеш -      2 (на 2х улицах)
   Стрит -     2
   Сет -       2
   Допер -     2
   Две пары -  2
   Топ-Пару -  1 (на 1й улице)
*/
