#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"

namespace tut
{
   // ����������� �����
   struct logic_shared_data
   {
   };

   typedef test_group<logic_shared_data, 20> tg_logic;// 20 tests
   typedef tg_logic::object testlogic;

   tg_logic group_logic("LOGIC");

   // ����������� ������
   template<>   template<>
   void testlogic::test<1>()
   {
      //���������� �����
      //������ �����
      /*
         ����:
         AsQs
         ����
         Ts5d2d

         ���������:
         ������� ����������.
         ����������� ������� ���������� �� �����, ���� ������.
         �� ��������:
         ���� -      0 (��� �����������)
         ����-���� - 0
         ���� -      2 (�� 2� ������)
         ����� -     2
         ��� -       2
         ����� -     2
         ��� ���� -  2
         ���-���� -  1 (�� 1� �����)
      */

      tensure(__FILE__, __LINE__, true);
   }
}

