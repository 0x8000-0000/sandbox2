add_executable(hello hello.cpp)
target_link_libraries(hello PUBLIC project_options project_warnings)
target_link_libraries(hello PUBLIC CONAN_PKG::fmt)

add_executable(dynstruct dynstruct.cpp)
target_link_libraries(dynstruct PUBLIC project_options project_warnings)
target_link_libraries(dynstruct PUBLIC CONAN_PKG::fmt CONAN_PKG::frozen)
