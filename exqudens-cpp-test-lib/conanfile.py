from conans import ConanFile, tools
from os import path


class ConanConfiguration(ConanFile):
    settings = "arch", "os", "compiler", "build_type"
    options = {"shared": [True, False]}

    def set_name(self):
        self.name = path.basename(path.dirname(path.abspath(__file__)))

    def set_version(self):
        self.version = tools.load(path.join(path.dirname(path.dirname(path.abspath(__file__))), "version.txt")).strip()

    def package(self):
        self.copy("*")

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)


if __name__ == "__main__":
    pass
