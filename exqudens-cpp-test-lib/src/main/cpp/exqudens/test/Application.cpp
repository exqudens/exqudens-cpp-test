#include "exqudens/test/Application.hpp"

namespace exqudens::test {

  /*int Application::run(
      int argc,
      char** argv,
      bool addHeader,
      bool addFooter,
      bool addNewLine,
      char delimiterChar,
      int delimiterSize,
      bool addSpace,
      bool addActionName
  ) {
    return Application(
        argc,
        argv,
        addHeader,
        addFooter,
        addNewLine,
        delimiterChar,
        delimiterSize,
        addSpace,
        addActionName
    ).run();
  }*/

  Application::Application(
      int argc,
      char** argv,
      bool addHeader,
      bool addFooter,
      bool addNewLine,
      char delimiterChar,
      int delimiterSize,
      bool addSpace,
      bool addActionName
  ):
    commandLineArguments(Vector<String>()),
    addHeader(addHeader),
    addFooter(addFooter),
    addNewLine(addNewLine),
    delimiterChar(delimiterChar),
    delimiterSize(delimiterSize),
    addSpace(addSpace),
    addActionName(addActionName)
  {
    for (int i = 0; i < argc; i++) {
      commandLineArguments.push_back(String(argv[i]));
    }
  }

  int Application::run() {
    using std::cout;
    using std::cerr;
    using std::endl;

    int result = 0;

    initialize();

    std::tuple<bool, bool, Set<String>> config;
    config = parseCommandLineArguments(commandLineArguments);

    bool helpRequested = std::get<0>(config);
    bool failFast = std::get<1>(config);
    Set<String> testNames = std::get<2>(config);

    if (helpRequested) {
      cout << "Usage: -h" << endl;
    } else {
      for (auto& [name, value] : testMap) {
        if (testNames.contains(name)) {
          Any object = std::get<1>(value);
          TestMethod function = std::get<2>(value);
          if (result == 0) {
            result = runTest(object, function, name);
          } else {
            if (failFast) {
              break;
            } else {
              runTest(object, function, name);
            }
          }
        }
      }
    }

    return result;
  }

  std::tuple<bool, bool, Set<String>> Application::parseCommandLineArguments(Vector<String>& args) {
    bool helpRequested = false;
    bool failFast = false;
    Set<String> testNames;
    if (!commandLineArguments.empty()) {
      bool testNamesStarted = false;
      for (const String& commandLineArgument : commandLineArguments) {
        if (SHORT_OPTION_HELP == commandLineArgument || LONG_OPTION_HELP == commandLineArgument) {
          helpRequested = true;
          break;
        }
        if (SHORT_OPTION_FAIL_FAST == commandLineArgument || LONG_OPTION_FAIL_FAST == commandLineArgument) {
          failFast = true;
          continue;
        }
        if (SHORT_OPTION_TESTS == commandLineArgument || LONG_OPTION_TESTS == commandLineArgument) {
          testNamesStarted = true;
          continue;
        }
        if (testNamesStarted) {
          if (VALUE_ALL_TESTS == commandLineArgument) {
            for (auto& [key, value] : testMap) {
              testNames.insert(key);
            }
            break;
          } else {
            testNames.insert(commandLineArgument);
            continue;
          }
        }
      }
    }
    return std::make_tuple(helpRequested, failFast, testNames);
  }

  int Application::runTest(Any object, TestMethod function, String name) {
    using std::cout;
    using std::cerr;
    using std::endl;

    int result;

    String header = String();
    String footer = String();

    if (addHeader) {
      header.append(delimiterSize, delimiterChar);
      if (addSpace) {
        header.append(1, ' ');
      }
      if (addActionName) {
        header.append("run test:");
        header.append(1, ' ');
      }
      header.append(name);
      if (addSpace) {
        header.append(1, ' ');
      }
      header.append(delimiterSize, delimiterChar);

      cout << header << endl;
    }

    try {
      function();
      result = 0;
    } catch (Exception& e) {
      cerr << "ERROR: " << e.what() << endl;
      result = 1;
    }

    if (addFooter) {
      footer.append(addHeader ? header.size() : delimiterSize, delimiterChar);

      cout << footer << endl;
    }

    if (addNewLine) {
      cout << endl;
    }

    return result;
  }

  void Application::addTestFunction(TestFunctionReference testFunctionReference, String testName) {
    std::tuple<String, Any> nameObject = getOrCreateTestNameObject<Any>(testName);
    String testTypeName = std::get<0>(nameObject);
    Any testObject = std::get<1>(nameObject);
    TestMethod testFunction = testFunctionReference;
    testMap[testName] = std::make_tuple(testTypeName, testObject, testFunction);
  }

  void Application::addTestFunctions() {
  }

  void Application::addTestMethods() {
  }

}
