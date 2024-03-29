from conans import ConanFile, CMake, tools


class Sandbox2(ConanFile):

    name = "sandbox2"
    version = "0.1"
    license = "Apache 2.0"
    description = "Modern C++ exploratory sandbox"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"

    def source(self):
        pass

    def requirements(self):
        self.requires("fmt/8.1.1")
        self.requires("nlohmann_json/3.10.5")
        self.requires("frozen/1.0.1")

    def configure(self):
        tools.check_min_cppstd(self, "20")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        pass

    def package_info(self):
        pass
