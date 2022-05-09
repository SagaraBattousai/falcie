
include(FetchContent)

set(googletest_commit_hash "e2239ee6043f73722e7aa812a459f54a28552929")

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/${googletest_commit_hash}.zip
)

if(WIN32)
  # VV How????
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)


set(testing_sources 
  "anima.cpp"
  "atomos.cpp"
  "cactuar.cpp"
  "dahaka.cpp"
  "pulse.cpp")

add_executable(Testing ${testing_sources})
target_link_libraries(Testing PRIVATE gtest_main pulse)
# target_include_directories(Testing PUBLIC "${INCLUDE_ROOT}")

copy_library_to_target(Testing pulse)
copy_openssl_runtime_lib_to_target(Testing crypto)

include(GoogleTest)
gtest_discover_tests(Testing)
