#include "ImgUtils.h"


int ImgUtils::hueCount(const QImage & img, int minHue, int maxHue, 
             int fromw, int fromh, int thr, QPoint & pt)
{
   int hue = 0;
   int imgw = img.width();
   int imgh = img.height();
   if (fromw >= imgw)
      fromw = 0;
   if (fromh >= imgh)
      fromh = 0;

   for (int h = fromh; h < imgh; h++)
   {
      for (int w = fromw; w < imgw; w++)
      {
         QRgb rgb = img.pixel(w, h);
         QColor cl(rgb);
         if (cl.hue() > minHue && cl.hue() < maxHue) 
         {
            hue++;
            if (hue > thr)
            {
               //сгенерить рандомный QPoint
               int x = fromw + (qrand() % (w - fromw)) + 10;
               int y = fromh + (qrand() % (h - fromh)) + 10;
               pt.setX(x);
               pt.setY(y);
               return hue;
            }
         }
      }
   }
   return hue;
}

int ImgUtils::maxHue(const QImage & img)
{
   QMap<int, int> hueGist;
   QMap<int, int> hueGray;

   const int width = img.width();
   const int height = img.height();
   for (int x = 0; x < width; ++x)
   {
      for (int y = 0; y < height; ++y)
      {
         QRgb rgb = img.pixel(x, y);
         QColor cl(rgb);
         if (cl.red() > 200 && cl.green() > 200 && cl.blue() > 200)
            continue;
         
         int gr = qGray(rgb);
         hueGray[gr]++;
      }
   }
   int maxHueCount = 0;
   int maxHueValue = 0;
   int maxGray = 0;
   foreach (int hue, hueGray.keys())
   {
      if (hueGray[hue] > maxHueCount)
      {
         maxHueCount = hueGray[hue];
         maxHueValue = hue;
         maxGray = hueGray[hue];
      }
   }
   return maxHueValue;
}

void ImgUtils::toBlackWhiteMid(QImage & img, const int threshold)
{
   int grWhite = 0, grBlack = 0;
   int points = 0;
   const int thr = threshold >> 1;
   const int w = img.width();
   const int h = img.height();
   int maxGray = 0;
   int minGray = 256;

   QVector<QPoint> pts;
   for (int x = 0; x < w; ++x)
   {
      for (int y = 0; y < h; ++y)
      {
         QRgb rgb = img.pixel(x, y);
         int gr = qGray(rgb);
         if (gr > threshold)//белый
            continue;
         
         points++;
         pts.push_back(QPoint(x, y));
         if (gr > maxGray)
            maxGray = gr;
         if (gr < minGray)
            minGray = gr;
      }
   }
   
   int midGray = ((maxGray - minGray) >> 1) + minGray;
   int cb = 0, cw = 0;
   foreach (QPoint pt, pts)
   {
      QRgb rgb = img.pixel(pt);
      int gr = qGray(rgb);

      if (gr < midGray)
      {
         grBlack += gr;//ближе к черному
         cb++;
      }
      else
      {
         grWhite += gr;//ближе к белому
         cw++;
      }
   }
   if (points == 0)
      return;
   int th = (grBlack - grWhite) / points;
   int mid = thr - th;
   mid += midGray;
   if (mid > threshold)
      mid = threshold;
   toBlackWhite(img, mid);
}

void ImgUtils::toBlackWhiteByHue(QImage & img, int minHue, int maxHue)
{
   const int width = img.width();
   const int height = img.height();
   
   for (int w = 0; w < width; ++w)
   {
      for (int h = 0; h < height; ++h)
      {
         QRgb rgb = img.pixel(w, h);
         QColor cl(rgb);
         int hu = cl.hue();
         if (hu >= minHue && hu <= maxHue)
         {
            img.setPixel(w,h, qRgb(0, 0, 0));
         }     
         else
         {
            img.setPixel(w,h, qRgb(255, 255, 255));
         }
      }
   }
}

void ImgUtils::toBlackWhiteByHue(QImage & img)
{
   int maxH = maxHue(img);
   const int width = img.width();
   const int height = img.height();
   
   for (int w = 0; w < width; ++w)
   {
      for (int h = 0; h < height; ++h)
      {
         QRgb rgb = img.pixel(w, h);
         QColor cl(rgb);
         //int hu = cl.hue();
         int gr = qGray(rgb);
         if ((gr < maxH + 20) && (gr > maxH - 20))
         {
            img.setPixel(w,h, qRgb(0, 0, 0));
         }     
         else
         {
            img.setPixel(w,h, qRgb(255, 255, 255));
         }
      }
   }
}

void ImgUtils::denoise(QImage & img, uchar threshold)
{
   //убрать одиночные белые пиксели
   for (int x = 0; x < img.width(); x++)
   {
      for (int y = 0; y < img.height(); y++)
      {
         if (img.pixel(x, y) == qRgb(255, 255, 255))
         {
            int xdb = (x == 0) ? 0 : x - 1;
            int xde = (x == img.width() - 1) ? img.width() - 1 : x + 1;
            int ydb = (y == 0) ? 0 : y - 1;
            int yde = (y == img.height() - 1) ? img.height() - 1 : y + 1;
            int wpix = 0;
            for (int xx = xdb; xx <= xde; xx++)
            {
               for (int yy = ydb; yy <= yde; yy++)
               {
                  if (img.pixel(xx, yy) == qRgb(255, 255, 255))
                  {
                     wpix++;
                  }
               }
            }
            if (wpix < threshold)
            {
               //удалить точку
               img.setPixel(x, y, qRgb(0, 0, 0));
            }
         }
      }
   }
}

void ImgUtils::toBlackWhite(QImage & img, uchar threshold)
{
   for (int w = 0; w < img.width(); w++)
   {
      for (int h = 0; h < img.height(); h++)
      {
         QRgb rgb = img.pixel(w, h);
         QColor cl(rgb);
         if (qGray(rgb) < threshold || 
            (cl.saturation() > 0))
         {
            img.setPixel(w,h, qRgb(0, 0, 0));
         }     
         else
         {
            img.setPixel(w,h, qRgb(255, 255, 255));
         }
      }
   }
}

int ImgUtils::countCheckLetters(const QImage & imgCheck)
{
   //очерняем
   const int width = imgCheck.width();
   const int height = imgCheck.height();

   BoolMatrix checkMatrix(imgCheck);
   for (int x = 0; x < width; ++x)
   {
      for (int y = 0; y < height; ++y)
      {
         QRgb rgb = imgCheck.pixel(x, y);
         QColor cl(rgb);
         if (cl.hue() >= 13 && cl.hue() <= 33) 
         {
            checkMatrix.set(x, y, false);
            //imgCheck.setPixel(x, y, qRgb(0, 0, 0));
         }
         else
         {
            checkMatrix.set(x, y, true);
            //imgCheck.setPixel(x, y, qRgb(255, 255, 255));
         }
      }
   }

   //сканируем рисунок на предмет пробелов
   int countLetters = 0;
   int count = 0;

   bool isBlackLinePrev = true;
   bool isBlackLineCurr = true;
   bool isLetterFinish  = false;

   for (int w = 0; w < width; ++w)
   {
      isLetterFinish = false;
      isBlackLineCurr = true;
      for (int h = 0; h < height; ++h)
      {
         if (checkMatrix.at(w, h) == true)
         {
            isBlackLineCurr = false;
            break;
         }
      }
      if ((isBlackLinePrev == true) && (isBlackLineCurr == false))
      {
         countLetters++;
         isBlackLinePrev = false;
      }
      else if ((isBlackLinePrev == false) && (isBlackLineCurr == true))
      {
         isBlackLinePrev = true;
         isLetterFinish = true;
      }
      if (isLetterFinish)
      {
         count++;
      }
   }
   
   return count;
}

int ImgUtils::countCheckLetters(const BoolMatrix & checkMatrix)
{
   //очерняем
   const int width = checkMatrix.width();
   const int height = checkMatrix.height();

   //сканируем рисунок на предмет пробелов
   int countLetters = 0;
   int count = 0;

   bool isBlackLinePrev = true;
   bool isBlackLineCurr = true;
   bool isLetterFinish  = false;

   for (int w = 0; w < width; ++w)
   {
      isLetterFinish = false;
      isBlackLineCurr = true;
      for (int h = 0; h < height; ++h)
      {
         if (checkMatrix.at(w, h) == true)
         {
            isBlackLineCurr = false;
            break;
         }
      }
      if ((isBlackLinePrev == true) && (isBlackLineCurr == false))
      {
         countLetters++;
         isBlackLinePrev = false;
      }
      else if ((isBlackLinePrev == false) && (isBlackLineCurr == true))
      {
         isBlackLinePrev = true;
         isLetterFinish = true;
      }
      if (isLetterFinish)
      {
         count++;
      }
   }
   
   return count;
}

QList<BoolMatrix> ImgUtils::splitByLetters(const BoolMatrix & checkMatrix)
{
   QList<BoolMatrix> letters;
   //очерняем
   const int width = checkMatrix.width();
   const int height = checkMatrix.height();

   //сканируем рисунок на предмет пробелов
   int countLetters = 0;
   int count = 0;

   bool isBlackLinePrev = true;
   bool isBlackLineCurr = true;
   bool isLetterFinish  = false;

   int w_begin = 0;
   int w_end = 0;
   for (int w = 0; w < width; ++w)
   {
      isLetterFinish = false;
      isBlackLineCurr = true;
      for (int h = 0; h < height; ++h)
      {
         if (checkMatrix.at(w, h) == true)
         {
            isBlackLineCurr = false;
            break;
         }
      }
      if ((isBlackLinePrev == true) && (isBlackLineCurr == false))
      {
         w_begin = w;
         countLetters++;
         isBlackLinePrev = false;
      }
      else if ((isBlackLinePrev == false) && (isBlackLineCurr == true))
      {
         isBlackLinePrev = true;
         isLetterFinish = true;
      }
      if (isLetterFinish)
      {
         w_end = w - 1;
         count++;
         QRect rect(QPoint(w_begin, 0), QPoint(w_end, height));
         BoolMatrix bm(checkMatrix, rect);
         letters << bm;
      }
   }
   
   return letters;
}

QList<int> ImgUtils::closedAreas(const BoolMatrix & bmatr_in)
{
   BoolMatrix bmBak = bmatr_in;
  
   int wCount = -1;
   int wx = 0;
   int wy = 0;

   QList<int> counts;
   while (wCount != 0)
   {
      fillMatrix(bmBak, wx, wy);
      
      wx = -1;
      wy = -1;
      int whiteCount = 0;
      for (int x = 0; x < bmBak.width(); x++)
      {
         for (int y = 0; y < bmBak.height(); y++)
         {
            if (bmBak.isWhite(x, y))
            {
               if (wx == -1)
                  wx = x;
               if (wy == -1)
                  wy = y;
               whiteCount++;
            }
         }
      }
      if (wCount != -1)
         counts << wCount - whiteCount;

      wCount = whiteCount;
   }
   
   return counts;
}

void ImgUtils::fillMatrix(BoolMatrix & bm, int x, int y)
{
   if (x < 0 || y < 0) 
      return;

   if (bm.isWhite(x, y))
   {
      bm.set(x, y, true);
      fillMatrix(bm, x, y - 1);
      fillMatrix(bm, x - 1, y);
      fillMatrix(bm, x, y + 1);
      fillMatrix(bm, x + 1, y);
   }
}

qreal ImgUtils::parseDigit(const BoolMatrix & bm)
{
   QList<int> areas = closedAreas(bm);
   QString s;
   qDebug() << areas;
   return 0.;
}

bool ImgUtils::isDot(const BoolMatrix & bm)
{
   return false;
}


