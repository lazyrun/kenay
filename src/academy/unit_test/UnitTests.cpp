#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "CardBase.h"
#include "ProcAcad.h"
#include "Config.h"

namespace tut
{
   // ќпределение типов
   struct bot_shared_data
   {
   };

   typedef test_group<bot_shared_data, 20> tg_bot;// 20 tests
   typedef tg_bot::object testobject;

   tg_bot group_bot("ACAD");

   static QString cardFromImage(QImage & img)
   {
      int imgW = img.width();
      int imgH = img.height();

      int minDist = imgW * imgH;
      int minIdx = 0;
      
      CardBase cardBase;
      BoolMatrix * whatMatrix = new BoolMatrix(img, 200);
      const int baseCount = cardBase.count();
      for (int i = 0; i < baseCount; ++i)
      {
         const BoolMatrix * bm = cardBase.matrix(i);
         //изображение подт€гиваетс€ под эталон
         if ((bm->width()  != imgW) ||
             (bm->height() != imgH) )
         {
            img = img.scaled(imgW, imgH);
            delete whatMatrix;
            whatMatrix = new BoolMatrix(img, 200);
         }
         int res = *whatMatrix - *bm;

         if (res < minDist)
         {
            minDist = res;
            minIdx = i;
         }
         if (res == 0)
            break;
      }
      delete whatMatrix;
      
      return cardBase.nominal(minIdx);
   }
   
   // ќпределение тестов
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
         QString rec = cardFromImage(imgCard);
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
      QImage imgTable("sshot/acad_1.bmp");
      tensure(__FILE__, __LINE__, !imgTable.isNull());

      ProcAcad proc("map/acad.xml");
      proc.setImage(imgTable);
      tensure(__FILE__, __LINE__, proc.hasFold());
      tensure(__FILE__, __LINE__, proc.hasCall());
      tensure(__FILE__, __LINE__, proc.hasCheck());
      tensure(__FILE__, __LINE__, proc.hasRaise());
      //   proc.holeFirst()
      //   proc.holeSecond()
      //   proc.pot()
      //   proc.stack()
   }
}
