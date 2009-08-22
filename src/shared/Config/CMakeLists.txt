
########### next target ###############

SET(ribonconfig_STAT_SRCS
   dotconfpp/dotconfpp.cpp
   dotconfpp/dotconfpp.h
   dotconfpp/mempool.cpp
   dotconfpp/mempool.h
   Config.cpp
   Config.h
   ConfigEnv.h
)

add_library(ribonconfig STATIC ${ribonconfig_STAT_SRCS})

