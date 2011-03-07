# Create custom command for flex/lex (note the outputs)
#--header-file=${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.hpp

  ADD_CUSTOM_COMMAND(
    SOURCE ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/adc.l
    COMMAND /usr/bin/flex++
    ARGS 
         --outfile=${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.cc
        ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/adc.l
    TARGET ADCParser
    OUTPUTS ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.cc)

  SET_SOURCE_FILES_PROPERTIES(
            ${PROJECT_SOURCE_DIR}/src/flexscanner/adc/lex.adc.cc GENERATED)
  #Create target for the parser
  ADD_CUSTOM_TARGET(ADCParser echo "Creating ADC Parser")

rosbuild_add_library(minihubo-adc ../src/flexscanner/adc/lex.adc.cc 
                                  ../src/flexscanner/adc/adcDS.cpp)
