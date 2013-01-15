/*! \file Config.h
    \brief ������ � ����������������� �������
    \author Kiselev Kirill
    \date 07.11.2012
*/
#ifndef CONFIG_H
#define CONFIG_H

//#include <QtXml>
#include "DirectMap.h"

/*! 
\class Settings 
\brief ����� ��������
*/
class Settings
{
public:
   //! �����������
   /*!
   \param fileName ��� �����
   \param rootName ��� ��������� ����
   */
   Settings(const QString & fileName = QString(), 
      const QString & rootName = QString());
   //! �������� �������� ���� �� ����������������� �����
   /*!
   \param root �������� �������� ���� � ����� ������������
   \return �������� ����
   */
   QVariant settingValue(const char * root, ...) const;
   //! ��������� ������� ����
   /*!
   \param root �������� �������� ���� � ����� ������������
   \return ������� ����
   */
   bool hasTag(const char * root, ...) const;
   //! �������� �������� ��������
   /*!
   \param attr �������� ��������
   \param root �������� �������� ���� � ����� ������������
   */
   QString settingAttribute(const QString & attr, const char * root, ...) const;
   //! �������� ������
   QList<DirectVariantMap> settingMultiValue(const char * root, ...) const;
   
   QStringList Settings::settingMultiMap(const char * root, ...) const;

   //! �������� ������ �������
   /*!
   \return ������ �������
   */
   DirectVariantMap settingsMap() const { return settingsMap_; }
   
   void appendMap(const DirectVariantMap & map);
   
   //! ���������� ��������
   /*!
   \param smap ������ �������
   */
   void setSettingsMap(const DirectVariantMap & smap) { settingsMap_ = smap; }

   //! ��������� ������������ ���
   /*!
   \param filename ��� �����
   \return ��������� ����������
   */
   bool save(const QString & filename = QString());
   //! ���������� �������� root-��������
   /*!
   \return �������� root-��������
   */
   QString rootName() const { return rootName_; }
   //! ���������� ��� ����� ��������
   /*!
   \return ��� ����� ��������
   */
   QString fileName() const { return fileName_; }

   QDomElement & root() { return rootElement_; }
protected: 
   //! �������������
   void init(const QString & filename = "", const QString & rootName = "" );
   //! ���������� ������ �������� � ��������
   void saveTag(QDomDocument * domDocument, QDomElement * parent,
                  const DirectVariantMap & map);

   //! ��������� ��������� �� ����������������� �����
   /*!
   \param filename ��� �����
   */
   void loadSettings(const QString & filename);

   //! ������������� DOM �������� � ������ �������
   DirectVariantMap parseXml(const QDomElement & domElem);
private:   
   //! DOM ��������
   QDomDocument document_;
   //! ������� �������
   QDomElement rootElement_;
   //! ������ �������
   DirectVariantMap settingsMap_;
   //! �������� root-��������
   QString rootName_;
   //! ��� ����� ��������
   QString fileName_;
};

/*!
\class ConfigGlobal 
\brief ����� ���������� ��������
*/
//! �������� ���������
enum MainConfig;
/*
����� 2 ������ ���������� � ����� � ��� �� �����������������.
��� ���������� copy/paste ������� ��� ������ � ��������� enum.
*/
template <class T>
class ConfigGlobal : public Settings
{
public:
   //! ������������ ������ � ���������� ������
   /*!
   \param[in] filename ��� �����
   \param[in] rootName �������� root-��������
   */
   static ConfigGlobal<T>& Instance(const QString & filename = "",
      const QString & rootName = "")
   {
      static ConfigGlobal<T> Instance_(filename, rootName);
      return Instance_;
   }

private:
   //! ����������� �� ���������
   /*!
   \param[in] filename ��� �����
   \param[in] rootName �������� root-��������
   */
   ConfigGlobal<T>(const QString & filename = "", const QString & rootName = "")
      : Settings(filename, rootName)
   {
      init(filename, rootName);
   }
};

#endif


