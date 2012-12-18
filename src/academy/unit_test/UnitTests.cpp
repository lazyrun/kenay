#include <stdio.h>

#include "tut.h"
#include "TstUtils.h"

namespace tut
{
   // Определение типов
   struct bot_shared_data
   {
   };

   typedef test_group<bot_shared_data, 20> tg_bot;// 10 tests
   typedef tg_bot::object testobject;

   tg_bot group_bot("ACAD");

   // Определение тестов
   //тест покет карт
   template<>   template<>
   void testobject::test<1>()
   {
      tensure(__FILE__, __LINE__, 1 == 1);
   }
}
