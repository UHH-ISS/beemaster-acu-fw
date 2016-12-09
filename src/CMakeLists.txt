# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        acu.cc
        aggregation.cc
        correlation.cc
        incoming_alert.cc
        outgoing_alert.cc
        receiver.cc
        sender.cc
        storage.cc
        threshold.cc)

# Static library definition (.a)
add_library(acu_static STATIC ${SOURCE_FILES})
set_target_properties(acu_static PROPERTIES
        OUTPUT_NAME acu)
target_link_libraries(acu_static ${LINK_LIBS})

# Shared library definition (.so)
add_library(acu_shared SHARED ${SOURCE_FILES})
set_target_properties(acu_shared PROPERTIES
        OUTPUT_NAME acu)
target_link_libraries(acu_shared ${LINK_LIBS})