
include(FetchContent)

set(googletest_commit_hash "e2239ee6043f73722e7aa812a459f54a28552929")

#VV Faster than git repo :)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/${googletest_commit_hash}.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE #??????
)

if(WIN32)
  #For Windows: Prevent overriding the project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

set(testing_sources 
  "anima.cpp"
  "atomos.cpp"
  "cactuar.cpp"
  "dahaka.cpp"
  "pulse.cpp")

add_executable(tests ${testing_sources})

set_property(TARGET tests PROPERTY FOLDER Tests)
set_target_properties(gtest gtest_main gmock gmock_main
  PROPERTIES FOLDER Tests/googleTest)

target_link_libraries(tests PRIVATE gtest_main pulse)
target_include_directories(tests PRIVATE 
  "${INCLUDE_ROOT}"
  "${INCLUDE_ROOT}/pulse/internal"
  )

if(WIN32)
  # symlink_library_to_target(tests pulse)
endif()

include(GoogleTest)
gtest_discover_tests(tests)


