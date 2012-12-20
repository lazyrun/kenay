#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include "BoolMatrix.h"

/*!
\class CardProcessing
\brief Абстрактный класс обработки скрина стола
*/
class CardProcessing
{
public:
   //! Текущий уровень игры
   enum HoldemLevel
   {
      Undefined,
      Preflop,
      Flop,
      Turn,
      River
   };
   //! ctor.
   CardProcessing();
   //! dtor.
   virtual ~CardProcessing();
   //! Задасет скрин стола
   virtual void setImage(const QImage & img);
   //! Проверяет на стадию префлопа
   bool isPreflop() const {return holdemLevel_ == Preflop; }
   //! Проверяет на стадию флопа
   bool isFlop() const {return holdemLevel_ == Flop; }
   //! Проверяет на стадию терна
   bool isTurn() const {return holdemLevel_ == Turn; }
   //! Проверяет на стадию ривера
   bool isRiver() const {return holdemLevel_ == River; }
   //! Возвращает текущую стадию
   HoldemLevel holdemLevel() const;
   //! Возвращает порог фильтрации
   uchar threshold() const {return threshold_; }

   virtual bool hasFold() const = 0;
   virtual bool hasCall() const = 0;
   virtual bool hasCheck() const = 0;
   virtual bool hasRaise() const = 0;
   virtual QString holeCard(const QString & card) const = 0;
   virtual qreal pot() const = 0;
   virtual qreal stack() const = 0;

protected:
   //! проверка цвета пикселя
   bool controlPixel(const QString & sx, const QString & sy, 
                            const QString & scl) const;
protected:
   //! оригинальная картинка
   QImage img_;
   //! булева матрица
   BoolMatrix * matrix_;
   //! порог перевода в ч/б
   uchar threshold_;
   //! текущий уровень
   HoldemLevel holdemLevel_;
};

#endif
