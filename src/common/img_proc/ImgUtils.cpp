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
               //��������� ��������� QPoint
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
         if (gr > threshold)//�����
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
         grBlack += gr;//����� � �������
         cb++;
      }
      else
      {
         grWhite += gr;//����� � ������
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
   //������ ��������� ����� �������
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
               //������� �����
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
   //��������
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

   //��������� ������� �� ������� ��������
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
   //��������
   const int width = checkMatrix.width();
   const int height = checkMatrix.height();

   //��������� ������� �� ������� ��������
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
   //��������
   const int width = checkMatrix.width();
   const int height = checkMatrix.height();

   //��������� ������� �� ������� ��������
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
         //�������� ����� � ������
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
   //������� � ������� ��� ������� ������� ������������� ������
   BoolMatrix bmatr(bmatr_in);
   //
   //������� ������� ��� ����� �������
   BoolMatrix bm(bmatr);
   const int w = bm.width();
   const int h = bm.height();

   //�������� ����� � �����
   PointList startPoints;
   for (int y = 1; y < h - 1; ++y)
   {
      for (int x = 1; x < w - 1; ++x)
      {
         //�����
         if (bm.at(x, y) == 0)
         {
            //������� ������ ����
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

   //��� ���� ��������� ����� ������ ���������
   for (int si = 0; si < startPoints.count(); ++si)
   {
      QPoint s = startPoints.at(si);
      //��������� ����� - �����
      if ((bm.at(s.x(), s.y()) == 0) && isCrux(bm, s.x(), s.y()))
      {
         QPoint pt = s;
         for (;;)
         {
            QList<QPoint> sublingPoints;
            const QPoint pt1 = QPoint(pt.x(),      pt.y() - 1);
            if (coh == Eight)
            {
               //8�� ���������
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
               //4-� ���������
               const QPoint pt3 = QPoint(pt.x() + 1,  pt.y());
               const QPoint pt5 = QPoint(pt.x(),      pt.y() + 1);
               const QPoint pt7 = QPoint(pt.x() - 1,  pt.y());

               sublingPoints << pt1 << pt3 << pt5 << pt7;
            }

            //������ � ������� ������ �����
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
               //���� � ����������� ����� �����
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
                     //������� ��������� - ��������� ����� - ����
                     //������� � ������� �� ���������
                     //������� �������
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

qreal ImgUtils::parseDigit(const BoolMatrix & bm)
{
   QList<PointList> areas = closedAreas(bm, Four);
   //������
   if (areas.count() > 2)
      return -1.;
   //���������� 8
   if (areas.count() == 2)
      return 8.0;
   
   //��������� �������
   //��������� ������� �����
   QVector<ProfileItem> leftProfile = scanLeftProfile(bm);
   //��������� ������� ������
   QVector<ProfileItem> rightProfile = scanRightProfile(bm);
   if (areas.count() == 1)
   {
      //6, 9, 0, 4
      bool leftIsSix = isSixProfile(leftProfile);
      bool rightIsSix = isSixProfile(rightProfile);
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
   bool isLeftTwo  = isLeftTwoProfile(leftProfile);
   bool isRightTwo = isRightTwoProfile(rightProfile);
   if (isLeftTwo && isRightTwo)
      return 2.0;
   //5
   bool isLeftFive  = isLeftFiveProfile(leftProfile);
   bool isRightFive = isRightFiveProfile(rightProfile);
   if (isLeftFive && isRightFive)
      return 5.0;
   //3
   bool isLeftThree  = isLeftThreeProfile(leftProfile);
   bool isRightThree = isRightThreeProfile(rightProfile);
   if (isLeftThree && isRightThree)
      return 3.0;

   //1, 7
   bool isLeftOne  = isLeftOneProfile(leftProfile);
   bool isRightOne = isRightOneProfile(rightProfile);
   if (isLeftOne && isRightOne)
      return 1.0;

   bool isLeftSeven  = isLeftSevenProfile(leftProfile);
   bool isRightSeven = isRightSevenProfile(rightProfile);
   if (isLeftSeven && isRightSeven)
      return 7.0;

   return -1.;
}

bool ImgUtils::isDot(const BoolMatrix & bm)
{
   //���������� ���������� ������ ����� - �� ����� ����
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
   //������������ ������ �������
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
   
   //���������� ��������
   int x_border = min_x + (max_x - min_x) / 2;
   //��������� ������� �������
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
   //������������ ������� �������
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
   
   //���������� ��������
   int x_border = min_x + (int)qRound(qreal(max_x - min_x) / 2. + 0.1);
   //��������� ������� �������
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
   //������������ ������� �������
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
   
   //���������� ��������
   int y_border = min_y + (max_y - min_y) / 2;
   //��������� ������� �������
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

bool ImgUtils::isSixProfile(const QVector<ProfileItem> & profile)
{
   //�������� ������� 6
   //     1) ���� �� ������ ������ ������ ��������� 
   //���  2) ����� ���� ����� � ��� ����� ����� ���� ����� �������
   bool isSix = true;
   if (profile.count() > 1)
   {
      //����� ������������ �����
      int max_idx = 0;
      qreal maxval = 0.;
      for (int i = 0; i < profile.count(); ++i)
      {
         if (profile.at(i).value > maxval)
         {
            maxval = profile.at(i).value;
            max_idx = i;
         }
      }
      
      for (int i = 0; i < profile.count(); ++i)
      {
         if (i == max_idx)
            continue;
         
         if (profile.at(i).value * 5. >= maxval)
         {
            //������� �� ���������
            isSix = false;
            break;
         }
      }
   }
   else if (profile.count() == 1)
   {
      if (profile.at(0).value < 0.75)
         isSix = false;
   }
   else
   {
      isSix = false;
   }

   return isSix;
}

bool ImgUtils::isRightThreeProfile(const QVector<ProfileItem> & profile)
{
//true 0.0588235 
//false 0.352941 
//true 0.0588235 
//false 0.411765 
//true 0.0588235 
  
   if (profile.count() < 3)
      return false;

   if (profile.count() == 5)
   {
      if ((profile.at(0).item == true)    &&
          (profile.at(1).item == false) &&
          (profile.at(2).item == true) && 
          (profile.at(3).item == false) && 
          (profile.at(4).item == true))
          return true;

   }
      
   return false;
}

bool ImgUtils::isLeftThreeProfile(const QVector<ProfileItem> & profile)
{
/*
   profile = [5](
   {item=false value=0.22222222222222221 },
   {item=true value=0.11111111111111110 },
   {item=false value=0.11111111111111110 },
   {item=true value=0.33333333333333331 },
   {item=false value=0.22222222222222221 })
*/
   if (profile.count() < 3)
      return false;
   if (profile.count() == 5)
   {
      if ((profile.at(0).item == false)     &&
          (profile.at(1).item == true)  &&
          (profile.at(2).item == false) && 
          (profile.at(3).item == true) && 
          (profile.at(4).item == false))
      {
         if (qFuzzyCompare(profile.at(0).value, profile.at(4).value) &&
             qFuzzyCompare(profile.at(1).value, profile.at(2).value) &&
             profile.at(3).value > 0.3)
             return true;
      }
   }
   return false;

//false 0.222222 
//true 0.444444 
//false 0.222222 Z
//true 0.0555556 
//------ 
//true 0.0555556 
//false 0.833333 
//true 0.0555556 
//"3c" "" 
//
//false 0.222222 
//true 0.444444 
//false 0.277778 
//------ 
//true 0.0555556 
//false 0.833333 
//true 0.0555556 
//"3d" "" 
//
//false 0.235294 
//true 0.470588 
//false 0.235294 
//------ 
//false 0.941176 
//"3h" "" 
//
//false 0.235294 
//true 0.411765 
//false 0.294118 
//------ 
//true 0.0588235 
//false 0.823529 
//true 0.0588235 
//"3s" "" 

   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx + 1).value > 0.35) && 
       (profile.at(idx).value < 0.30) &&
       (profile.at(idx + 2).value < 0.30))
       return true;
      
   return false;
}


bool ImgUtils::isLeftFiveProfile(const QVector<ProfileItem> & profile)
{
//"2c" "" 
//false 0.555556 
//true 0.222222 
//false 0.166667 
// or
//true 0.0769231 
//false 0.384615 
//true 0.384615 
//false 0.0769231 
   if (profile.count() == 4)
   {
      if ((profile.at(0).item == true)    &&
          (profile.at(1).item == false) &&
          (profile.at(2).item == true) && 
          (profile.at(3).item == false))
      {
         if ((profile.at(1).value > profile.at(0).value) && 
             (profile.at(2).value > profile.at(3).value))
             return true;
      }
   }

   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx).value >= 0.30) && 
       (profile.at(idx + 1).value < 0.35) &&
       (profile.at(idx + 2).value < 0.30))
       return true;
      
   return false;
}

bool ImgUtils::isRightFiveProfile(const QVector<ProfileItem> & profile)
{
//false 0.111111 
//true 0.277778 
//false 0.555556 
//"5c" "" 
//false 0.111111 
//true 0.277778 
//false 0.555556 
//"5d" "" 
//false 0.117647 
//true 0.235294 
//false 0.588235 
//"5h" "" 
//false 0.117647 
//true 0.235294 
//false 0.588235 
//"5s" "" 
   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx + 2).value >= 0.40) && 
       (profile.at(idx).value < 0.30) &&
       (profile.at(idx + 1).value < 0.33))
       return true;
      
   return false;

}

bool ImgUtils::isLeftTwoProfile(const QVector<ProfileItem> & profile)
{
//"2h" "" 
//false 0.294118 
//true 0.294118 
//false 0.352941 
   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx + 2).value > profile.at(idx).value) && 
       (profile.at(idx + 2).value > profile.at(idx + 1).value) &&
       qAbs<qreal>(profile.at(idx).value - profile.at(idx + 1).value < 0.1))
       return true;
   
   //������������ ���������� ���� ������������ ����� �� ����� 15%
   //qreal diff1 = qAbs(profile.at(idx).value - profile.at(idx + 1).value);
   //qreal diff2 = qAbs(profile.at(idx).value - profile.at(idx + 2).value);
   //qreal diff3 = qAbs(profile.at(idx + 1).value - profile.at(idx + 2).value);
   //qreal max_off = diff1;
   //if (diff2 > max_off)
   //   max_off = diff2;
   //if (diff3 > max_off)
   //   max_off = diff3;

   //if (max_off > 0.18)
   //   return false;
   
   return true;
}

bool ImgUtils::isRightTwoProfile(const QVector<ProfileItem> & profile)
{
//"2c" "" 
//false 0.555556 
//true 0.222222 
//false 0.166667 
// or
//true 0.0769231 
//false 0.384615 
//true 0.384615 
//false 0.0769231 
   if (profile.count() == 4)
   {
      if ((profile.at(0).item == true)    &&
          (profile.at(1).item == false) &&
          (profile.at(2).item == true) && 
          (profile.at(3).item == false))
      {
         if ((profile.at(1).value > profile.at(0).value) && 
             (profile.at(2).value > profile.at(3).value))
             return true;
      }
   }

   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx).value >= 0.40) && 
       (profile.at(idx + 1).value < 0.35) &&
       (profile.at(idx + 2).value < 0.20))
       return true;
      
   return false;
}

bool ImgUtils::isLeftOneProfile(const QVector<ProfileItem> & profile)
{
/*
profile = [3](
{item=true value=0.11111111111111110 },
{item=false value=0.33333333333333331 },
{item=true value=0.55555555555555558 })
*/
   if (profile.count() < 3)
      return false;
   if (profile.count() == 3)
   {
      if ((profile.at(0).item == true)     &&
          (profile.at(1).item == false)  &&
          (profile.at(2).item == true))
      {
         if (profile.at(2).value > 0.5)
             return true;
      }
   }
   return false;
}

bool ImgUtils::isRightOneProfile(const QVector<ProfileItem> & profile)
{
/*
profile = [1](
{item=false value=1.0000000000000000 })
*/
   if (profile.count() == 1 && profile.at(0).value >= 0.99)
      return true;
   return false;
}

bool ImgUtils::isLeftSevenProfile(const QVector<ProfileItem> & profile)
{
/*
profile = [3](
{item=false value=0.11111111111111110 },
{item=true value=0.33333333333333331 },
{item=false value=0.55555555555555558 })
*/
   if (profile.count() < 3)
      return false;
   if (profile.count() == 3)
   {
      if ((profile.at(0).item == false)     &&
          (profile.at(1).item == true)  &&
          (profile.at(2).item == false))
      {
         if (profile.at(2).value > 0.5)
             return true;
      }
   }
   return false;
}

bool ImgUtils::isRightSevenProfile(const QVector<ProfileItem> & profile)
{
/*
profile = [2](
{item=false value=0.44444444444444442 },
{item=true value=0.55555555555555558 })
*/
   if (profile.count() != 2)
      return false;
   if ((profile.at(0).item == false)     &&
       (profile.at(1).item == true))
   {
      if (profile.at(0).value > 0.4 &&
          profile.at(1).value > 0.5)
          return true;
   }
   return false;
}

