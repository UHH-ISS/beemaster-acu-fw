/* main.cc
 * ACU Framework (test)
 * 
 * Stub file for catch main function.
 *
 * DO NOT MODIFY THIS FILE!
 * Write tests in separate source files named test_<classname>.cc
 *
 * @author: 1wilkens
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// This prevents CLion to accidentally optimize out
// the #include and #define
TEST_CASE("IGNORE", "IGNORE") {
    CATCH_CONFIG_MAIN
}
