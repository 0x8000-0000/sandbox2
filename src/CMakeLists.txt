add_executable(hello hello.cpp)
target_link_libraries(hello PUBLIC project_options project_warnings)
target_link_libraries(hello PUBLIC CONAN_PKG::fmt)

add_executable(dynstruct dynstruct.cpp)
target_link_libraries(dynstruct PUBLIC project_options project_warnings)
target_link_libraries(dynstruct PUBLIC CONAN_PKG::fmt CONAN_PKG::frozen)

add_executable(tooples tooples.cpp)
target_link_libraries(tooples PUBLIC project_options project_warnings)
target_link_libraries(tooples PUBLIC CONAN_PKG::fmt CONAN_PKG::frozen)

add_executable(synchro synchro.cpp)
target_link_libraries(synchro PUBLIC project_options project_warnings)
target_link_libraries(synchro PUBLIC CONAN_PKG::fmt Threads::Threads)

#add_executable(say_my_name say_my_name.cpp)
#target_link_libraries(say_my_name PUBLIC project_options project_warnings)
#target_link_libraries(say_my_name PUBLIC CONAN_PKG::fmt)

add_executable(sequences sequences.cpp)
target_link_libraries(sequences PUBLIC project_options project_warnings)
target_link_libraries(sequences PUBLIC CONAN_PKG::fmt CONAN_PKG::frozen)

add_executable(out_stream out_stream.cpp)
target_link_libraries(out_stream PUBLIC project_options project_warnings)
target_link_libraries(out_stream PUBLIC CONAN_PKG::fmt CONAN_PKG::frozen)

add_executable(discoverable_setters discoverable_setters.cpp)
target_link_libraries(discoverable_setters PUBLIC project_options project_warnings)
target_link_libraries(discoverable_setters PUBLIC CONAN_PKG::fmt)
