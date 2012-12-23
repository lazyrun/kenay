#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "BoolMatrix.h"

typedef QList<QPoint> PointList;

struct ProfileItem
{
   //����������
   bool item;
   //�������� ���������� � ��������� �� ���� �������
   qreal value;
};

class ImgUtils
{
public:
   ImgUtils() {}
   enum Coherence
   {
      Eight,
      Four
   };
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
   //! ��������� ������� 8-�� ���������
   static QList<PointList> closedAreas(const BoolMatrix & bmatr_in,
      Coherence c = Eight);

   //! ���������� �����
   static qreal parseDigit(const BoolMatrix & bm);
   //! �������� �� �����
   static bool isDot(const BoolMatrix & bm);
   //! �������� ����� � ������
   static BoolMatrix cropTopBottom(const BoolMatrix & bm);
protected:
   //! �������� �� �����
   static bool isCrux(const BoolMatrix & bm, const int sx, const int sy);
   //! ������������ ������ �������
   static QVector<ProfileItem> scanLeftProfile(const BoolMatrix & imgMatrix);
   //! ������������ ������� �������
   static QVector<ProfileItem> scanRightProfile(const BoolMatrix & imgMatrix);
   //! ������������ ������� �������
   static QVector<ProfileItem> scanDownProfile(const BoolMatrix & imgMatrix);
   //! ������� ��������
   static bool isSixProfile(const QVector<ProfileItem> & profile);
   //! ������ ������� ������
   static bool isRightThreeProfile(const QVector<ProfileItem> & profile);
   //! ����� ������� ������
   static bool isLeftThreeProfile(const QVector<ProfileItem> & profile);
   //! ����� ������� �������
   static bool isLeftFiveProfile(const QVector<ProfileItem> & profile);
   //! ������ ������� �������
   static bool isRightFiveProfile(const QVector<ProfileItem> & profile);
   //! ����� ������� ������
   static bool isLeftTwoProfile(const QVector<ProfileItem> & profile);
   //! ������ ������� ������
   static bool isRightTwoProfile(const QVector<ProfileItem> & profile);
   //! ����� ������� ��������
   static bool isLeftOneProfile(const QVector<ProfileItem> & profile);
   //! ������ ������� ��������
   static bool isRightOneProfile(const QVector<ProfileItem> & profile);
   //! ����� ������� �������
   static bool isLeftSevenProfile(const QVector<ProfileItem> & profile);
   //! ������ ������� �������
   static bool isRightSevenProfile(const QVector<ProfileItem> & profile);
};

#endif
