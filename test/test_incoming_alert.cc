//
// Created by florian on 12/9/16.
//

#include <catch.hpp>
#include <iostream>
#include <broker/message.hh>
#include "acu/incoming_alert.h"

TEST_CASE("Testing IncomingAlert", "[incoming_alert]") {
    auto msg = broker::message{ "first", "second", "third", "fourth", "fifth", "sixth", "seventh" };
    auto alert = acu::IncomingAlert(msg);

    // Woohoo indexing.. hopefully this is zero-copy?
    std::cout << alert.incident_type() << std::endl;
}