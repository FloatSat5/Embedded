// Using UART other than UART_IDX3 used for debug
// Sends msg every second and echos whatever it receives

#include "rodos.h"
#include "hal_uart.h"

uint32_t baudrate = 115200; //115200; 9600
uint32_t baudrates[] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

// IDX3 is default UART used by RODOS
HAL_UART blueToothSerial(UART_IDX4); // tx: PC10, rx: PC11: probably swapped
//HAL_UART pi(UART_IDX3); // tx: PD8, rx: PD9

uint8_t msg[] = "Hi I am STM!\n";
uint16_t counter;
bool initHappened = false;

class Hello_Bluetooth : public StaticThread<>
{
	void init()
	{
		blueToothSerial.init(baudrate);
		//blueToothSerial.config(UART_PARAMETER_HW_FLOW_CONTROL,1);
		PRINTF("Hello_Bluetooth init\n");
		//setupBluetooth();
	}

	void run()
	{
		void init();
		//init();
		TIME_LOOP(NOW() + 0*SECONDS, 10000 * MILLISECONDS)
		{
            if (!initHappened){
                initHappened = true;
                setupBluetooth();
            }
			while(true) {
				//writeUartSafely("AT", 2, false);
				keepReading();
				Thread::suspendCallerUntil(NOW() + 10*MILLISECONDS);
			}
				
			uint8_t input[50];
			uint8_t msg_size = blueToothSerial.read(input, sizeof(input));
			input[msg_size] = '\0';

			// If input then echo
			if(msg_size)
			{
				blueToothSerial.write(input, msg_size);
				blueToothSerial.write("\n", 2);
			}

			if(counter >= 100)
			{
				blueToothSerial.write(msg, sizeof(msg));
				counter = 0;
			}
			counter++;
		}
	}

	bool writeUartSafely(char* buf, int size, bool printOut=true){
		if (printOut)
			PRINTF("writeUartSafely: %s\n", buf);
		int txBytes = 0;
		int retVal = 0;
		int errCnt = 0;
		while (txBytes < size){
			retVal = blueToothSerial.write(&buf[txBytes],size-txBytes);
			if (printOut)
            	PRINTF("writeUartSafely: write retVal=%d\n", retVal);
			if (retVal <= 0){
				errCnt++;
				if (errCnt > 100){
					return false;
				}
				//Thread::suspendCallerUntil(NOW() + 1*MILLISECONDS);
			}else if ( retVal != (size-txBytes) ){ // the whole buffer couldn't be sent -> wait a moment and send the rest
				//Thread::suspendCallerUntil(NOW() + 1*MILLISECONDS);
				txBytes += retVal;
				errCnt = 0;
			} else{
                break;
				//return true;
			}
		}
		if(printOut)
        	PRINTF("writeUartSafely: before isWriteFinished\n");
		//while(!blueToothSerial.isWriteFinished()){
		//	Thread::suspendCallerUntil(NOW() + 1*MILLISECONDS);
		//}
        Thread::suspendCallerUntil(NOW() + 100*MILLISECONDS);
        //while(!blueToothSerial.isDataReady()) {
        //    Thread::suspendCallerUntil(NOW() + 1*MICROSECONDS);
        //}
        //PRINTF("writeUartSafely: after isWriteFinished\n");
		char buf2[100];
		int pos = 0;
		while(true){
			int sizeRead = blueToothSerial.read(&buf2[pos], 1);
            if (sizeRead <= 0){
                break;
            }
            pos++;
            if (pos >= 100){
                break;
            }
		}
		buf2[pos] = '\0';
		if(printOut)
        	PRINTF("readUartSafely: %s\n", buf2);
		else if (buf2[0] != '\0' && buf2[0] != 'O')
			PRINTF("readUartSafely: %s\n", buf2);

		return pos;
	}

	void keepReading(){
		char buf2[100];
		int pos = 0;
		while(true){
			int sizeRead = blueToothSerial.read(&buf2[pos], 100 - pos);
            if (sizeRead <= 0){
                break;
            }
            pos += sizeRead;
            if (pos >= 100){
                break;
            }
		}
		buf2[pos] = '\0';
		if (pos > 0) {
			PRINTF("received: %s\n", buf2);
			writeUartSafely(buf2, pos, false);
		}
	}

    bool findBaudrate(){
        //for (int i = 0; i < sizeof(baudrates)/sizeof(uint32_t); i++){
		for (int i = sizeof(baudrates)/sizeof(uint32_t) -1; i >= 0; i--){
            baudrate = baudrates[i];
            blueToothSerial.init(baudrate);
            PRINTF("baudrate %d\n", baudrate);
            if (writeUartSafely("AT+BAUD?", 8) > 0){
                PRINTF("baudrate %d found\n", baudrate);
                return true;
            }
			PRINTF("bufLevel: %d\n", blueToothSerial.status(UART_STATUS_RX_BUF_LEVEL));
			PRINTF("rxError: %d\n", blueToothSerial.status(UART_STATUS_RX_ERROR));
            /*setupBluetooth();
            char buf[100];
            sprintf(buf, "AT+VERSION");
            if (writeUartSafely(buf, strlen(buf))){
                PRINTF("baudrate %d found\n", baudrate);
                return;
            }*/
        }
        PRINTF("baudrate not found\n");
        return false;
    }

	void setupBluetooth()
	{
		//PRINTF("bufLevel: %d\n", blueToothSerial.status(UART_STATUS_RX_BUF_LEVEL));
		//PRINTF("rxError: %d\n", blueToothSerial.status(UART_STATUS_RX_ERROR));
        //blueToothSerial.reset();

		//blueToothSerial.init(baudrate);
		//writeUartSafely("AT+RESTART", 10);
        //if (!findBaudrate()){
        //    return;
        //}
        //blueToothSerial.reset();
		//blueToothSerial.write("AT", 2);
		//char buf[100];
		//blueToothSerial.read(buf, 100);
		//PRINTF("setupBluetooth: %s\n", buf);
		//blueToothSerial.putcharNoWait('A');
		//blueToothSerial.putcharNoWait('T');
		//Thread::suspendCallerUntil(NOW() + 10*MILLISECONDS);
		//PRINTF("getchar: %c\n", blueToothSerial.getcharNoWait());
		//PRINTF("getchar: %c\n", blueToothSerial.getcharNoWait());
	
		//writeUartSafely("AT", 2);
		//suspendCallerUntil(NOW() + 400*MILLISECONDS);
		
		// CAREFUL EXECUTING THIS COMMAND, IT WILL RESET THE BLUETOOTH MODULE
		////writeUartSafely("AT+DEFAULT", 10);  // Restore all setup value to factory setup (9600 baud, name: HMSoft, pincode: 1234, 8N1)
		//suspendCallerUntil(NOW() + 2000*MILLISECONDS);
		
		//writeUartSafely("AT+NAMEGrabSat", 14);    // set the bluetooth name as "SeeedBTSlave" ,the length of bluetooth name must less than 12 characters.
		//suspendCallerUntil(NOW() + 400*MILLISECONDS);
		
		//writeUartSafely("AT+PIN3141", 10);             // set the pair code to connect 
		//suspendCallerUntil(NOW() + 400*MILLISECONDS);

		writeUartSafely("AT+RADD?", 8); 
		Thread::suspendCallerUntil(NOW() + 400*MILLISECONDS);

		writeUartSafely("AT+LADD?", 8);
		Thread::suspendCallerUntil(NOW() + 400*MILLISECONDS);

		writeUartSafely("AT+TYPE?", 8);
		Thread::suspendCallerUntil(NOW() + 400*MILLISECONDS);

		for(int i = 0; i < 2; i++){
			//writeUartSafely("AT+BAUD8", 8);
			//writeUartSafely("AT+RADD?", 8); 
			writeUartSafely("AT+ROLE?", 8);
			//Thread::suspendCallerUntil(NOW() + 10*MILLISECONDS);
		}
		
		//writeUartSafely("AT+AUTH1", 8);             // 
		//suspendCallerUntil(NOW() + 400*MILLISECONDS);  

		//blueToothSerial.flush();
	}
} hello_bluetooth;
