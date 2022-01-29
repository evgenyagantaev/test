#include <limits.h>
#include "gtest/gtest.h"

#include "main.hpp"
#include "key.hpp"
#include "key_factory.hpp"

namespace key_tst
{

    TEST(key_class, creation) 
    {
        EXPECT_EQ(Key_factory::get_instance().get_map_size(), 1);
        EXPECT_EQ(Key_factory::get_instance().get_items_created(), 0);
        Key *key_pntr = Key_factory::get_instance().create(KEY_CLASS_ID);
        EXPECT_EQ(Key_factory::get_instance().get_items_created(), 1);

        Key &tst_key = (*key_pntr);

        EXPECT_FALSE(tst_key.get_state());
        tst_key.set_state(true);
        EXPECT_TRUE(tst_key.get_state());
        EXPECT_EQ(tst_key.get_count(), 0);
        tst_key.increment_count();
        EXPECT_EQ(tst_key.get_count(), 1);
        tst_key.increment_count();
        EXPECT_EQ(tst_key.get_count(), 2);
        tst_key.reset_count();
        EXPECT_EQ(tst_key.get_count(), 0);

        tst_key.bind_pin(S1_PIN);
        EXPECT_EQ(tst_key.get_pin(), S1_PIN);


    }




}// namespace key_tst