project(tests)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run BattlefieldTest.cpp WombatTest.cpp
        WeaponTest.cpp
        AttackTest.cpp
        AllTests.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested

target_link_libraries(Google_Tests_run gtest gtest_main gmock)