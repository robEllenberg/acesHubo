all: adc.l adcDS.hpp
	flex++ --header=lex.adc.hpp adc.l
	g++ -D__ADC_PARSE_TESTSUITE__ lex.adc.cc -o lexer

clean:
	-@rm lex.adc.cc lex.adc.hpp lexer
