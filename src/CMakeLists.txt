# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        acu.cc
        alert_mapper.cc
        incoming_alert.cc
        outgoing_alert.cc
        receiver.cc
        sender.cc
        threshold.cc)

# Disable -Wununsed-parameter as broker::get produces this error even when used correctly (I think?!)
# TODO: Maybe scope this to the source files where broker::get is used? (Currently incoming_alert.cc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")

# Static library definition (.a)
add_library(acu_static STATIC ${SOURCE_FILES})
set_target_properties(acu_static PROPERTIES
        OUTPUT_NAME acu)
target_link_libraries(acu_static ${LINK_LIBS})

# Shared library definition (.so)
add_library(acu_shared SHARED ${SOURCE_FILES} alert_mapper.cc)
set_target_properties(acu_shared PROPERTIES
        OUTPUT_NAME acu)
target_link_libraries(acu_shared ${LINK_LIBS})