
########### next target ###############

SET(ribon-core_SRCS
CliRunnable.cpp
CliRunnable.h
Main.cpp
Master.cpp
Master.h
RASocket.cpp
RASocket.h
WorldRunnable.cpp
WorldRunnable.h
)

SET(ribon-core_LINK_FLAGS "")

add_executable(ribon-core ${ribon-core_SRCS})
add_definitions(
-D_RIBON_CORE_CONFIG='"${CONF_DIR}/riboncore.conf"'
)
IF (DO_MYSQL)
    SET(ribon-core_LINK_FLAGS "-pthread ${ribon-core_LINK_FLAGS}")
ENDIF(DO_MYSQL)
IF (DO_POSTGRESQL)
    SET(ribon-core_LINK_FLAGS "-pthread ${ribon-core_LINK_FLAGS}")
ENDIF(DO_POSTGRESQL)


IF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    SET(ribon-core_LINK_FLAGS "-framework Carbon ${ribon-core_LINK_FLAGS}")
    SET(SCRIPT_LIB "")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")

SET_TARGET_PROPERTIES(ribon-core PROPERTIES LINK_FLAGS "${ribon-core_LINK_FLAGS}")

target_link_libraries(
ribon-core
game
shared
zlib
ribonframework
trinitysockets
ribondatabase
ribonauth
ribonconfig
vmaps
ZThread
g3dlite
${READLINE_LIBRARY}
gomp
${SCRIPT_LIB}
${MYSQL_LIBRARIES}
${POSTGRESQL_LIBRARIES}
${SSLLIB}
${ACE_LIBRARY}
${ZLIB}
${OSX_LIBS}
)

IF (CENTOS)
    target_link_libraries(
        ribon-core
        termcap
    )
ENDIF (CENTOS)

install(TARGETS ribon-core DESTINATION bin)


########### install files ###############

install(FILES  riboncore.conf.dist DESTINATION etc)

