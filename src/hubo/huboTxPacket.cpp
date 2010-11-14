#include "huboTxPacket.hpp"
namespace Hubo{
/*
int main()
{
	
	unsigned char tx[8];
	getHuboTx( 35, -87.73, tx);
//	std::count << "TX data \n";
	for( int i = 0; i < 8; i++)
	{
		cout << (unsigned int)tx[i] << " ";
		//printf(":%d",tx[i]);
	}
	
	cout << endl;
	return 0;
}
*/
    void getHuboTx( unsigned char motorNum, double deg, unsigned char* tx)
    {
        // Arguements: byte motorNum = motor number, see 'UDP Protocall
        // Final.xlsx' for deff double deg = angle in degree to send to motor.
        // motor directions are defined in Hubo manual Return: &tx[] = bytes to
        // send to hubo via udp to port 192.168.0.193:11000
        
        bool isNeg = false;		// negitive flag
        //unsigned char tx[8];
        unsigned char md5 = 0;			// md5 
        
        double degHold = deg;	// save deg
        int degInt	= std::floor(std::abs(deg));		// interger degree
        //degInt = abs(degInt);
        int degRem = (std::abs(deg) - degInt) * 100;		// degree remainder
        
        tx[0] = 255; 		// header
        tx[1] = 65;			// char(65) = A, this says to use advanced mode
        tx[2] = 1;			// number of motors to send to
        tx[3] = motorNum;	// motor number
        
        if(deg < 0)
        {
            isNeg = true;
        }
        
        tx[4] = (unsigned char)(( degInt >> 8 ) & 127);
        tx[5] = (unsigned char)(degInt & 255);
        
        if( degRem > 100)
        {
            degRem = 99;    
        }
        
        tx[6] = (unsigned char)(degRem & 255);
        
        if(isNeg)
        {		
            tx[4] = tx[4] | 128;
        }
        
        tx[7] = (unsigned char)( (((unsigned int)tx[3] +
                                   (unsigned int)tx[4] +
                                   (unsigned int)tx[5] +
                                   (unsigned int)tx[6])*tx[2])
                                & 255);
    }
};
