/*! \file Config.h
    \brief Работа с конфигурационными файлами
    \author Kiselev Kirill
    \date 07.11.2012
*/
#ifndef CONFIG_H
#define CONFIG_H

//#include <QtXml>
#include "DirectMap.h"

/*! 
\class Settings 
\brief Класс настроек
*/
class Settings
{
public:
   //! Конструктор
   /*!
   \param fileName Имя файла
   \param rootName Имя корневого тега
   */
   Settings(const QString & fileName = QString(), 
      const QString & rootName = QString());
   //! Получить значение тега из конфигурационного файла
   /*!
   \param root Корневое значение тега в файле конфигурации
   \return Значение тега
   */
   QVariant settingValue(const char * root, ...) const;
   //! Проверить наличие тега
   /*!
   \param root Корневое значение тега в файле конфигурации
   \return Наличие тега
   */
   bool hasTag(const char * root, ...) const;
   //! Получить значение атрибута
   /*!
   \param attr Название атрибута
   \param root Корневое значение тега в файле конфигурации
   */
   QString settingAttribute(const QString & attr, const char * root, ...) const;
   //! Получить список
   QList<DirectVariantMap> settingMultiValue(const char * root, ...) const;
   
   QStringList Settings::settingMultiMap(const char * root, ...) const;

   //! Получить массив свойств
   /*!
   \return Массив свойств
   */
   DirectVariantMap settingsMap() const { return settingsMap_; }
   
   void appendMap(const DirectVariantMap & map);
   
   //! Установить свойства
   /*!
   \param smap Массив свойств
   */
   void setSettingsMap(const DirectVariantMap & smap) { settingsMap_ = smap; }

   //! Сохранить конфигурацию как
   /*!
   \param filename Имя файла
   \return Результат сохранения
   */
   bool save(const QString & filename = QString());
   //! Возвращает название root-элемента
   /*!
   \return Название root-элемента
   */
   QString rootName() const { return rootName_; }
   //! Возвращает имя файла настроек
   /*!
   \return Имя файла настроек
   */
   QString fileName() const { return fileName_; }

   QDomElement & root() { return rootElement_; }
protected: 
   //! Инициализация
   void init(const QString & filename = "", const QString & rootName = "" );
   //! Записывает массив настроек в документ
   void saveTag(QDomDocument * domDocument, QDomElement * parent,
                  const DirectVariantMap & map);

   //! Загрузить настройки из конфигурационного файла
   /*!
   \param filename Имя файла
   */
   void loadSettings(const QString & filename);

   //! Преобразовать DOM документ в массив свойств
   DirectVariantMap parseXml(const QDomElement & domElem);
private:   
   //! DOM документ
   QDomDocument document_;
   //! Рутовый элемент
   QDomElement rootElement_;
   //! Массив свойств
   DirectVariantMap settingsMap_;
   //! Название root-элемента
   QString rootName_;
   //! Имя файла настроек
   QString fileName_;
};

/*!
\class ConfigGlobal 
\brief Класс глобальных настроек
*/
//! Основные настройки
enum MainConfig;
/*
Нужно 2 разных синглетона с одной и той же функциональностью.
Для уменьшения copy/paste сделано как шаблон с фиктивным enum.
*/
template <class T>
class ConfigGlobal : public Settings
{
public:
   //! Обеспечивает доступ к экземпляру класса
   /*!
   \param[in] filename Имя файла
   \param[in] rootName Название root-элемента
   */
   static ConfigGlobal<T>& Instance(const QString & filename = "",
      const QString & rootName = "")
   {
      static ConfigGlobal<T> Instance_(filename, rootName);
      return Instance_;
   }

private:
   //! Конструктор по умолчанию
   /*!
   \param[in] filename Имя файла
   \param[in] rootName Название root-элемента
   */
   ConfigGlobal<T>(const QString & filename = "", const QString & rootName = "")
      : Settings(filename, rootName)
   {
      init(filename, rootName);
   }
};

#endif


