#include <chrono>

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
      String usage("Usage:");
      usage += " ";
      usage += "[" + SHORT_OPTION_HELP + " | " + LONG_OPTION_HELP + "]";
      usage += " ";
      usage += "[" + SHORT_OPTION_FAIL_FAST + " | " + LONG_OPTION_FAIL_FAST + "]";
      usage += " ";
      usage += SHORT_OPTION_TESTS + " | " + LONG_OPTION_TESTS;
      usage += " ";
      usage += VALUE_ALL_TESTS + " | names...";
      cout << usage << endl;
    } else {
      for (auto& [name, value] : testMap) {
        if (testNames.count(name) > 0) {
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
      auto start = std::chrono::high_resolution_clock::now();
      function();
      auto stop = std::chrono::high_resolution_clock::now();
      std::chrono::nanoseconds diff(std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count());
      cout << "TIME:" << toString(diff, true) << endl;
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

  std::string Application::toString(std::chrono::nanoseconds nanoSeconds, bool extended) {
    std::string string;

    if (!extended) {
      string += std::to_string(nanoSeconds.count());
      return string;
    }

    std::chrono::microseconds microSeconds;
    std::chrono::milliseconds milliSeconds;
    std::chrono::seconds seconds;
    std::chrono::minutes minutes;
    std::chrono::hours hours;

    microSeconds = std::chrono::duration_cast<std::chrono::microseconds>(nanoSeconds);
    nanoSeconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(microSeconds);

    milliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(microSeconds);
    microSeconds -= std::chrono::duration_cast<std::chrono::microseconds>(milliSeconds);

    seconds = std::chrono::duration_cast<std::chrono::seconds>(milliSeconds);
    milliSeconds -= std::chrono::duration_cast<std::chrono::milliseconds>(seconds);

    minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

    hours = std::chrono::duration_cast<std::chrono::hours>(minutes);
    minutes -= std::chrono::duration_cast<std::chrono::minutes>(hours);

    if (hours.count() > 0) string += String(" hours: ").append(std::to_string(hours.count()));
    if (minutes.count() > 0) string += String(" minutes: ").append(std::to_string(minutes.count()));
    if (seconds.count() > 0) string += String(" seconds: ").append(std::to_string(seconds.count()));
    if (milliSeconds.count() > 0) string += String(" milliSeconds: ").append(std::to_string(milliSeconds.count()));
    if (microSeconds.count() > 0) string += String(" microSeconds: ").append(std::to_string(microSeconds.count()));
    if (nanoSeconds.count() > 0) string += String(" nanoSeconds: ").append(std::to_string(nanoSeconds.count()));

    return string;
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
