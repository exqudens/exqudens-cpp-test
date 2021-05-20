#include <iostream>
#include "exqudens/test/Application.hpp"
#include "exqudens/other/OtherTests.hpp"

namespace exqudens::test {

  class OtherTestApplication : public Application {

    public:

      using Application::Application;

    protected:

      void initialize() override {
        std::cout << __FUNCTION__ << " start" << std::endl;
        addTestFunctions(
            &exqudens::other::testFunction1, "exqudens.other.testFunction1",
            &exqudens::other::testFunction2, "exqudens.other.testFunction2"
        );
        addTestMethods<exqudens::other::OtherTests>(
            &exqudens::other::OtherTests::testMethod1, "exqudens.other.OtherTests.testMethod1",
            &exqudens::other::OtherTests::testMethod2, "exqudens.other.OtherTests.testMethod2"
        );
        std::cout << __FUNCTION__ << " done" << std::endl;
      }

  };

}

int main(int argc, char** argv) {
  return exqudens::test::OtherTestApplication(argc, argv).run();
}
