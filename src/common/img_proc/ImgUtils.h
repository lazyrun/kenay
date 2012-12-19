#ifndef IMGUTILS_H
#define IMGUTILS_H

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
//!
static int countCheckLetters(const QImage & img);

#endif
