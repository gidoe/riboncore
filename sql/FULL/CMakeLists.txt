########### install files ###############

FILE(GLOB _SQL *.sql)
FILE(GLOB README readme.txt)

install(FILES ${_SQL} DESTINATION share/ribon/sql/FULL)
install(FILES ${README} DESTINATION share/ribon/sql/FULL)