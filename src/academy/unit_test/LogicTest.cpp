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

      */
      tensure(__FILE__, __LINE__, true);
   }
}

