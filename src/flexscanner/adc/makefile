all: adc.l adcDS.cpp adcDS.hpp
	flex++ --header=lex.adc.hpp adc.l
	g++ -g -D__ADC_PARSE_TESTSUITE__ -DNDEBUG lex.adc.cc adcDS.cpp -o lexer

clean:
	-@rm lex.adc.cc lex.adc.hpp lexer
