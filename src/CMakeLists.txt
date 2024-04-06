set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
list(APPEND SOURCE_FILES "main.cpp" "cpu.cpp" "memory.cpp")

add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE project_settings fmt::fmt)
target_link_libraries(main PRIVATE
                      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
                      $<IF:$<TARGET_EXISTS:SDL2::SDL2>, SDL2::SDL2, SDL2::SDL2-static>)
