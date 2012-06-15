all: driver cadknob

cadknob:
	./3dx/xapp | python ./3dx/parsenav.py 
	#&> /dev/null

driver:
	sudo echo "got root"
	sudo /etc/3DxWare/daemon/3dxsrv -d usb &

conductor:
	@echo "Rember to source the Conductor env"
	stty -F /dev/ttyUSB0 38400
	stty -F /dev/ttyUSB0 raw
	aces-launcher ../orocosScripts/omnibot.ops

matlab:
	@echo "Run spaceball.m for interaction code"
	matlab -nodesktop
