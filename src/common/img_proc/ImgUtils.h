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

class Parser
{
friend class ImgUtils;
protected:
   //! ������� ��������
   virtual bool isSixProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ������ ������� ������
   virtual bool isRightThreeProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ����� ������� ������
   virtual bool isLeftThreeProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ����� ������� �������
   virtual bool isLeftFiveProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ������ ������� �������
   virtual bool isRightFiveProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ����� ������� ������
   virtual bool isLeftTwoProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ������ ������� ������
   virtual bool isRightTwoProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ����� ������� ��������
   virtual bool isLeftOneProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ������ ������� ��������
   virtual bool isRightOneProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ����� ������� �������
   virtual bool isLeftSevenProfile(const QVector<ProfileItem> & profile) const = 0;
   //! ������ ������� �������
   virtual bool isRightSevenProfile(const QVector<ProfileItem> & profile) const = 0;
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
   //! ������ ������������ �����
   static qreal parseRealNumber(const QList<BoolMatrix> & list, 
      const Parser * parser);

   //! ��������� ������� 4-�, 8-�� ���������
   static QList<PointList> closedAreas(const BoolMatrix & bmatr_in,
      Coherence c = Eight);

protected:
   //! �������� ����� � ������
   static BoolMatrix cropTopBottom(const BoolMatrix & bm);
   //! ������ ���� �����
   static qreal parseDigit(const BoolMatrix & bm, const Parser * parser);
   //! �������� �� �����
   static bool isDot(const BoolMatrix & bm);
   //! �������� �� �����
   static bool isCrux(const BoolMatrix & bm, const int sx, const int sy);
   //! ������������ ������ �������
   static QVector<ProfileItem> scanLeftProfile(const BoolMatrix & imgMatrix);
   //! ������������ ������� �������
   static QVector<ProfileItem> scanRightProfile(const BoolMatrix & imgMatrix);
   //! ������������ ������� �������
   static QVector<ProfileItem> scanDownProfile(const BoolMatrix & imgMatrix);
};

#endif
