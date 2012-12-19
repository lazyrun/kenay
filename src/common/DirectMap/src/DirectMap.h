/*! \file DirectMap.h
    \date 15.05.2009
    \brief Реализация "Прямого" QMap, не сортирующего по ключу
    \author Kiselev Kirill
*/

#ifndef DIRECTMAP_H
#define DIRECTMAP_H

/*! \class DirectMapIterator
    \brief DirectMapIterator
*/
class DirectMapIterator;

#define Q_DECLARE_METATYPE_COMMA(...)                                         \
   QT_BEGIN_NAMESPACE                                                         \
   template <>                                                                \
   struct QMetaTypeId< __VA_ARGS__ >                                          \
   {                                                                          \
      enum { Defined = 1 };                                                   \
      static int qt_metatype_id()                                             \
      {                                                                       \
          static QBasicAtomicInt metatype_id = Q_BASIC_ATOMIC_INITIALIZER(0); \
          if (!metatype_id)                                                   \
              metatype_id = qRegisterMetaType< __VA_ARGS__ >( #__VA_ARGS__,   \
                            reinterpret_cast< __VA_ARGS__ *>(quintptr(0)));   \
          return metatype_id;                                                 \
      }                                                                       \
  };                                                                          \
  QT_END_NAMESPACE                                                            \

/*! \class QVector
    \brief QVector
*/
/*! \class DirectMap
   \brief Шаблонный класс прямого QMap
*/
template <class Key, class Value>
class DirectMap : public QVector<QPair<Key, Value> >
{
   //! Дружественный класс
   friend class DirectMapIterator;
public:
   //! Определение вектора из пар
   typedef QVector<QPair<Key, Value> > VectorPair;
   //! Конструктор
   DirectMap();
   //! Копирующий конструктор
   /*!
   \param vec Копия
   */
   DirectMap(const VectorPair & vec);
   //! Добавляем пару
   /*!
   \param key Ключ
   \param value Значение
   */
   void append(const Key & key, const Value & value);
   //! Список ключей
   /*!
   \return Список ключей
   */
   QVector<Key> keys() const;
   //! Список значений
   /*!
   \return Список значений
   */
   QVector<Value> values() const;
   //! Список значений, соответствующих ключу
   /*!
   \param key Ключ
   \return Список значений
   */
   QVector<Value> values(const Key & key) const;

   //! Количество строк
   /*!
   \return Количество строк
   */
   int count() const
   {
      return VectorPair::count();
   }
   //! Проверяет содержит ли список значение с определенным ключом
   /*!
   \param key Ключ
   \return Признак
   */
   bool contains(const Key & key) const;
   //! Возвратить ключ определенной строки
   /*!
   \param row Номер строки
   \return Ключ
   */
   Key key(int row) const; 
   //! Возвратить значение определенной строки
   /*!
   \param row Номер строки
   \return Значение
   */
   Value value(int row) const;
   //! Возвратить значение по ключу
   /*!
   \param key Ключ
   \return Значение
   */
   const Value value(Key key) const;

   //! Возвратить значение по ключу
   /*!
   \param key Ключ
   \return Значение
   */
   const Value operator[](const Key & key) const;
   //! Возвратить ссылку на значение, соответствующее ключу
   /*!
   \param key Ключ
   \return Значение
   */
   Value & operator[](const Key & key);

   Value & operator[](int idx);
   //! Склеить текущий список с другим
   /*!
   \param right Другой список
   */
   void operator<<(const DirectMap<Key, Value> & right);
   //!
   QPair<Key, Value> & item(int idx); 
   //!
   const QPair<Key, Value> & item(int idx) const; 
   
   QMap<Key, Value> toMap() const;
   //DirectMap<Key, Value> & operator=(const DirectMap<Key, Value> &other);
   static DirectMap<Key, Value> fromMap(const QMap<Key, Value> & map);
};

template <class Key, class Value>
DirectMap<Key, Value>::DirectMap()
: QVector<QPair<Key, Value> >()
{
}

template <class Key, class Value>
DirectMap<Key, Value>::DirectMap(const QVector<QPair<Key, Value> > & vec)
: QVector<QPair<Key, Value> >(vec)
{
}

template <class Key, class Value>
void DirectMap<Key, Value>::append(const Key & key, const Value & value)
{
   VectorPair::append(qMakePair(key, value));
}

template <class Key, class Value>
QVector<Key> DirectMap<Key, Value>::keys() const
{
   QVector<Key> resKeys;
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      resKeys.append(VectorPair::at(i).first);
   }
   return resKeys;
}

template <class Key, class Value>
QVector<Value> DirectMap<Key, Value>::values() const
{
   QVector<Value> resValues;
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      resValues.append(VectorPair::at(i).second);
   }
   return resValues;
}

template <class Key, class Value>
QVector<Value> DirectMap<Key, Value>::values(const Key & key) const
{
   QVector<Value> resValues;
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      if (VectorPair::at(i).first == key)
         resValues.append(VectorPair::at(i).second);
   }
   return resValues;
}

template <class Key, class Value>
const Value DirectMap<Key, Value>::operator[](const Key & key) const
{
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      if (VectorPair::at(i).first == key)
      {
         return VectorPair::at(i).second;
      }
   }
   return Value();
}

template <class Key, class Value>
bool DirectMap<Key, Value>::contains(const Key & key) const
{
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      if (VectorPair::at(i).first == key)
      {
         return true;
      }
   }
   return false;
}

template <class Key, class Value>
Value & DirectMap<Key, Value>::operator[](const Key & key)
{
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      if (VectorPair::at(i).first == key)
      {
         return VectorPair::operator[](i).second;
      }
   }

   append(key, Value());
   return VectorPair::last().second;
}

template <class Key, class Value>
Value & DirectMap<Key, Value>::operator[](int idx)
{
   return VectorPair::operator[](idx).second;
}

template <class Key, class Value>
void DirectMap<Key, Value>::operator<<(const DirectMap<Key, Value> & right)
{
   foreach (QString key, right.keys())
   {
      append(key, right[key]);
   }
}

template <class Key, class Value>
Key DirectMap<Key, Value>::key(int row) const
{
   QPair<Key, Value> pair = VectorPair::at(row);
   return pair.first;
}

template <class Key, class Value>
Value DirectMap<Key, Value>::value(int row) const
{
   QPair<Key, Value> pair = VectorPair::at(row);
   return pair.second;
}

template <class Key, class Value>
const Value DirectMap<Key, Value>::value(Key key) const
{
   return operator[](key);
}

template <class Key, class Value>
QPair<Key, Value> & DirectMap<Key, Value>::item(int idx)
{
   return VectorPair::operator[](idx);
}

template <class Key, class Value>
const QPair<Key, Value> & DirectMap<Key, Value>::item(int idx) const
{
   return VectorPair::at(idx);
}

template <class Key, class Value>
QMap<Key, Value> DirectMap<Key, Value>::toMap() const
{
   QMap<Key, Value> map;
   for (int i = 0; i < VectorPair::count(); ++i)
   {
      QPair<Key, Value> pair = item(i);
      QVariant value = QVariant::fromValue(pair.second);
      if (value.canConvert<DirectMap<Key, Value> >())
      {
         DirectMap<Key, Value> submap = qvariant_cast<DirectMap<Key, Value> >(value);
         map.insert(pair.first, submap.toMap());
      }
      else
      {
         map.insert(pair.first, pair.second);
      }
   }

   return map;
}

template <class Key, class Value>
DirectMap<Key, Value> DirectMap<Key, Value>::fromMap(const QMap<Key, Value> & map)
{
   DirectMap<Key, Value> dmap;
   foreach (Key key, map.keys())
   {
      Value value = map.value(key);
      dmap.append(key, value);
   }
   return dmap;
}

/*! \class QVectorIterator
    \brief QVectorIterator
*/
/*! \class DirectIterator
    \brief Итератор класса DirectMap
*/
template <class Key, class Value>
class DirectIterator : private QVectorIterator<QPair<Key, Value> >
{
public:
   //! Копирующий конструктор
   /*!
   \param map Копируемое значение
   */
   DirectIterator(const QVector<QPair<Key, Value> > & map);
   //! Копирующий конструктор
   /*!
   \param map Копируемое значение
   */
   DirectIterator(const DirectMap<Key, Value>  & map);
   //! Проверка на наличие следующего значения в списке
   /*!
   \return Наличие следующего значения в списке
   */
   bool hasNext() const;
   //! Переход к следующему значению в списке
   void next();
   //! Текущий ключ
   /*!
   \return Ключ
   */
   Key key() const;
   //! Текущее значение
   /*!
   \return Значение
   */
   Value value() const;
protected:
   //! Текущая пара ключ - значение
   QPair<Key, Value> current_;
};

template <class Key, class Value>
DirectIterator<Key, Value>::DirectIterator(const QVector<QPair<Key, Value> >  & map)
: QVectorIterator<QPair<Key, Value> >(map)
{
}

template <class Key, class Value>
DirectIterator<Key, Value>::DirectIterator(const DirectMap<Key, Value>  & map)
: QVectorIterator<QPair<Key, Value> >((QVector<QPair<Key, Value> > &)map)
{
   //QVectorIterator<QPair<Key, Value> >(()map)
}

template <class Key, class Value>
bool DirectIterator<Key, Value>::hasNext() const
{
   return QVectorIterator<QPair<Key, Value> >::hasNext();
}

template <class Key, class Value>
void DirectIterator<Key, Value>::next()
{
   current_ = QVectorIterator<QPair<Key, Value> >::next();
};

template <class Key, class Value>
Key DirectIterator<Key, Value>::key() const
{
   return current_.first;
};

template <class Key, class Value>
Value DirectIterator<Key, Value>::value() const
{
   return current_.second;
};

typedef QPair<QString, QVariant> VariantPair;
//! Определение DirectVariantMap
typedef DirectMap<QString, QVariant> DirectVariantMap; 
//! Определение DirectStringMap
typedef DirectMap<QString, QString> DirectStringMap;
//! Определение DirectVariantIterator
typedef DirectIterator<QString, QVariant> DirectVariantIterator;

Q_DECLARE_METATYPE_COMMA(QVector<QPair<QString, QVariant> >)
Q_DECLARE_METATYPE(DirectVariantMap)

Q_DECLARE_METATYPE_COMMA(QVector<QPair<QString, QString> >)
Q_DECLARE_METATYPE_COMMA(DirectStringMap)

#endif
