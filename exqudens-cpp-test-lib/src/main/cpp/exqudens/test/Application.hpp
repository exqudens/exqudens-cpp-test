#pragma once

#include <string>
#include <chrono>
#include <any>
#include <vector>
#include <tuple>
#include <functional>
#include <exception>
#include <stdexcept>
#include <iostream>

namespace exqudens::test {

  template <typename T> using Vector              = std::vector<T>;
  template <typename T>  using TestMethodReference = void(T::*)();

  using String                = std::string;
  using Any                   = std::any;
  using TestFunctionReference = void(*)();
  using TestMethod            = std::function<void()>;
  using Exception             = std::exception;
  using RuntimeError          = std::runtime_error;

  class Application {

    private:

      inline static const String SHORT_OPTION_HELP = "-h";
      inline static const String SHORT_OPTION_TESTS = "-t";
      inline static const String SHORT_OPTION_FAIL_FAST = "-ff";
      inline static const String LONG_OPTION_HELP = "--help";
      inline static const String LONG_OPTION_TESTS = "--tests";
      inline static const String LONG_OPTION_FAIL_FAST = "--fail-fast";
      inline static const String VALUE_ALL_TESTS = "all";

    private:

      Vector<String> commandLineArguments;
      Vector<std::tuple<unsigned int, String, String, Any, TestMethod>> tests;
      bool addHeader;
      bool addFooter;
      bool addNewLine;
      char delimiterChar;
      int delimiterSize;
      bool addSpace;
      bool addActionName;

    public:

      Application(
          int argc,
          char** argv,
          bool addHeader = true,
          bool addFooter = true,
          bool addNewLine = true,
          char delimiterChar = '=',
          int delimiterSize = 10,
          bool addSpace = true,
          bool addActionName = true
      );

      int run();

    protected:

      virtual void initialize() = 0;

      void addTestFunction(TestFunctionReference testFunctionReference, String testName);

      template <typename T>
      void addTestMethod(TestMethodReference<T> testMethodReference, String testName) {
        std::tuple<String, Any> nameObject = getOrCreateTestNameObject<T>(testName);
        String testTypeName = std::get<0>(nameObject);
        Any testObject = std::get<1>(nameObject);
        TestMethod testFunction = std::bind(testMethodReference, std::any_cast<T>(testObject));
        tests.push_back(std::make_tuple((unsigned int) tests.size(), testName, testTypeName, testObject, testFunction));
      }

      template <typename... ARGS>
      void addTestFunctions(TestFunctionReference testFunctionReference, String testName, ARGS... args) {
        addTestFunction(testFunctionReference, testName);
        addTestFunctions(args...);
      }

      void addTestFunctions();

      template <typename T, typename... ARGS>
      void addTestMethods(TestMethodReference<T> testMethodReference, String testName, ARGS... args) {
        addTestMethod(testMethodReference, testName);
        addTestMethods(args...);
      }

      void addTestMethods();

    private:

      bool isNumber(const String& string);

      std::tuple<bool, bool, Vector<String>> parseCommandLineArguments(Vector<String>& args);

      std::tuple<unsigned int, String, String, Any, TestMethod>* find(String testName);

      int runTest(Any object, TestMethod function, unsigned int index, String name);

      std::string toString(std::chrono::nanoseconds nanoseconds, bool extended = false);

      template <typename T>
      std::tuple<String, Any> getOrCreateTestNameObject(String testName) {
        if (find(testName) != nullptr) {
          String message;
          message += "Test map contains 'testName': ";
          message += testName;
          message += " (repeat not implemented yet!)";
          throw RuntimeError(message);
        }
        String testTypeName = typeid(Any).name();
        Any testObject;
        for (const std::tuple<unsigned int, String, String, Any, TestMethod>& entry : tests) {
          String currentTestTypeName = std::get<2>(entry);
          if (testTypeName == currentTestTypeName) {
            testObject = std::get<3>(entry);
            break;
          }
        }
        if (!testObject.has_value()) {
          testObject = T();
        }
        return std::make_tuple(testTypeName, testObject);
      }

  };

}
