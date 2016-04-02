#include <boost/test/unit_test.hpp>

#include "util/textselector.h"

BOOST_AUTO_TEST_CASE( textselector_test ) {
    TextSelector<std::string> Selector;
    Selector.insert({"aaaa", "aa", "bbacad", "addee", "asdaf123", "asdfees"});
    std::string found;
    //Should not find -> throw exception
    BOOST_CHECK_THROW(found = Selector.find_match("aa232"), SelectorError);

    BOOST_CHECK_NO_THROW(found = Selector.find_match("aa"));
    BOOST_CHECK(found == "aa");

    auto range = Selector.selection_range("as");
    auto correct = {"asdaf123", "asdfees"};
    BOOST_CHECK_EQUAL_COLLECTIONS(range.first, range.second, correct.begin(), correct.end());


    TextSelector<std::string> Selector2 = {"aa", "bb", "cc"};

    BOOST_CHECK_NO_THROW(found = Selector2.find_match("aa"));
    BOOST_CHECK(found == "aa");

    BOOST_CHECK_NO_THROW(found = Selector2.find_match("bb"));
    BOOST_CHECK(found == "bb");
}
