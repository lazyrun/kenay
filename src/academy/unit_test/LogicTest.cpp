#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "Mind.h"
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
      //Ts5d2d
      //QStringList board;
      //board.append("Ts");
      //board.append("5d");
      //board.append("2d");
      ////
      //Mind mind;
      //mind.setHole("Qs", "As");
      //mind.setBoard(board);
      //mind.currentCombs();
      
      }
   }
   
   template<>   template<>
   void testlogic::test<2>()
   {
      DBManager db;
      tensure(__FILE__, __LINE__, db.isGood());

      ProcAcad proc("map/acad.xml");
      QImage imgTable("sshot/acad_2.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());
      proc.setImage(imgTable);
      QString sid = proc.holeCard("first") + proc.holeCard("second");
      Session ses(&proc);
      ses.saveStats(sid);
      ses.saveStats("save");
   }
}

