#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"

#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SchemaItem { namespace StringSchemaItemTest {

    /**
     * Test StringSchemaItem no default value
     *
     * Create SchemaItem without default value. Method setDefaultValue should always return false
     * and leave SmartObject in previous state.
     **/
    TEST(test_no_default_value, test_StringSchemaItemTest)
    {
        using namespace NsSmartDeviceLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CStringSchemaItem> item = CStringSchemaItem::create(); // No default value, no max length

        //Object - valid string
        obj = "New valid string";
        ASSERT_EQ(std::string("New valid string"), (std::string)obj);

        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_EQ(std::string("New valid string"), (std::string)obj);

        //Obj - bool
        obj = true;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_TRUE((bool)obj);

        //Object - number
        obj = 3.1415926;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        EXPECT_EQ(3.1415926, (double)obj);
    }

    /**
     * Test StringSchemaItem with default value
     *
     * Create SchemaItem with default value. Method setDefaultValue should return true,
     * String SmartObject should contain default value.
     * Not string SmartObject should converted to StringObject and setted up by the default value.
     **/
    TEST(test_item_with_default_value, test_StringSchemaItemTest)
    {
        using namespace NsSmartDeviceLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CStringSchemaItem> item = CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
            TSchemaItemParameter<std::string>("Default string")); // Default value, no max length

        //Object - valid string
        obj = "New valid string";
        ASSERT_EQ(std::string("New valid string"), (std::string)obj);

        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);

        //Obj - bool
        obj = true;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);

        //Object - number
        obj = 3.1415926;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);
    }

    /**
     * Test StringSchemaItem with max length
     **/
    TEST(test_item_with_max_length, test_StringSchemaItemTest)
    {
        using namespace NsSmartDeviceLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CStringSchemaItem> item = CStringSchemaItem::create(TSchemaItemParameter<size_t>(25),
            TSchemaItemParameter<std::string>("Default string"));

        //Object - valid string
        obj = "New valid string";
        ASSERT_EQ(std::string("New valid string"), (std::string)obj);

        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);

        //Object - too long string
        obj = "New very very loooooong string";
        ASSERT_EQ(std::string("New very very loooooong string"), (std::string)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
    }


    TEST(test_map_validate, test_StringSchemaItemTest)
    {
        using namespace NsSmartDeviceLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CStringSchemaItem> item = CStringSchemaItem::create(TSchemaItemParameter<size_t>(25),
            TSchemaItemParameter<std::string>("Default string"));

        obj["str"] = "New valid string";
        obj["long"] = "New very very loooooong string";
        obj["bool"] = true;
        obj["num"] = 3.14;

        int resultType = item->validate(obj["str"]);
        EXPECT_EQ(Errors::OK, resultType);

        resultType = item->validate(obj["long"]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

        resultType = item->validate(obj["bool"]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resultType = item->validate(obj["num"]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        bool resDefault = item->setDefaultValue(obj["str"]);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj["str"]);

        resDefault = item->setDefaultValue(obj["bool"]);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj["bool"]);

        resDefault = item->setDefaultValue(obj["num"]);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj["num"]);

        resultType = item->validate(obj["str"]);
        EXPECT_EQ(Errors::OK, resultType);

        resultType = item->validate(obj["long"]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

        resultType = item->validate(obj["bool"]);
        EXPECT_EQ(Errors::OK, resultType);

        resultType = item->validate(obj["num"]);
        EXPECT_EQ(Errors::OK, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
    }

    TEST(test_array_validate, test_StringSchemaItemTest)
    {
        using namespace NsSmartDeviceLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CStringSchemaItem> item = CStringSchemaItem::create(TSchemaItemParameter<size_t>(25),
            TSchemaItemParameter<std::string>("Default string"));

        obj[0] = "New valid string";
        obj[1] = "New very very loooooong string";
        obj[2] = true;
        obj[3] = 3.14;
        obj[4] = "New valid string";

        int resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::OK, resultType);

        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

        resultType = item->validate(obj[2]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resultType = item->validate(obj[3]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resultType = item->validate(obj[4]);
        EXPECT_EQ(Errors::OK, resultType);

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        bool resDefault = item->setDefaultValue(obj[0]);
        EXPECT_TRUE(resDefault);
        resDefault = item->setDefaultValue(obj[2]);
        EXPECT_TRUE(resDefault);
        resDefault = item->setDefaultValue(obj[4]);
        EXPECT_TRUE(resDefault);

        //Set default value for non-initialized element
        resDefault = item->setDefaultValue(obj[5]);
        EXPECT_TRUE(resDefault);

        EXPECT_EQ(std::string("Default string"), (std::string)obj[0]);
        EXPECT_EQ(std::string("Default string"), (std::string)obj[2]);
        EXPECT_EQ(std::string("Default string"), (std::string)obj[4]);
        EXPECT_EQ(std::string("Default string"), (std::string)obj[5]);

        resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        resultType = item->validate(obj[2]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[3]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[4]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[5]);
        EXPECT_EQ(Errors::OK, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(std::string("Default string"), (std::string)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);    }

}}}}}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}