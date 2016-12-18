# CMakeLists.txt for tests

# Source files
set(SOURCE_FILES
        main.cc
        test_aggregation.cc
        test_correlation.cc
        test_incoming_alert.cc
        test_logger.cc
        test_outgoing_alert.cc
        test_receiver.cc
        test_sender.cc
        test_storage.cc
        test_threshold.cc
        test_acu.cc)

# Add catch (testing library)
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Define the executable
add_executable(tests ${SOURCE_FILES})
target_link_libraries(tests
        Catch
        acu_shared)

# Add tests to CTest
enable_testing()
add_test(NAME acu-fw-tests COMMAND tests)
