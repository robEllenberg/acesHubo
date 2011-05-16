# Create custom command for flex/lex (note the outputs)
  ADD_CUSTOM_COMMAND(
    SOURCE ${PROJECT_SOURCE_DIR}/src/flexscanner/robotis/robotis.l
    COMMAND /usr/bin/flex++
    ARGS --outfile=${PROJECT_SOURCE_DIR}/src/flexscanner/robotis/robotis.yy.cpp
    #    --header-file=robotis.yy.h
        ${PROJECT_SOURCE_DIR}/src/flexscanner/robotis/robotis.l
    TARGET RobotisParser
    OUTPUTS ${PROJECT_SOURCE_DIR}/src/flexscanner/robotis/robotis.yy.cpp)
  SET_SOURCE_FILES_PROPERTIES(
      ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/robotis.yy.cpp GENERATED)
  #Create target for the parser
  ADD_CUSTOM_TARGET(RobotisParser echo "Creating Robotis Parser")

rosbuild_add_library(aces-robotis ../src/flexscanner/robotis/robotisDS.cpp
                                  ../src/flexscanner/robotis/robotis.yy.cpp
                                  ../src/robotis.cpp)
