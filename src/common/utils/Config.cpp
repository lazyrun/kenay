/*! \file Config.cpp
    \brief Работа с конфигурационными файлами
    \author Kiselev Kirill
    \date 07.11.2012
*/
#include "Config.h"

Settings::Settings(const QString & filename, const QString & rootName)
{
   init(filename, rootName);
}

void Settings::init(const QString & filename, const QString & rootName)
{
   rootName_ = rootName;
   if (rootName_.isEmpty())
      rootName_ = "root";

   fileName_ = filename;
   
   loadSettings(filename);
}

void Settings::appendMap(const DirectVariantMap & map)
{
   saveTag(&document_, &rootElement_, map);
}

void Settings::saveTag(QDomDocument * domDocument, QDomElement * parent,
                       const DirectVariantMap & map)
{
   //рекурсивно сохраняем теги
   //сохраняем значения свойств
   QVectorIterator<VariantPair> vi(map);
   //QMapIterator<QString, QVariant> si(map);
   while (vi.hasNext())
   {
      VariantPair vp = vi.next();
      QString sTag = vp.first;
      QVariant sValue = vp.second;
      
      QDomElement tag = domDocument->createElement(sTag);
      parent->appendChild(tag);

      if (sValue.canConvert<DirectVariantMap>())
      {
         DirectVariantMap smap = qvariant_cast<DirectVariantMap>(sValue);
         saveTag(domDocument, &tag, smap);
      }
      else if (sValue.canConvert<QVariantMap>())
      {
         QVariantMap map = qvariant_cast<QVariantMap>(sValue);
         DirectVariantMap dMap = DirectVariantMap::fromMap(map);
         saveTag(domDocument, &tag, dMap);
      }
      else
      {
         //вставить текстовое значение
         QDomText value = domDocument->createTextNode(sValue.toString());
         tag.appendChild(value);
      }
   }
}

bool Settings::save(const QString & fname)
{
   if (fname.isEmpty() && fileName_.isEmpty())
      return false;

   QString filename = fname;
   if (filename.isEmpty())
      filename = fileName_;

   //сохраняем в xml
   QDomDocument domDocument;
	QDomProcessingInstruction instr =
      domDocument.createProcessingInstruction("xml",
      "version=\"1.0\" encoding=\"UTF-8\"");
	domDocument.appendChild(instr);

   //вставим рутовый элемент
   QDomElement root = domDocument.createElement(rootName_);
   domDocument.appendChild(root);

   //в цикле вставим свойства
   //QMapIterator<QString, QVariant> mi(settingsMap_);
   QVectorIterator<VariantPair> vi(settingsMap_);
   while (vi.hasNext())
   {
      VariantPair vp = vi.next();
      QString sName = vp.first;
      QVariant val = vp.second;

      //сохраняем новый тег
      QDomElement tagPage = domDocument.createElement(sName);
      root.appendChild(tagPage);

      if (val.canConvert<DirectVariantMap>())
      {
         DirectVariantMap varMap = qvariant_cast<DirectVariantMap>(val);
         //рекурсивно сохраняем теги
         saveTag(&domDocument, &tagPage, varMap);
      }
      else if (val.canConvert<QVariantMap>())
      {
         QVariantMap varMap = qvariant_cast<QVariantMap>(val);
         DirectVariantMap dMap = DirectVariantMap::fromMap(varMap);
         //рекурсивно сохраняем теги
         saveTag(&domDocument, &tagPage, dMap);
      }
      else
      {
         //вставить текстовое значение
         QDomText value = domDocument.createTextNode(val.toString());
         tagPage.appendChild(value);
      }
   }

   //сохраняем
   const int IndentSize = 4;
   
   QFile file(filename);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
      return false;
   
   QTextStream stream( &file );
   stream.setCodec("UTF-8");
   
   domDocument.save(stream, IndentSize);

   file.close();

   return true;
}

void Settings::loadSettings(const QString & filename)
{
   //читаем данные из файла и забиваем если есть
   QFile file(filename);
   if (file.open(QIODevice::ReadOnly))
   {
      //QDomDocument document;
      if (document_.setContent(&file)) 
      {
         //перевести документ в qvariantmap
         rootElement_ = document_.firstChildElement(rootName_);
         settingsMap_ = parseXml(rootElement_);
      }
      file.close();
   }
}

DirectVariantMap Settings::parseXml(const QDomElement & domElem)
{
   DirectVariantMap vmap;
   if (!domElem.isNull())
   {
      if (domElem.hasChildNodes())
      {
         for (QDomNode n = domElem.firstChild(); !n.isNull(); n = n.nextSibling())
         {
            QString sTagName = n.nodeName();
            if (n.firstChild().nodeType() == QDomNode::TextNode )
            {
               vmap[sTagName] = n.firstChild().nodeValue();
            }
            else
            {
               DirectVariantMap subMap = parseXml(n.toElement());
               vmap.append(sTagName, QVariant::fromValue(subMap));
            }
         }
      }
   }

   return vmap;
}

//Извлечь свойство из settingsMap_
QVariant Settings::settingValue(const char * root, ...) const
{
   const char **cp = &root;
   
   DirectVariantMap map = settingsMap_;
   QString str = QString::fromAscii(*cp);
   QVariant res;
   while(!str.isEmpty())
   {
      res = map[str];
      map = qvariant_cast<DirectVariantMap>(map[str]);
      cp++;
      str = QString::fromAscii(*cp);
   }

   return res;
}

bool Settings::hasTag(const char * root, ...) const
{
   const char **cp = &root;
   
   DirectVariantMap map = settingsMap_;
   QString str = QString::fromAscii(*cp);
   QVariant res;
   while(!str.isEmpty())
   {
      res = map[str];
      map = qvariant_cast<DirectVariantMap>(map[str]);
      cp++;
      str = QString::fromAscii(*cp);
   }

   return !res.isNull();
}

QString Settings::settingAttribute(const QString & attr, const char * root, ...) const
{
   //собрать список тегов
   QStringList lstTags;
   const char **cp = &root;
   QString str = QString::fromAscii(*cp);
   while(!str.isEmpty())
   {
      lstTags.append(str);
      cp++;
      str = QString::fromAscii(*cp);
   }
   
   QDomElement elem = rootElement_.firstChildElement(lstTags.at(0));
   for (int i = 1; i < lstTags.count(); ++i)
   {
      elem = elem.firstChildElement(lstTags.at(i));
   }
   
   return elem.attribute(attr);
}

QList<DirectVariantMap> Settings::settingMultiValue(const char * root, ...) const
{
   QList<DirectVariantMap> map;

   const char **cp = &root;
   
   QString str = QString::fromAscii(*cp);
   QDomNodeList dnList = document_.elementsByTagName(str);
   QDomElement elem = dnList.at(0).toElement();
   cp++;
   str = QString::fromAscii(*cp);
   QString lastTag;
   while(!str.isEmpty())
   {
      //идем по тегам
      dnList = elem.elementsByTagName(str);
      elem = dnList.at(0).toElement();
      cp++;
      lastTag = str;
      str = QString::fromAscii(*cp);
   }
   //str - последний не пустой тег
   //dnList - хранит мультитег
   //заполняем мультимап
   for (int i=0; i < dnList.count(); ++i)
   {
      QDomNodeList dn = dnList.at(i).childNodes();
      DirectVariantMap vmap;
      for (int j = 0; j < dn.count(); ++j)
      {
         if (dn.at(j).nodeType() == QDomNode::CommentNode)
            continue;

         QString tag = dn.at(j).nodeName();
         QString val = dn.at(j).firstChild().nodeValue();
         vmap.append(tag, val);
      }
      map.append(vmap);
   }
   
   return map;
}

QStringList Settings::settingMultiMap(const char * root, ...) const
{
   QStringList lst;

   const char **cp = &root;
   
   QString str = QString::fromAscii(*cp);
   QDomNodeList dnList = document_.elementsByTagName(str);
   QDomElement elem = dnList.at(0).toElement();
   cp++;
   str = QString::fromAscii(*cp);
   QString lastTag;
   while(!str.isEmpty())
   {
      //идем по тегам
      dnList = elem.elementsByTagName(str);
      elem = dnList.at(0).toElement();
      cp++;
      lastTag = str;
      str = QString::fromAscii(*cp);
   }
   //str - последний не пустой тег
   //dnList - хранит мультитег
   //заполняем мультимап
   for (int i=0; i < dnList.count(); ++i)
   {
      QString val = dnList.at(i).firstChild().nodeValue();
      lst << val;
   }
   
   return lst;
}
