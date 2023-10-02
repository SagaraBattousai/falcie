
include(FetchContent)

set(googletest_commit_hash "e47544ad31cb3ceecd04cc13e8fe556f8df9fe0b")

#Since master (main) is updated so often it can be slow to use git based on branch
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/${googletest_commit_hash}.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

if(WIN32)
  #For Windows: Prevent overriding the project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

set(testing_sources 
  "cactuar.cpp"
  )

add_executable(falcie_tests ${testing_sources})

set_property(TARGET falcie_tests PROPERTY FOLDER Tests)
set_target_properties(gtest gtest_main gmock gmock_main
  PROPERTIES FOLDER Tests/googleTest)

target_link_libraries(falcie_tests PRIVATE gtest_main cactuar)

if(WIN32)
  # symlink_library_to_target(tests pulse)
endif()

include(GoogleTest)
gtest_discover_tests(falcie_tests)


