from conans import ConanFile, tools


class ConanConfiguration(ConanFile):
    #requires = "glog/0.4.0"
    name = "library-exqudens-cpp-test"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
