/*! \file GlobVars.h
    \brief Класс, содержащий глобальные переменные
*/

#ifndef GlobVarsH
#define GlobVarsH

#include <vector>

/*! 
\class CGlobal
\brief Singleton, содержащий глобальные переменные
*/
class CGlobal
{
public:
   //! Обеспечивает доступ к экземпляру класса
   /*!
   \param[in] canWrite Признак возможности писать
   \return Ссылка на глобальный экземпляр класса
   */
   static CGlobal& Instance()
   {
      static CGlobal Instance_;
      return Instance_;
   }
   //! Пишет строку в протокол при dbg_level_==0
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap0(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==1
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap1(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==2
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap2(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==3
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap3(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==4
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap4(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==5
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap5(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==6
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap6(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==7
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap7(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==8
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap8(const QString& msg);
   //! Пишет строку в протокол при dbg_level_==9
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap9(const QString& msg);
   //! Возвращает уровень отладки
   /*!
      \sa dbg_level_
   */
   int GetDbg(){return dbg_level_;}
   //! Устанавливает уровень отладки
   /*!
      \sa dbg_level_
   */
   void SetDbg(int);
   //! Устанавливает размер файла протокола
   /*!
      \sa proto_size_
   */
   void SetProtoSize(int);
   //! Возвращает каталог запуска приложения
   /*!
      \sa workDir_
   */
protected:
   //! Каталог с файлами протоколов
   const QString & ProtoDir(void) const;
   //! Имя файла протоколов
   const QString & ProtoFile(void) const;
   //! Вывод строки протокола
   void ap(const QString & msg);
   //! Уровень отладки
   int dbg_level_;
   //! Прочитать все переменные окружения и инициализировать зависящие от них данные
   void Init();
   //! Мютекс для файла протоколов
   QMutex proto_mtx_;
   //! Путь к каталогу протоколов
   QString protoDir_;
   //! Имя файла протокола
   QString protoFile_;
   //! Максимальный размер файла протокола
   int proto_size_;
private:
   //! Конструктор по умолчанию
   CGlobal()
   {
      Init();
   };
   //! Конструктор копирования
   CGlobal(const CGlobal&);
   //! Оператор присвоения
   CGlobal& operator=(const CGlobal&);
   //! Деструктор
   ~CGlobal();
   //!
};

#endif
