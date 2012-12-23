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
   //! Замкнутые области 8-ми связность
   static QList<PointList> closedAreas(const BoolMatrix & bmatr_in,
      Coherence c = Eight);

   //! Возвращает цифру
   static qreal parseDigit(const BoolMatrix & bm);
   //! Проверка на точку
   static bool isDot(const BoolMatrix & bm);
   //! Обрезать снизу и сверху
   static BoolMatrix cropTopBottom(const BoolMatrix & bm);
protected:
   //! Проверка на крест
   static bool isCrux(const BoolMatrix & bm, const int sx, const int sy);
   //! Сканирование левого профиля
   static QVector<ProfileItem> scanLeftProfile(const BoolMatrix & imgMatrix);
   //! Сканирование правого профиля
   static QVector<ProfileItem> scanRightProfile(const BoolMatrix & imgMatrix);
   //! Сканирование нижнего профиля
   static QVector<ProfileItem> scanDownProfile(const BoolMatrix & imgMatrix);
   //! Профиль шестерки
   static bool isSixProfile(const QVector<ProfileItem> & profile);
   //! Правый профиль тройки
   static bool isRightThreeProfile(const QVector<ProfileItem> & profile);
   //! Левый профиль тройки
   static bool isLeftThreeProfile(const QVector<ProfileItem> & profile);
   //! Левый профиль пятерки
   static bool isLeftFiveProfile(const QVector<ProfileItem> & profile);
   //! Правый профиль пятерки
   static bool isRightFiveProfile(const QVector<ProfileItem> & profile);
   //! Левый профиль двойки
   static bool isLeftTwoProfile(const QVector<ProfileItem> & profile);
   //! Правый профиль двойки
   static bool isRightTwoProfile(const QVector<ProfileItem> & profile);
   //! Левый профиль единички
   static bool isLeftOneProfile(const QVector<ProfileItem> & profile);
   //! Правый профиль единички
   static bool isRightOneProfile(const QVector<ProfileItem> & profile);
   //! Левый профиль семерки
   static bool isLeftSevenProfile(const QVector<ProfileItem> & profile);
   //! Правый профиль семерки
   static bool isRightSevenProfile(const QVector<ProfileItem> & profile);
};

#endif
