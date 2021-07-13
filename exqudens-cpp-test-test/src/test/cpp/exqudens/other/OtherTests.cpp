#include <iostream>
#include <stdexcept>

#include "exqudens/other/OtherTests.hpp"

namespace exqudens::other {

  void testFunction1() {
    std::cout << "exqudens.other.testFunction1()" << std::endl;
  }

  void testFunction2() {
    std::cout << "exqudens.other.testFunction2()" << std::endl;
  }

  OtherTests::OtherTests() {
    std::cout << "OtherTests.CONSTRUCTOR" << std::endl;
  }

  void OtherTests::testMethod1() {
    std::cout << "exqudens.other.OtherTests.testMethod1()" << std::endl;
  }

  void OtherTests::testMethod2() {
    std::cout << "exqudens.other.OtherTests.testMethod2()" << std::endl;
    //throw std::runtime_error("in 'exqudens.other.OtherTests.testMethod2'");
  }

}
