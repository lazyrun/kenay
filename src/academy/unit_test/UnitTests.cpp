#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "ProcAcad.h"
#include "Config.h"

namespace tut
{
   // ����������� �����
   struct bot_shared_data
   {
   };

   typedef test_group<bot_shared_data, 20> tg_bot;// 20 tests
   typedef tg_bot::object testobject;

   tg_bot group_bot("ACAD");

   // ����������� ������
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
         config.settingValue("stack", "").toString();
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
         tensure(__FILE__, __LINE__, !proc.isDealer());
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
         tensure(__FILE__, __LINE__, !proc.isDealer());
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
         tensure(__FILE__, __LINE__, !proc.isDealer());
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
         tensure(__FILE__, __LINE__, !proc.isDealer());
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
         tensure(__FILE__, __LINE__, !proc.isDealer());
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
         tensure(__FILE__, __LINE__, !proc.isDealer());
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
         tensure(__FILE__, __LINE__, proc.isDealer());
      }
      {
         QImage imgTable("sshot/acad_14.bmp");
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

   template<>   template<>
   void testobject::test<6>()
   {
      //������������ ����� ����������
      ProcAcad proc("map/acad.xml");
      {
         QImage imgTable("sshot/acad_1.bmp");
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
         QImage imgTable("sshot/acad_12.bmp");
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
   }

   template<>   template<>
   void testobject::test<7>()
   {
      ProcAcad proc("map/acad.xml");
      {
         //�������� ����-�����
         QImage imgTable("sshot/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable.isNull());
         proc.setImage(imgTable);
         OppNick nick = proc.opp("1").nick();

         QImage imgTable1("sshot/acad_3.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();

         tensure(__FILE__, __LINE__, nick == nick1);

         QImage imgTable2("sshot/acad_4.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("1").nick();

         tensure(__FILE__, __LINE__, nick == nick2);

         QImage imgTable3("sshot/acad_15.bmp");
         tensure(__FILE__, __LINE__, !imgTable3.isNull());
         proc.setImage(imgTable3);
         OppNick nick3 = proc.opp("4").nick();

         tensure(__FILE__, __LINE__, nick == nick3);
      }
      {
         QImage imgTable1("sshot/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();

         QImage imgTable2("sshot/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("5").nick();

         tensure(__FILE__, __LINE__, nick1 == nick2);
      }
      {
         QImage imgTable1("sshot/acad_1.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();

         QImage imgTable2("sshot/acad_2.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("5").nick();

         tensure(__FILE__, __LINE__, nick1 == nick2);
      }
      {
         QImage imgTable1("sshot/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("2").nick();

         QImage imgTable2("sshot/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable2.isNull());
         proc.setImage(imgTable2);
         OppNick nick2 = proc.opp("5").nick();

         tensure(__FILE__, __LINE__, nick1 == nick2);
      }
      {
         QImage imgTable1("sshot/acad_7.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("2").nick();
         
         //����� ������� ����� ��������� �� ������ ��������
         QImage imgTable2("sshot/acad_13.bmp");
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
         QImage imgTable1("sshot/acad_13.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("3").nick();
         
         //����� ������� ����� ��������� �� ������ ��������
         QImage imgTable2("sshot/acad_14.bmp");
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
         QImage imgTable1("sshot/acad_14.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("1").nick();
         
         //����� ������� ����� ��������� �� ������ ��������
         QImage imgTable2("sshot/acad_15.bmp");
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
         QImage imgTable1("sshot/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("3").nick();
         
         //����� ������� ����� ��������� �� ������ ��������
         QImage imgTable2("sshot/acad_1.bmp");
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
         QImage imgTable1("sshot/acad_8.bmp");
         tensure(__FILE__, __LINE__, !imgTable1.isNull());
         proc.setImage(imgTable1);
         OppNick nick1 = proc.opp("5").nick();
         
         //����� ������� ����� ��������� �� ������ ��������
         QImage imgTable2("sshot/acad_8.bmp");
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
   void testobject::test<8>()
   {
      //����������� ���� � �����
      ProcAcad proc("map/acad.xml");
      {
         QImage imgTable("sshot/acad_1.bmp");
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
         QImage imgTable("sshot/acad_2.bmp");
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
         QImage imgTable("sshot/acad_3.bmp");
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
         QImage imgTable("sshot/acad_8.bmp");
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
   void testobject::test<9>()
   {
      //����������� ����� ������ � �����
      ProcAcad proc("map/acad.xml");
      {
         QImage imgTable("sshot/acad_1.bmp");
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
         QImage imgTable("sshot/acad_3.bmp");
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
         QImage imgTable("sshot/acad_4.bmp");
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
         QImage imgTable("sshot/acad_6.bmp");
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
         QImage imgTable("sshot/acad_2.bmp");
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
}
