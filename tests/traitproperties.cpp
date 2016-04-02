#include <boost/test/unit_test.hpp>

#include "traits/traitproperty.h"

BOOST_AUTO_TEST_CASE( trait_property_test ) {
    TraitProperty<std::string> stringProperty("string");
    TraitProperty<int> intProperty("int");
    TraitProperty<std::unique_ptr<int>> uniquePtrProperty("unique");

    TraitPropertyValueMap values;

    stringProperty.store(values, "Hello world");
    intProperty.store(values, 42);
    uniquePtrProperty.store(values, std::unique_ptr<int>(new int(42)));

    BOOST_CHECK_EQUAL(intProperty.load(values), 42);
    BOOST_CHECK_EQUAL(*uniquePtrProperty.load(values), 42);

    std::string s = stringProperty.load(values);
    BOOST_CHECK_EQUAL(s, "Hello world");

    stringProperty.destructValue(values);
    intProperty.destructValue(values);
    uniquePtrProperty.destructValue(values);
}
