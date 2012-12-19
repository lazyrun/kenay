#include "ImgUtils.h"
#include "BoolMatrix.h"

int hueCount(const QImage & img, int minHue, int maxHue, 
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

int maxHue(const QImage & img)
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

void toBlackWhiteMid(QImage & img, const int threshold)
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

void toBlackWhiteByHue(QImage & img, int minHue, int maxHue)
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

void toBlackWhiteByHue(QImage & img)
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

void denoise(QImage & img, uchar threshold)
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

void toBlackWhite(QImage & img, uchar threshold)
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

int countCheckLetters(const QImage & imgCheck)
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

   QPoint ptLeft, ptRight;
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
