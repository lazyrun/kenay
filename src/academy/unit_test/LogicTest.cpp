#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"

namespace tut
{
   // Определение типов
   struct logic_shared_data
   {
   };

   typedef test_group<logic_shared_data, 20> tg_logic;// 20 tests
   typedef tg_logic::object testlogic;

   tg_logic group_logic("LOGIC");

   // Определение тестов
   template<>   template<>
   void testlogic::test<1>()
   {
      //вычисление аутов
      //анализ стола
      /*
         Дано:
         AsQs
         Флоп
         Ts5d2d

         Посчитать:
         Текущую комбинацию.
         Возможность собрать комбинацию на одной, двух улицах.
         По убыванию:
         Каре -      0 (нет возможности)
         Фулл-хаус - 0
         Флеш -      2 (на 2х улицах)
         Стрит -     2
         Сет -       2
         Допер -     2
         Две пары -  2
         Топ-Пару -  1 (на 1й улице)
      */

      tensure(__FILE__, __LINE__, true);
   }
}

