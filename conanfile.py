from conans import ConanFile, CMake, tools
from conans.errors import ConanException
import os

class LLAAndroidConan(ConanFile):
    name = "LogicalAccessAndroid"
    version = "2.2.1"
    license = "<Put the package license here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of LLA here>"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "androidreaders*"
    default_options="""
    LogicalAccess:LLA_BUILD_PKCS=False
    LogicalAccess:LLA_BUILD_IKS=False
    """
    revision = "scm"
    generators = "cmake"

    def requirements(self):
        # Either use channel embedded in package metadata or use current git branch.
        try:
            self.requires('LogicalAccessPrivate/' + self.version + '@islog/' + self.channel)
        except ConanException:
            self.requires('LogicalAccessPrivate/' + self.version + '@islog/' + tools.Git().get_branch())
    
    def configure_cmake(self):
        cmake = CMake(self)

        if self.settings.os == 'Android':
            # Fix issue with conan and clang / libcxx. see LLA conan file
            cmake.definitions['CONAN_LIBCXX'] = ''
        
        if tools.os_info.is_windows:
            # For MSVC we need to restrict configuration type to avoid issues.
            cmake.definitions['CMAKE_CONFIGURATION_TYPES'] = self.settings.build_type
            
        cmake.configure(source_folder='androidreaders')
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs.append('androidreaders')

    def package_id(self):
        self.info.requires['LogicalAccess'].full_package_mode()
