add_executable(hello hello.cpp)
target_link_libraries(hello PUBLIC project_options project_warnings)
target_link_libraries(hello PUBLIC CONAN_PKG::fmt)
