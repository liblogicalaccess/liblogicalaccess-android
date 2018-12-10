from conans import ConanFile, CMake, tools
import os

class LLAAndroidConan(ConanFile):
    name = "LogicalAccessAndroid"
    version = "2.1.0"
    license = "<Put the package license here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of LLA here>"
    settings = "os", "compiler", "build_type", "arch"

    default_options="""
    LogicalAccess:LLA_BUILD_PKCS=False
    LogicalAccess:LLA_BUILD_IKS=False
    """
    # We rely on islog/develop despite being in the master branch.
    requires = 'LogicalAccess/2.1.0@islog/develop'
    generators = "cmake"
    
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
