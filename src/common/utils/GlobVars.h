/*! \file GlobVars.h
    \brief �����, ���������� ���������� ����������
*/

#ifndef GlobVarsH
#define GlobVarsH

#include <vector>

/*! 
\class CGlobal
\brief Singleton, ���������� ���������� ����������
*/
class CGlobal
{
public:
   //! ������������ ������ � ���������� ������
   /*!
   \param[in] canWrite ������� ����������� ������
   \return ������ �� ���������� ��������� ������
   */
   static CGlobal& Instance()
   {
      static CGlobal Instance_;
      return Instance_;
   }
   //! ����� ������ � �������� ��� dbg_level_==0
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap0(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==1
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap1(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==2
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap2(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==3
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap3(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==4
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap4(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==5
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap5(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==6
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap6(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==7
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap7(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==8
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap8(const QString& msg);
   //! ����� ������ � �������� ��� dbg_level_==9
   /*!
      \sa dbg_level_,protoFile_,protoDir_,proto_mtx_
   */
   void ap9(const QString& msg);
   //! ���������� ������� �������
   /*!
      \sa dbg_level_
   */
   int GetDbg(){return dbg_level_;}
   //! ������������� ������� �������
   /*!
      \sa dbg_level_
   */
   void SetDbg(int);
   //! ������������� ������ ����� ���������
   /*!
      \sa proto_size_
   */
   void SetProtoSize(int);
   //! ���������� ������� ������� ����������
   /*!
      \sa workDir_
   */
protected:
   //! ������� � ������� ����������
   const QString & ProtoDir(void) const;
   //! ��� ����� ����������
   const QString & ProtoFile(void) const;
   //! ����� ������ ���������
   void ap(const QString & msg);
   //! ������� �������
   int dbg_level_;
   //! ��������� ��� ���������� ��������� � ���������������� ��������� �� ��� ������
   void Init();
   //! ������ ��� ����� ����������
   QMutex proto_mtx_;
   //! ���� � �������� ����������
   QString protoDir_;
   //! ��� ����� ���������
   QString protoFile_;
   //! ������������ ������ ����� ���������
   int proto_size_;
private:
   //! ����������� �� ���������
   CGlobal()
   {
      Init();
   };
   //! ����������� �����������
   CGlobal(const CGlobal&);
   //! �������� ����������
   CGlobal& operator=(const CGlobal&);
   //! ����������
   ~CGlobal();
   //!
};

#endif
