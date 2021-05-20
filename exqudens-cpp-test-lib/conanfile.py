from conans import ConanFile, tools
from os import path


class ConanConfiguration(ConanFile):
    name = "exqudens-cpp-test-lib"
    settings = "arch", "os", "compiler", "build_type"
    options = {"shared": [True, False]}
    default_options = {"shared": True}

    def set_version(self):
        self.version = tools.load(path.join(path.dirname(path.dirname(path.abspath(__file__))), "version.txt")).strip()

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
