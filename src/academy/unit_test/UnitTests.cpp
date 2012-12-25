#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "ProcAcad.h"
#include "Config.h"

namespace tut
{
   // Определение типов
   struct bot_shared_data
   {
   };

   typedef test_group<bot_shared_data, 20> tg_bot;// 20 tests
   typedef tg_bot::object testobject;

   tg_bot group_bot("ACAD");

   // Определение тестов
   template<>   template<>
   void testobject::test<1>()
   {
      QDir test_dir("test/acad");
      tensure(__FILE__, __LINE__, test_dir.exists());
      
      QFileInfoList files = test_dir.entryInfoList(QDir::Files);
      foreach (QFileInfo fi, files)
      {
         QString card = fi.baseName();
         QImage imgCard(fi.absoluteFilePath());
         QString rec = ProcAcad("map/acad.xml").cardFromImage(imgCard);
         tensure(__FILE__, __LINE__, rec == card);
      }
   }

   template<>   template<>
   void testobject::test<2>()
   {
      Settings & config = 
         ConfigGlobal<MainConfig>::Instance("map/acad.xml", "root");
      QString str =
         config.settingValue("buttons", "fold", "control", "").toString();
      tensure(__FILE__, __LINE__, str == "#E5C0B5"); 
      str = config.settingAttribute("width", "window", "");
      tensure(__FILE__, __LINE__, str == "1018"); 
      str = config.settingAttribute("height", "window", "");
      tensure(__FILE__, __LINE__, str == "695"); 
      str = config.settingAttribute("x", "buttons", "fold", "");
      tensure(__FILE__, __LINE__, str == "158"); 
      str = config.settingAttribute("y", "buttons", "check", "control", "");
      tensure(__FILE__, __LINE__, str == "659"); 
      str =
         config.settingValue("stack", "color", "").toString();
      tensure(__FILE__, __LINE__, str == "#00FF00"); 
   }

   template<>   template<>
   void testobject::test<3>()
   {
      ProcAcad proc("map/acad.xml");
      {
         QImage imgTable("sshot/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "2s");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "As");
      }
      {
         QImage imgTable("sshot/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "8d");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "9h");
      }
      {
         QImage imgTable("sshot/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "Th");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "Ac");
      }
      {
         QImage imgTable("sshot/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "As");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "6s");
      }
      {
         QImage imgTable("sshot/acad_5.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "7s");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "8s");
      }
      {
         QImage imgTable("sshot/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "8h");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "4c");
      }
      {
         QImage imgTable("sshot/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "5s");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "Ah");
      }
   }

   template<>   template<>
   void testobject::test<4>()
   {
      ProcAcad proc("map/acad.xml");
      {
         QImage imgTable("sshot/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.35));
      }
      {
         QImage imgTable("sshot/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.25));
      }
      {
         QImage imgTable("sshot/acad_5.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.25));
      }
      {
         QImage imgTable("sshot/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.95));
      }
      {
         QImage imgTable("sshot/acad_9.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 1.85));
      }
      {
         QImage imgTable("sshot/acad_10.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 2.45));
      }
      {
         QImage imgTable("sshot/acad_11.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 1.0));
      }
      {
         QImage imgTable("sshot/acad_12.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.65));
      }
      {
         QImage imgTable("sshot/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.75));
      }
   }
   
   template<>   template<>
   void testobject::test<5>()
   {
      ProcAcad proc("map/acad.xml");
      {
         QImage imgTable("sshot/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.95));
      }
      {
         QImage imgTable("sshot/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 10.0));
      }
      {
         QImage imgTable("sshot/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.70));
      }
      {
         QImage imgTable("sshot/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.60));
      }
      {
         QImage imgTable("sshot/acad_5.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.55));
      }
      {
         QImage imgTable("sshot/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.55));
      }
      {
         QImage imgTable("sshot/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.40));
      }
      {
         QImage imgTable("sshot/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.60));
      }
      {
         QImage imgTable("sshot/acad_9.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.10));
      }
      {
         QImage imgTable("sshot/acad_10.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.90));
      }
      {
         QImage imgTable("sshot/acad_11.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.30));
      }
      {
         QImage imgTable("sshot/acad_12.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 4.95));
      }
      {
         QImage imgTable("sshot/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.0));
      }
      {
         QImage imgTable("sshot/acad_14.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.20));
      }
   }
}
