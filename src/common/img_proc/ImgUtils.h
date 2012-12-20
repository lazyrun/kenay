#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "BoolMatrix.h"

class ImgUtils
{
public:
   ImgUtils() {}
   //! ��������� ����������� � �����-����� �� �������� ������
   static void toBlackWhite(QImage & img, uchar threshold = 240);
   //! ��������� ����������� � �����-����� �� maxHue
   static void toBlackWhiteByHue(QImage & img);
   //! ��������� ����������� � �/� �� ��������� Hue
   static void toBlackWhiteByHue(QImage & img, int minHue, int maxHue);
   //! ��������� ����������� � �����-����� �� �������� �������� ������
   static void toBlackWhiteMid(QImage & img, const int threshold);
   //! ������� ���
   static void denoise(QImage & img, uchar threshold);
   //! ������������ ������� ��������� �������
   static int hueCount(const QImage & img, int minHue, int maxHue, 
                       int fromw, int fromh, int thr, QPoint & pt);
   //! ����� ������������� �������� �������
   static int maxHue(const QImage & img);
   //! ������� ���� � �����������
   static int countCheckLetters(const QImage & img);
   //! ������� ���� � �������
   static int countCheckLetters(const BoolMatrix & checkMatrix);
   //! ���������� �� ������
   static QList<BoolMatrix> splitByLetters(const BoolMatrix & matrix);

   // ������� ������������� �����
   //! ��������� �������
   static QList<int> closedAreas(const BoolMatrix & bmatr_in);
   //! ���������� �����
   static qreal parseDigit(const BoolMatrix & bm);
   //! �������� �� �����
   static bool isDot(const BoolMatrix & bm);
protected:
   //! 
   static void fillMatrix(BoolMatrix & bm, int x, int y);
};

#endif
