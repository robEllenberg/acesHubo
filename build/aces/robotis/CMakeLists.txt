# Create custom command for flex/lex (note the outputs)
  ADD_CUSTOM_COMMAND(
    SOURCE ${PROJECT_SOURCE_DIR}/src/robotis.l
    COMMAND /usr/bin/flex 
    ARGS --outfile=${PROJECT_SOURCE_DIR}/robotis.yy.cpp
        --header-file=robotis.yy.h
        ${PROJECT_SOURCE_DIR}/src/robotis.l
    TARGET RobotisParser
    OUTPUTS ${PROJECT_SOURCE_DIR}/robotis.yy.cpp)
  SET_SOURCE_FILES_PROPERTIES(${PROJECT_SOURCE_DIR}/robotis.yy.cpp GENERATED)
  #Create target for the parser
  ADD_CUSTOM_TARGET(RobotisParser echo "Creating Robotis Parser")

rosbuild_add_library(aces-robotis ../src/robotis.cpp
                                  ../robotis.yy.cpp)
