add_executable(DrautosTests
        Replica/SQEX/Luminous/AssetManager/LmAssetIDTest.cpp)

target_link_libraries(DrautosTests PRIVATE Boost::unit_test_framework)
add_test(NAME DrautosTests COMMAND DrautosTests)