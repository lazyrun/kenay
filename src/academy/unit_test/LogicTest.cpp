#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "MindFL6max.h"
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
      {HoleCards hole("Qs", "Ad");
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
      Session ses(&proc);

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
      Session ses(&proc);
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
      Session ses(&proc);
      {
      QImage imgTable("sshot/acad_6max/acad_1.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      
      Opp opp = proc.opp("1");
      ses.stat(opp);
      
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
