# Create custom command for flex/lex (note the outputs)
  ADD_CUSTOM_COMMAND(
    SOURCE ${PROJECT_SOURCE_DIR}/flexscanner/robotis/robotis.l
    COMMAND /usr/bin/flex++
    ARGS --outfile=${PROJECT_SOURCE_DIR}/flexscanner/robotis/robotis.yy.cpp
    #    --header-file=robotis.yy.h
        ${PROJECT_SOURCE_DIR}/flexscanner/robotis/robotis.l
    TARGET RobotisParser
    OUTPUTS ${PROJECT_SOURCE_DIR}/flexscanner/robotis/robotis.yy.cpp)
  SET_SOURCE_FILES_PROPERTIES(
      ${PROJECT_SOURCE_DIR}/flexscanner/robotis/robotis.yy.cpp GENERATED)
  #Create target for the parser
  ADD_CUSTOM_TARGET(RobotisParser echo "Creating Robotis Parser")

#  ADD_CUSTOM_COMMAND(
#   SOURCE ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/adc.l
#   COMMAND /usr/bin/flex++
#   ARGS 
#        --outfile=${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.cc
#       ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/adc.l
#   TARGET ADCParser
#   OUTPUTS ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.cc)

# SET_SOURCE_FILES_PROPERTIES(
#           ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.cc GENERATED)
# #Create target for the parser
# ADD_CUSTOM_TARGET(ADCParser echo "Creating ADC Parser")

#rosbuild_add_library(minihubo-adc ../src/flexscanner/adc/lex.adc.cc 
#                                  ../src/flexscanner/adc/adcDS.cpp)

add_library(conductor-robotis SHARED robotis/robotisDS.cpp
                              robotis/robotis.yy.cpp
                              ../robotis.cpp)
target_link_libraries(conductor-robotis conductor-common)
install(TARGETS conductor-robotis
		LIBRARY DESTINATION lib)
