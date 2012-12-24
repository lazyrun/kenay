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
         //обрезать снизу и сверху
         bm = cropTopBottom(bm);
         letters << bm;
      }
   }
   
   return letters;
}

BoolMatrix ImgUtils::cropTopBottom(const BoolMatrix & bm)
{
   const int w = bm.width();
   const int h = bm.height();
   int top = 0, bottom = h;
   for (int y = 0; y < h; y++)
   {
      for (int x = 0; x < w; x++)
      {
         if (bm.isBlack(x, y))
         {
            top = y;
            y = h;
            break;
         }
      }
   }

   for (int y = h - 1; y >= 0; y--)
   {
      for (int x = 0; x < w; x++)
      {
         if (bm.isBlack(x, y))
         {
            bottom = y;
            y = -1;
            break;
         }
      }
   }
   QRect crRect(QPoint(0, top), QPoint(w - 1, bottom));
   return BoolMatrix(bm, crRect);
}

QList<PointList> ImgUtils::closedAreas(const BoolMatrix & bmatr_in, Coherence coh)
{
   //матрица в которой уже готовые области закрашиваются черным
   BoolMatrix bmatr(bmatr_in);
   //
   //рабочая матрица для одной области
   BoolMatrix bm(bmatr);
   const int w = bm.width();
   const int h = bm.height();

   //отбираем точки с углом
   PointList startPoints;
   for (int y = 1; y < h - 1; ++y)
   {
      for (int x = 1; x < w - 1; ++x)
      {
         //белый
         if (bm.at(x, y) == 0)
         {
            //смотрим черный угол
            if (bm.at(x - 1, y)     == 1 &&
                //bm.at(x - 1, y - 1) == 1 &&
                bm.at(x, y - 1)     == 1)
               
            {
               startPoints << QPoint(x, y);
            }
         }
      }
   }
   
   if (startPoints.isEmpty())
   {
      return QList<PointList>();
   }
   QList<PointList> areas;
   PointList currentArea;
   QPoint pt = startPoints.at(0);

   //для всех стартовых точек строим замыкание
   for (int si = 0; si < startPoints.count(); ++si)
   {
      QPoint s = startPoints.at(si);
      //стартовая точка - белая
      if ((bm.at(s.x(), s.y()) == 0) && isCrux(bm, s.x(), s.y()))
      {
         QPoint pt = s;
         for (;;)
         {
            QList<QPoint> sublingPoints;
            const QPoint pt1 = QPoint(pt.x(),      pt.y() - 1);
            if (coh == Eight)
            {
               //8ми связность
               const QPoint pt2 = QPoint(pt.x() + 1,  pt.y() - 1);
               const QPoint pt3 = QPoint(pt.x() + 1,  pt.y());
               const QPoint pt4 = QPoint(pt.x() + 1,  pt.y() + 1);
               const QPoint pt5 = QPoint(pt.x(),      pt.y() + 1);
               const QPoint pt6 = QPoint(pt.x() - 1,  pt.y() + 1);
               const QPoint pt7 = QPoint(pt.x() - 1,  pt.y());
               const QPoint pt8 = QPoint(pt.x() - 1,  pt.y() - 1);

               sublingPoints << pt1 << pt2 << pt3 << pt4 << pt5 << pt6 << pt7 << pt8;
            }
            else
            {
               //4-х связность
               const QPoint pt3 = QPoint(pt.x() + 1,  pt.y());
               const QPoint pt5 = QPoint(pt.x(),      pt.y() + 1);
               const QPoint pt7 = QPoint(pt.x() - 1,  pt.y());

               sublingPoints << pt1 << pt3 << pt5 << pt7;
            }

            //крутим в поисках черной точки
            int first_black = 0;
            if (bm.at(pt1.x(), pt1.y()) == 1)
            {
               foreach (QPoint spt, sublingPoints)
               {
                  if (bm.at(spt.x(), spt.y()) == 1)
                  {
                     first_black = sublingPoints.indexOf(spt);
                     break;
                  }
               }
            }
            else
            {
               first_black = 0;
            }
            QList<QPoint> swap1 = sublingPoints.mid(first_black);
            QList<QPoint> swap2 = sublingPoints.mid(0, first_black);
            QList<QPoint> circularedPoints;
            circularedPoints << swap1 << swap2;
            
            bool hasSibling = false;
            bool isBreak = false;
            foreach (QPoint spt, circularedPoints)
            {
               //ищем в окрестности белую точку
               if (bm.at(spt.x(), spt.y()) == 0)
               {
                  bm.set(pt.x(), pt.y(), 1);
                  if (isCrux(bmatr, spt.x(), spt.y()))
                  {
                     currentArea << pt;
                     //startPoints.removeOne(pt);
                     pt = spt;
                     hasSibling = true;
                     break;
                  }
                  else
                  {
                     //область разорвана - стартовая точка - лажа
                     //выходим и пробуем со следующей
                     //очищаем матрицу
                     bm = bmatr;
                     currentArea.clear();
                     isBreak = true;
                     break;
                  }
               }
            }
            if (isBreak)
               break;

            if (!hasSibling)
            {
               bm.set(pt.x(), pt.y(), 1);
               currentArea << pt;
               areas << currentArea;
               bmatr = bm;
               currentArea.clear();
               break;
            }
         }
      }
   }

   return areas;
}

bool ImgUtils::isCrux(const BoolMatrix & bm, const int sx, const int sy)
{
   int leftBound     = -1;
   int rightBound    = -1;
   int topBound      = -1;
   int bottomBound   = -1;
   for (int x = sx - 1; x >= 0; x--)
   {
      if (bm.at(x, sy) == 1)
      {
         leftBound = x;
         break;
      }
   }
   
   for (int x = sx + 1; x < bm.width(); x++)
   {
      if (bm.at(x, sy) == 1)
      {
         rightBound = x;
         break;
      }
   }

   for (int y = sy - 1; y >= 0; y--)
   {
      if (bm.at(sx, y) == 1)
      {
         topBound = y;
         break;
      }
   }
   
   for (int y = sy + 1; y < bm.height(); y++)
   {
      if (bm.at(sx, y) == 1)
      {
         bottomBound = y;
         break;
      }
   }
   
   if (leftBound != -1 && rightBound != -1 && topBound != -1 && bottomBound != -1)
   {
      if ((rightBound - leftBound > 2) || (bottomBound - topBound > 2))
      {
         return true;
      }
   }
   return false;
}

qreal ImgUtils::parseRealNumber(const QList<BoolMatrix> & list, 
                                       const Parser * parser)
{
   qreal val = 0.0;
   int dot = 0;
   //считаем целую часть
   for (int i = 0; i < list.count(); i++)
   {
      bool areDot = isDot(list[i]);
      if (!areDot)
      {
         qreal digit = parseDigit(list[i], parser);
         val = val * 10. + digit;
      }
      else
      {
         dot = i + 1;
         break;
      }
   }
   //считаем дробную часть
   if (dot)//при наличии точки
   {
      qreal fract = 0.0;
      for (int i = 0; i < list.count() - dot; i++)
      {
         qreal digit = parseDigit(list[dot + i], parser);
         fract = fract + digit / qPow(10., i + 1);
      }
      val += fract;
   }
   return val;
}

qreal ImgUtils::parseDigit(const BoolMatrix & bm, const Parser * parser)
{
   QList<PointList> areas = closedAreas(bm, Four);
   //ошибка
   if (areas.count() > 2)
      return -1.;
   //Однозначно 8
   if (areas.count() == 2)
      return 8.0;
   
   //сканируем профили
   //построить профиль слева
   QVector<ProfileItem> leftProfile = scanLeftProfile(bm);
   //построить профиль справа
   QVector<ProfileItem> rightProfile = scanRightProfile(bm);
   if (areas.count() == 1)
   {
      //6, 9, 0, 4
      bool leftIsSix = parser->isSixProfile(leftProfile);
      bool rightIsSix = parser->isSixProfile(rightProfile);
      if (leftIsSix && rightIsSix)
         return 0.0;
      else if (leftIsSix)
         return 6.0;
      else if (rightIsSix)
         return 9.0;
      else
         return 4.0;
   }
   //2
   bool isLeftTwo  = parser->isLeftTwoProfile(leftProfile);
   bool isRightTwo = parser->isRightTwoProfile(rightProfile);
   if (isLeftTwo && isRightTwo)
      return 2.0;
   //5
   bool isLeftFive  = parser->isLeftFiveProfile(leftProfile);
   bool isRightFive = parser->isRightFiveProfile(rightProfile);
   if (isLeftFive && isRightFive)
      return 5.0;
   //3
   bool isLeftThree  = parser->isLeftThreeProfile(leftProfile);
   bool isRightThree = parser->isRightThreeProfile(rightProfile);
   if (isLeftThree && isRightThree)
      return 3.0;

   //1, 7
   bool isLeftOne  = parser->isLeftOneProfile(leftProfile);
   bool isRightOne = parser->isRightOneProfile(rightProfile);
   if (isLeftOne && isRightOne)
      return 1.0;

   bool isLeftSeven  = parser->isLeftSevenProfile(leftProfile);
   bool isRightSeven = parser->isRightSevenProfile(rightProfile);
   if (isLeftSeven && isRightSeven)
      return 7.0;

   return -1.;
}

bool ImgUtils::isDot(const BoolMatrix & bm)
{
   //подсчитать количество черных точек - их очень мало
   const int w = bm.width();
   const int h = bm.height();

   int black = 0;
   for (int y = 0; y < h; ++y)
   {
      for (int x = 0; x < w; ++x)
      {
         if (bm.at(x, y) == 1)
         {
            black++;
         }
      }
   }
   if (black < 6 && black > 0)
      return true;

   return false;
}

QVector<ProfileItem> ImgUtils::scanLeftProfile(const BoolMatrix & imgMatrix)
{
   //сканирование левого профиля
   const int w = imgMatrix.width();
   const int h = imgMatrix.height();
   int min_x = w, max_x = 0;

   QVector<int> x_array;
   for (int y = 0; y < h; ++y)
   {
      for (int x = 0; x < w; ++x)
      {
         if (imgMatrix.at(x, y) == 1)
         {
            if (x < min_x)
               min_x = x;
            if (x > max_x)
               max_x = x;
            x_array.push_back(x);
            break;
         }
      }
   }
   
   //определяем середину
   int x_border = min_x + (max_x - min_x) / 2;
   //формируем портрет профиля
   QVector<bool> prof;
   QVector<ProfileItem> profile;

   //qDebug() << x_array;
   int ccnt = 0;
   bool item = true;
   foreach (int x, x_array)
   {
      item = (x > x_border);
      if (prof.size() == 0)
      {
         prof.push_back(item);
         ccnt++;
      }
      else
      {
         if (prof.last() != item)
         {
            ProfileItem it;
            it.item = !item;
            it.value = qreal(ccnt) / qreal(h);
            profile.push_back(it);
            prof.push_back(item);
            ccnt = 1;
         }
         else
         {
            ccnt++;
         }
      }
   }

   if (ccnt != 0)
   {
      ProfileItem it;
      it.item = item;
      it.value = qreal(ccnt) / qreal(h);
      profile.push_back(it);
   }
   
   return profile;
}

QVector<ProfileItem> ImgUtils::scanRightProfile(const BoolMatrix & imgMatrix)
{
   //сканирование правого профиля
   const int w = imgMatrix.width();
   const int h = imgMatrix.height();
   int min_x = w, max_x = 0;

   QVector<int> x_array;
   for (int y = 0; y < h; ++y)
   {
      for (int x = w - 1; x >= 0; --x)
      {
         if (imgMatrix.at(x, y) == 1)
         {
            if (x < min_x)
               min_x = x;
            if (x > max_x)
               max_x = x;
            x_array.push_back(x);
            break;
         }
      }
   }
   
   //определяем середину
   int x_border = min_x + (int)qRound(qreal(max_x - min_x) / 2. + 0.1);
   //формируем портрет профиля
   QVector<bool> prof;
   QVector<ProfileItem> profile;

   //qDebug() << x_array;
   int ccnt = 0;
   bool item = true;
   foreach (int x, x_array)
   {
      item = (x < x_border);
      if (prof.size() == 0)
      {
         prof.push_back(item);
         ccnt++;
      }
      else
      {
         if (prof.last() != item)
         {
            ProfileItem it;
            it.item = !item;
            it.value = qreal(ccnt) / qreal(h);
            profile.push_back(it);
            prof.push_back(item);
            ccnt = 1;
         }
         else
         {
            ccnt++;
         }
      }
   }

   if (ccnt != 0)
   {
      ProfileItem it;
      it.item = item;
      it.value = qreal(ccnt) / qreal(h);
      profile.push_back(it);
   }
   
   return profile;
}

QVector<ProfileItem> ImgUtils::scanDownProfile(const BoolMatrix & imgMatrix)
{
   //сканирование нижнего профиля
   const int w = imgMatrix.width();
   const int h = imgMatrix.height();
   int min_y = h, max_y = 0;

   QVector<int> y_array;
   for (int x = 1; x < w; ++x)
   {
      for (int y = h - 1; y >= 0; --y)
      {
         if (imgMatrix.at(x, y) == 1)
         {
            if (y < min_y)
               min_y = y;
            if (y > max_y)
               max_y = y;
            y_array.push_back(y);
            break;
         }
      }
   }
   
   //определяем середину
   int y_border = min_y + (max_y - min_y) / 2;
   //формируем портрет профиля
   QVector<bool> prof;
   QVector<ProfileItem> profile;

   //qDebug() << x_array;
   int ccnt = 0;
   bool item = true;
   foreach (int y, y_array)
   {
      item = (y < y_border);
      if (prof.size() == 0)
      {
         prof.push_back(item);
         ccnt++;
      }
      else
      {
         if (prof.last() != item)
         {
            ProfileItem it;
            it.item = !item;
            it.value = qreal(ccnt) / qreal(w);
            profile.push_back(it);
            prof.push_back(item);
            ccnt = 1;
         }
         else
         {
            ccnt++;
         }
      }
   }

   if (ccnt != 0)
   {
      ProfileItem it;
      it.item = item;
      it.value = qreal(ccnt) / qreal(w);
      profile.push_back(it);
   }
   
   return profile;
}

