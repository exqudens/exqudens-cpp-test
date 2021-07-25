#include <locale>
#include <chrono>

#include "exqudens/test/Application.hpp"

namespace exqudens::test {

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

    std::tuple<bool, bool, Vector<String>> config;
    config = parseCommandLineArguments(commandLineArguments);

    bool helpRequested = std::get<0>(config);
    bool failFast = std::get<1>(config);
    Vector<String> testTags = std::get<2>(config);

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
      if (testTags.size() == 1 && testTags.at(0) == VALUE_ALL_TESTS) {
        for (unsigned int i = 0; i < tests.size(); i++) {
          std::tuple<unsigned int, String, String, Any, TestMethod>& entry = tests.at(i);
          String name = std::get<1>(entry);
          Any object = std::get<3>(entry);
          TestMethod function = std::get<4>(entry);
          if (result == 0) {
            result = runTest(object, function, i, name);
          } else {
            if (failFast) {
              break;
            } else {
              runTest(object, function, i, name);
            }
          }
        }
      } else {
        for (const String& tag : testTags) {
          for (unsigned int i = 0; i < tests.size(); i++) {
            std::tuple<unsigned int, String, String, Any, TestMethod>& entry = tests.at(i);
            String name = std::get<1>(entry);
            if (isNumber(tag)) {
              unsigned int index = std::stoul(tag,nullptr,0);
              if (i == index) {
                Any object = std::get<3>(entry);
                TestMethod function = std::get<4>(entry);
                if (result == 0) {
                  result = runTest(object, function, i, name);
                } else {
                  if (failFast) {
                    break;
                  } else {
                    runTest(object, function, i, name);
                  }
                }
              }
            } else {
              if (name == tag) {
                Any object = std::get<3>(entry);
                TestMethod function = std::get<4>(entry);
                if (result == 0) {
                  result = runTest(object, function, i, name);
                } else {
                  if (failFast) {
                    break;
                  } else {
                    runTest(object, function, i, name);
                  }
                }
              }
            }
          }
          if (result != 0 && failFast) {
            break;
          }
        }
      }
    }

    return result;
  }

  bool Application::isNumber(const String& string) {
    for (const char& c : string) {
      if (!std::isdigit(c)) {
        return false;
      }
    }
    return true;
  }

  std::tuple<bool, bool, Vector<String>> Application::parseCommandLineArguments(Vector<String>& args) {
    bool helpRequested = false;
    bool failFast = false;
    Vector<String> testTags;
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
            testTags.push_back(VALUE_ALL_TESTS);
            break;
          } else {
            testTags.push_back(commandLineArgument);
            continue;
          }
        }
      }
    }
    return std::make_tuple(helpRequested, failFast, testTags);
  }

  std::tuple<unsigned int, String, String, Any, TestMethod>* Application::find(String testName) {
    std::tuple<unsigned int, String, String, Any, TestMethod>* result = nullptr;
    for (std::tuple<unsigned int, String, String, Any, TestMethod>& entry : tests) {
      String name = std::get<1>(entry);
      if (testName == name) {
        result = &entry;
      }
    }
    return result;
  }

  int Application::runTest(Any object, TestMethod function, unsigned int index, String name) {
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
      header.append("[").append(std::to_string(index)).append("] ").append(name);
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
    tests.push_back(std::make_tuple((unsigned int) tests.size(), testName, testTypeName, testObject, testFunction));
  }

  void Application::addTestFunctions() {
  }

  void Application::addTestMethods() {
  }

}
