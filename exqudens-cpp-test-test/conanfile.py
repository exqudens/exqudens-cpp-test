from conans import ConanFile, tools
from os import path


def get_version():
    return tools.load(path.join(path.dirname(path.dirname(path.abspath(__file__))), "version.txt")).strip()


class ConanConfiguration(ConanFile):
    requires = [
        "exqudens-cpp-test-lib/1.0.0@test-user/test-channel"
    ]
    settings = "arch", "os", "compiler", "build_type"
    options = {"shared": [True, False]}

    def imports(self):
        self.copy(pattern="*", folder=True)
