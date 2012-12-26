#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "BoolMatrix.h"

typedef QList<QPoint> PointList;

struct ProfileItem
{
   //отклонение
   bool item;
   //величина выраженная в процентах ко всей области
   qreal value;
};

class Parser
{
friend class ImgUtils;
protected:
   //! Профиль шестерки
   virtual bool isSixProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Правый профиль тройки
   virtual bool isRightThreeProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Левый профиль тройки
   virtual bool isLeftThreeProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Левый профиль пятерки
   virtual bool isLeftFiveProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Правый профиль пятерки
   virtual bool isRightFiveProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Левый профиль двойки
   virtual bool isLeftTwoProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Правый профиль двойки
   virtual bool isRightTwoProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Левый профиль единички
   virtual bool isLeftOneProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Правый профиль единички
   virtual bool isRightOneProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Левый профиль семерки
   virtual bool isLeftSevenProfile(const QVector<ProfileItem> & profile) const = 0;
   //! Правый профиль семерки
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
   //! Переводит изображение в черно-белое по значению серого
   static void toBlackWhite(QImage & img, uchar threshold = 240);
   //! Переводит изображение в черно-белое по maxHue
   static void toBlackWhiteByHue(QImage & img);
   //! Переводит изображение в ч/б по диапазону Hue
   static void toBlackWhiteByHue(QImage & img, int minHue, int maxHue);
   //! Переводит изображение в черно-белое по значению среднего серого
   static void toBlackWhiteMid(QImage & img, const int threshold);
   //! Убирает шум
   static void denoise(QImage & img, uchar threshold);
   //! Подсчитывает пиксели заданного оттенка
   static int hueCount(const QImage & img, int minHue, int maxHue, 
                       int fromw, int fromh, int thr, QPoint & pt);
   //! Поиск максимального значения оттенка
   static int maxHue(const QImage & img);
   //! Подсчет букв в изображении
   static int countCheckLetters(const QImage & img);
   //! Подсчет букв в матрице
   static int countCheckLetters(const BoolMatrix & checkMatrix);
   //! Разделение по буквам
   static QList<BoolMatrix> splitByLetters(const BoolMatrix & matrix);

   // Функции распознавания цифры
   //! Парсит вещественное число
   static qreal parseRealNumber(const QList<BoolMatrix> & list, 
      const Parser * parser);

   //! Замкнутые области 4-х, 8-ми связность
   static QList<PointList> closedAreas(const BoolMatrix & bmatr_in,
      Coherence c = Eight);

protected:
   //! Обрезать снизу и сверху
   static BoolMatrix cropTopBottom(const BoolMatrix & bm);
   //! Парсит одну цифру
   static qreal parseDigit(const BoolMatrix & bm, const Parser * parser);
   //! Проверка на точку
   static bool isDot(const BoolMatrix & bm);
   //! Проверка на крест
   static bool isCrux(const BoolMatrix & bm, const int sx, const int sy);
   //! Сканирование левого профиля
   static QVector<ProfileItem> scanLeftProfile(const BoolMatrix & imgMatrix);
   //! Сканирование правого профиля
   static QVector<ProfileItem> scanRightProfile(const BoolMatrix & imgMatrix);
   //! Сканирование нижнего профиля
   static QVector<ProfileItem> scanDownProfile(const BoolMatrix & imgMatrix);
};

#endif
