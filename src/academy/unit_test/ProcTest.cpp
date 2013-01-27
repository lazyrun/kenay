#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "ProcAcad.h"
#include "Config.h"

namespace tut
{
   // Определение типов
   struct proc_shared_data
   {
   };

   typedef test_group<proc_shared_data, 20> tg_proc;// 20 tests
   typedef tg_proc::object testproc;

   tg_proc group_proc("IMAGEPROC");

   // Определение тестов
   template<>   template<>
   void testproc::test<1>()
   {
      QDir test_dir("test/acad");
      tensure(__FILE__, __LINE__, test_dir.exists());
      
      QFileInfoList files = test_dir.entryInfoList(QDir::Files);
      foreach (QFileInfo fi, files)
      {
         QString card = fi.baseName();
         QImage imgCard(fi.absoluteFilePath());
         QString rec = ProcAcad("map/acad_6max.xml").cardFromImage(imgCard);
         tensure(__FILE__, __LINE__, rec == card);
      }
   }

   template<>   template<>
   void testproc::test<2>()
   {
      Settings & config = 
         ConfigGlobal<MainConfig>::Instance("map/acad_6max.xml", "root");
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
         config.settingValue("stack", "").toString();
      tensure(__FILE__, __LINE__, str == "#00FF00"); 
      
      str = config.settingAttribute("y", "buttons", "check", "");
      tensure(__FILE__, __LINE__, str == "655"); 

   }

   template<>   template<>
   void testproc::test<3>()
   {
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "2s");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "As");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "8d");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "9h");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "Th");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "Ac");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "As");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "6s");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_5.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "7s");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "8s");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "8h");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "4c");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "5s");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "Ah");
         tensure(__FILE__, __LINE__, proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_14.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "Ts");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "Qh");
         tensure(__FILE__, __LINE__, !proc.isDealer());
      }

   }

   template<>   template<>
   void testproc::test<4>()
   {
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.35));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.25));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_5.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.15));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.25));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.95));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_9.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 1.85));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_10.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 2.45));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_11.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 1.0));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_12.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.65));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.75));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_17.png");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.43));
      }

   }
   
   template<>   template<>
   void testproc::test<5>()
   {
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.95));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 10.0));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.70));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.60));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_5.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.55));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.55));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.40));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.60));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_9.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.10));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_10.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.90));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_11.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.30));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_12.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 4.95));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 8.0));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_14.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 9.20));
      }
   }

   template<>   template<>
   void testproc::test<6>()
   {
      //протестируем стеки оппонентов
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         Opp opp6 = proc.opp("645");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.stack(), 9.85));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.stack(), 9.35));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.stack(), 9));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.stack(), 10.95));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.stack(), 11.75));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp6.stack(), 0));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_12.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.stack(), 13.10));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.stack(), 11.20));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.stack(), 10.10));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.stack(), 10.85));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.stack(), 9.15));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_16.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.stack(), 0.));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.stack(), 9.15));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.stack(), 9.50));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.stack(), 8.40));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.stack(), 12.90));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_17.png");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.stack(), 0.));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.stack(), 9.22));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.stack(), 12.23));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.stack(), 15.87));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.stack(), 18.15));
      }
   }

   template<>   template<>
   void testproc::test<7>()
   {
      ProcAcad proc("map/acad_6max.xml");
      {
         //получить хэши-ников
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         OppNick nick = proc.opp("1").nick();

         QImage imgTable1("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();

         tensure(__FILE__, __LINE__, nick == nick1);

         QImage imgTable2("sshot/acad_6max/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("1").nick();

         tensure(__FILE__, __LINE__, nick == nick2);

         QImage imgTable3("sshot/acad_6max/acad_15.bmp");
         tensure(__FILE__, __LINE__, !imgTable3.isNull());
         proc.setImage(imgTable3);
         OppNick nick3 = proc.opp("4").nick();

         tensure(__FILE__, __LINE__, nick == nick3);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();

         QImage imgTable2("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("5").nick();

         tensure(__FILE__, __LINE__, nick1 == nick2);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();

         QImage imgTable2("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("5").nick();

         tensure(__FILE__, __LINE__, nick1 == nick2);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("2").nick();

         QImage imgTable2("sshot/acad_6max/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("5").nick();

         tensure(__FILE__, __LINE__, nick1 == nick2);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("2").nick();
         
         //поиск позиции этого персонажа на второй картинке
         QImage imgTable2("sshot/acad_6max/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         int pos = 0;
         for (int i = 1; i < 6; i++)
         {
            OppNick nick2 = proc.opp(QString::number(i)).nick();
            if (nick1 == nick2)
            {
               pos = i;
               break;
            }
         }

         tensure(__FILE__, __LINE__, pos == 5);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("3").nick();
         
         //поиск позиции этого персонажа на второй картинке
         QImage imgTable2("sshot/acad_6max/acad_14.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         int pos = 0;
         for (int i = 1; i < 6; i++)
         {
            OppNick nick2 = proc.opp(QString::number(i)).nick();
            if (nick1 == nick2)
            {
               pos = i;
               break;
            }
         }

         tensure(__FILE__, __LINE__, pos == 4);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_14.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();
         
         //поиск позиции этого персонажа на второй картинке
         QImage imgTable2("sshot/acad_6max/acad_15.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         int pos = 0;
         for (int i = 1; i < 6; i++)
         {
            OppNick nick2 = proc.opp(QString::number(i)).nick();
            if (nick1 == nick2)
            {
               pos = i;
               break;
            }
         }

         tensure(__FILE__, __LINE__, pos == 2);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("3").nick();
         
         //поиск позиции этого персонажа на второй картинке
         QImage imgTable2("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         int pos = 0;
         for (int i = 1; i < 6; i++)
         {
            OppNick nick2 = proc.opp(QString::number(i)).nick();
            if (nick1 == nick2)
            {
               pos = i;
               break;
            }
         }

         tensure(__FILE__, __LINE__, pos == 5);
      }
      {
         QImage imgTable1("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("5").nick();
         
         //поиск позиции этого персонажа на второй картинке
         QImage imgTable2("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         int pos = 0;
         for (int i = 1; i < 6; i++)
         {
            OppNick nick2 = proc.opp(QString::number(i)).nick();
            if (nick1 == nick2)
            {
               pos = i;
               break;
            }
         }

         tensure(__FILE__, __LINE__, pos == 5);
      }
   }

   template<>   template<>
   void testproc::test<8>()
   {
      //определение карт у оппов
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__,  opp1.hasCards());
         tensure(__FILE__, __LINE__, !opp2.hasCards());
         tensure(__FILE__, __LINE__, !opp3.hasCards());
         tensure(__FILE__, __LINE__, !opp4.hasCards());
         tensure(__FILE__, __LINE__, !opp5.hasCards());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, opp1.hasCards());
         tensure(__FILE__, __LINE__, opp2.hasCards());
         tensure(__FILE__, __LINE__, opp3.hasCards());
         tensure(__FILE__, __LINE__, opp4.hasCards());
         tensure(__FILE__, __LINE__, !opp5.hasCards());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, opp1.hasCards());
         tensure(__FILE__, __LINE__, opp2.hasCards());
         tensure(__FILE__, __LINE__, opp3.hasCards());
         tensure(__FILE__, __LINE__, opp4.hasCards());
         tensure(__FILE__, __LINE__, opp5.hasCards());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, !opp1.hasCards());
         tensure(__FILE__, __LINE__, !opp2.hasCards());
         tensure(__FILE__, __LINE__, !opp3.hasCards());
         tensure(__FILE__, __LINE__, opp4.hasCards());
         tensure(__FILE__, __LINE__, !opp5.hasCards());
      }
   }

   template<>   template<>
   void testproc::test<9>()
   {
      //определение фошки дилера у оппов
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, !opp1.isDealer());
         tensure(__FILE__, __LINE__, !opp2.isDealer());
         tensure(__FILE__, __LINE__, !opp3.isDealer());
         tensure(__FILE__, __LINE__, !opp4.isDealer());
         tensure(__FILE__, __LINE__, opp5.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, !opp1.isDealer());
         tensure(__FILE__, __LINE__, !opp2.isDealer());
         tensure(__FILE__, __LINE__, opp3.isDealer());
         tensure(__FILE__, __LINE__, !opp4.isDealer());
         tensure(__FILE__, __LINE__, !opp5.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, !opp1.isDealer());
         tensure(__FILE__, __LINE__, !opp2.isDealer());
         tensure(__FILE__, __LINE__, !opp3.isDealer());
         tensure(__FILE__, __LINE__, opp4.isDealer());
         tensure(__FILE__, __LINE__, !opp5.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_6.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, !opp1.isDealer());
         tensure(__FILE__, __LINE__, opp2.isDealer());
         tensure(__FILE__, __LINE__, !opp3.isDealer());
         tensure(__FILE__, __LINE__, !opp4.isDealer());
         tensure(__FILE__, __LINE__, !opp5.isDealer());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, opp1.isDealer());
         tensure(__FILE__, __LINE__, !opp2.isDealer());
         tensure(__FILE__, __LINE__, !opp3.isDealer());
         tensure(__FILE__, __LINE__, !opp4.isDealer());
         tensure(__FILE__, __LINE__, !opp5.isDealer());
      }
   }

   template<>   template<>
   void testproc::test<10>()
   {
      //определение размера ставки и действий оппонента
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp1.action() == Opp::BigBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp2.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp3.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp5.action() == Opp::Fold);
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp1.action() == Opp::Nope);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.05));
         tensure(__FILE__, __LINE__, opp2.action() == Opp::SmallBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp3.action() == Opp::BigBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Raise);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp5.action() == Opp::Fold);
      }
      {
         QImage imgTable("sshot/acad_6max/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.05));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::SmallBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp5.action() == Opp::BigBlind);
      }
      {
         QImage imgTable("sshot/acad_6max/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.05));
         tensure(__FILE__, __LINE__, opp1.action() == Opp::SmallBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp2.action() == Opp::BigBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp3.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp5.action() == Opp::Call);
      }
      {
         QImage imgTable("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Bet);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.00));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_9.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.00));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Check);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.00));
      }
      {
         QImage imgTable("sshot/acad_6max/acad_11.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp1.bet(), 0.20));
         tensure(__FILE__, __LINE__, opp1.action() == Opp::Call);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.20));
         tensure(__FILE__, __LINE__, opp2.action() == Opp::Call);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp3.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp3.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.10));
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Raise);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.00));
         tensure(__FILE__, __LINE__, opp5.action() == Opp::Fold);
      }
      {
         QImage imgTable("sshot/acad_6max/acad_16.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         Opp opp2 = proc.opp("2");
         Opp opp3 = proc.opp("3");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         tensure(__FILE__, __LINE__, opp1.action() == Opp::Nope);
         tensure(__FILE__, __LINE__, opp2.action() == Opp::Call);
         tensure(__FILE__, __LINE__, opp3.action() == Opp::Call);
         tensure(__FILE__, __LINE__, opp4.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, opp5.action() == Opp::Fold);
      }
      {
         QImage imgTable("sshot/acad_6max/acad_17.png");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         Opp opp1 = proc.opp("1");
         tensure(__FILE__, __LINE__, opp1.action() == Opp::BigBlind);
      }

   }

   template<>   template<>
   void testproc::test<11>()
   {
      ProcAcad proc("map/acad_6max.xml");
      {
         //получить хэши-ников
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         OppNick nick1 = proc.opp("1").nick();
         OppNick nick2 = proc.opp("2").nick();
         OppNick nick3 = proc.opp("3").nick();
         OppNick nick4 = proc.opp("4").nick();
         OppNick nick5 = proc.opp("5").nick();
         {
         QString hash = nick1.hash();
         OppNick nick_recon(hash);
         tensure(__FILE__, __LINE__, nick1 == nick_recon);
         }
         {
         QString hash = nick2.hash();
         OppNick nick_recon(hash);
         tensure(__FILE__, __LINE__, nick2 == nick_recon);
         }
         {
         QString hash = nick3.hash();
         OppNick nick_recon(hash);
         tensure(__FILE__, __LINE__, nick3 == nick_recon);
         }
         {
         QString hash = nick4.hash();
         OppNick nick_recon(hash);
         tensure(__FILE__, __LINE__, nick4 == nick_recon);
         }
         {
         QString hash = nick5.hash();
         OppNick nick_recon(hash);
         tensure(__FILE__, __LINE__, nick5 == nick_recon);
         }
      }
   }

   template<>   template<>
   void testproc::test<12>()
   {
      ProcAcad proc("map/acad_6max.xml");
      {
         QImage imgTable("sshot/acad_6max/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         proc.street();
         tensure(__FILE__, __LINE__, proc.isPreflop());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         proc.street();
         tensure(__FILE__, __LINE__, proc.isPreflop());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         proc.street();
         tensure(__FILE__, __LINE__, proc.isFlop());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_9.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         proc.street();
         tensure(__FILE__, __LINE__, proc.isRiver());
      }
      {
         QImage imgTable("sshot/acad_6max/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         proc.street();
         tensure(__FILE__, __LINE__, proc.isTurn());
      }
   }

   template<>   template<>
   void testproc::test<13>()
   {
      ProcAcad proc("map/acad_fr.xml");
      {
         QImage imgTable("sshot/acad_fr/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         tensure(__FILE__, __LINE__, proc.hasFold());
         tensure(__FILE__, __LINE__, proc.hasCall());
         tensure(__FILE__, __LINE__, proc.hasCheck());
         tensure(__FILE__, __LINE__, proc.hasRaise());
         tensure(__FILE__, __LINE__, proc.holeCard("first") == "As");
         tensure(__FILE__, __LINE__, proc.holeCard("second") == "Th");
         tensure(__FILE__, __LINE__, !proc.isDealer());
         proc.street();
         tensure(__FILE__, __LINE__, proc.isPreflop());

         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.pot(), 0.55));
         tensure(__FILE__, __LINE__, qFuzzyCompare(proc.stack(), 10.0));
         Opp opp2 = proc.opp("2");
         Opp opp4 = proc.opp("4");
         Opp opp5 = proc.opp("5");
         Opp opp6 = proc.opp("6");
         Opp opp8 = proc.opp("8");
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.stack(), 9.95));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.stack(), 9.90));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.stack(), 10));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp6.stack(), 9.90));
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp8.stack(), 10.0));

         tensure(__FILE__, __LINE__,  opp2.hasCards());
         tensure(__FILE__, __LINE__,  opp4.hasCards());
         tensure(__FILE__, __LINE__, !opp5.hasCards());
         tensure(__FILE__, __LINE__,  opp6.hasCards());
         tensure(__FILE__, __LINE__, !opp8.hasCards());

         tensure(__FILE__, __LINE__, !opp2.isDealer());
         tensure(__FILE__, __LINE__, !opp4.isDealer());
         tensure(__FILE__, __LINE__, !opp5.isDealer());
         tensure(__FILE__, __LINE__, !opp6.isDealer());
         tensure(__FILE__, __LINE__, !opp8.isDealer());
         
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp2.bet(), 0.05));
         tensure(__FILE__, __LINE__,               opp2.action() == Opp::SmallBlind);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp4.bet(), 0.10));
         tensure(__FILE__, __LINE__,               opp4.action() == Opp::Call);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp5.bet(), 0.00));
         tensure(__FILE__, __LINE__,               opp5.action() == Opp::Fold);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp6.bet(), 0.10));
         tensure(__FILE__, __LINE__,               opp6.action() == Opp::Call);
         tensure(__FILE__, __LINE__, qFuzzyCompare(opp8.bet(), 0.00));
         tensure(__FILE__, __LINE__,               opp8.action() == Opp::Fold);


      }
   }
}

