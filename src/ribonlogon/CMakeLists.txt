########### next target ###############

SET(ribon-logon_SRCS
AuthCodes.h
AuthSocket.cpp
AuthSocket.h
Main.cpp
RealmList.cpp
RealmList.h
)

SET(ribon-logon_LINK_FLAGS "")

add_executable(ribon-logon ${ribon-logon_SRCS})
add_definitions(
-D_RIBON_REALM_CONFIG='"${CONF_DIR}/ribonlogon.conf"'
)
IF (DO_MYSQL)
   SET(ribon-logon_LINK_FLAGS "-pthread ${ribon-logon_LINK_FLAGS}")
ENDIF(DO_MYSQL)
IF (DO_POSTGRESQL)
   SET(ribon-logon_LINK_FLAGS "-pthread ${ribon-logon_LINK_FLAGS}")
ENDIF(DO_POSTGRESQL)

IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
   SET(ribon-logon_LINK_FLAGS "-framework Carbon ${ribon-logon_LINK_FLAGS}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")

SET_TARGET_PROPERTIES(ribon-logon PROPERTIES LINK_FLAGS "${ribon-logon_LINK_FLAGS}")

target_link_libraries(
ribon-logon
shared
ribonframework
ribonsockets
ribondatabase
ribonauth
ribonconfig
zlib
gomp
${SSLLIB}
${MYSQL_LIBRARIES}
${POSTGRESQL_LIBRARIES}
${OSX_LIBS}
)

install(TARGETS ribon-logon DESTINATION bin)


########### install files ###############

install(FILES  ribonlogon.conf.dist DESTINATION etc)

