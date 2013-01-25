/*! \file DBManager.h
    \brief �������� ��
    \author Kiselev Kirill
    \date 08.07.2011
*/
#ifndef DBMANAGER_H
#define DBMANAGER_H

/*! \class DBManager
   \brief ����� �������� ����
*/
class DBManager : public QObject
{
   Q_OBJECT
public:
   //! �����������
   DBManager();
   //! ����������
   ~DBManager();
   //! ������� �� ���������� � ��
   /*!
   \return ����������
   */
   inline bool isGood() const { return db_is_good_; }
   //! �������� ��������� ������
   /*!
   \return �������� ������
   */
   inline QString lastError() const { return lastError_; }
   //! ������� ������ �� ��
   /*!
   \return ������ �� ��
   */
   const QSqlDatabase & database() const { return db_; }
protected:
   //! ���������� ���������� � ��
   /*!
   \return ��������� ����������
   */
   bool connectDB();
   //! ��������� ��������� ��
   /*!
   \return ��������� ��������
   */
   bool CheckDBStruct();
   //! ��������� �������
   /*!
   \param tableName ��� �������
   \param columnName ��� �������
   \param columnType ��� �������
   \return ��������� ����������
   */
   bool alterTable(const QString & tableName,
                   const QString & columnName,
                   const QString & columnType);
   //! ������� ������������ ��
   bool db_is_good_;
   //! �������� ��������� ������
   QString lastError_;
   //! ��
   QSqlDatabase db_;
};

#endif