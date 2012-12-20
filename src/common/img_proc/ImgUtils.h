#ifndef IMGUTILS_H
#define IMGUTILS_H

#include "BoolMatrix.h"

class ImgUtils
{
public:
   ImgUtils() {}
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
   //! Замкнутые области
   static QList<int> closedAreas(const BoolMatrix & bmatr_in);
   //! Возвращает цифру
   static qreal parseDigit(const BoolMatrix & bm);
   //! Проверка на точку
   static bool isDot(const BoolMatrix & bm);
protected:
   //! 
   static void fillMatrix(BoolMatrix & bm, int x, int y);
};

#endif
