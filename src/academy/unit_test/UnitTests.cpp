#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"
#include "CardBase.h"

namespace tut
{
   // ����������� �����
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
         //����������� ������������� ��� ������
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
         QString rec = cardFromImage(imgCard);
         tensure(__FILE__, __LINE__, rec == card);
      }
   }

   template<>   template<>
   void testobject::test<2>()
   {

   }
}
