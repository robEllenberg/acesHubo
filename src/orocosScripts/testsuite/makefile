.PHONY : %.ops
run:
	@aces-launcher 

run: testsuite50.log testsuite100.log testsuite500.log testsuite1000.log testsuite5000.log testsuite10000.log
	matlab
	
%.log: %.ops
	-aces-launcher ${<}

clean:
	@-rm orocos.log
	@-rm *.txt
