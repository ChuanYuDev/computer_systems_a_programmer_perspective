add_executable(3.5 3.5.c)

add_executable(11.2_hex2dd 11.2_hex2dd.c)
target_link_libraries(11.2_hex2dd ${CSAPP_LIB})

add_executable(11.3_dd2hex 11.3_dd2hex.c)
target_link_libraries(11.3_dd2hex ${CSAPP_LIB})