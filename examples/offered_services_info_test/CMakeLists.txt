# Copyright (C) 2015-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required (VERSION 2.8)

set(EXAMPLE_CONFIG_FILES
    "../config/vsomeip.json"
    "../config/vsomeip-local.json"
    "../config/vsomeip-tcp-client.json"
    "../config/vsomeip-tcp-service.json"
    "../config/vsomeip-udp-client.json"
    "../config/vsomeip-udp-service.json"
)

# Examples
add_executable(offered_services_info_test_client offered_services_info_test_client.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(offered_services_info_test_client ${VSOMEIP_NAME} ${Boost_LIBRARIES} ${DL_LIBRARY})

add_executable(offered_services_info_test_service offered_services_info_test_service.cpp ${EXAMPLE_CONFIG_FILES})
target_link_libraries(offered_services_info_test_service ${VSOMEIP_NAME} ${Boost_LIBRARIES} ${DL_LIBRARY})

add_dependencies(offered_services_info_test hello offered_services_info_test_client offered_services_info_test_service)

install (
    TARGETS offered_services_info_test_client offered_services_info_test_service
    RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
)

###################################################################################################
