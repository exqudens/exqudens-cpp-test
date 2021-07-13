from conans import ConanFile, tools
from os import path


def get_name():
    return path.basename(path.dirname(path.dirname(path.abspath(__file__)))) + "-lib"


def get_version():
    return tools.load(path.join(path.dirname(path.dirname(path.abspath(__file__))), "version.txt")).strip()


class ConanConfiguration(ConanFile):
    requires = [
        get_name() + "/" + get_version() + "@test-user/test-channel"
    ]
    settings = "arch", "os", "compiler", "build_type"
    options = {"shared": [True, False]}
    generators = "cmake_paths", "cmake_find_package"

    def imports(self):
        self.copy(pattern="*.dll", dst="bin", src="bin")
        self.copy(pattern="*.dylib", dst="lib", src="lib")


if __name__ == "__main__":
    pass
