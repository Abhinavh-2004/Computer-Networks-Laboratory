#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define TOTAL_PACKETS 6
void sendPackets(int num){
	printf("Sent the packet {P%d}\n",num);
	sleep(1);
}

void recievePacket(int num){
	printf("Recieved the packet {P%d}\n",num);
	sleep(1);
}

void sendAck(int num){
	printf("Sent the acknowledgement {P%d}\n",num);
	sleep(1);
}


void slidingWindowProtocol(int windowSize){
	int windowStart = 0;
	while(windowStart < TOTAL_PACKETS){
		printf("Window start: %d\n", windowStart);
		for(int i = windowStart; i < windowStart + windowSize && i < TOTAL_PACKETS; i++){
			sendPackets(i+1);
		}
		for(int i = windowStart; i < windowStart + windowSize && i < TOTAL_PACKETS; i++){
			recievePacket(i+1);
			sendAck(i+1);
		}
		windowStart += windowSize;
	}
}
		 
	

int main(){
	int windowSize = 0;
	printf("Enter the window size: ");
	scanf("%d",&windowSize);
	if (windowSize < 1 || windowSize > TOTAL_PACKETS) {
        	printf("Invalid window size. It must be between 1 and %d.\n", TOTAL_PACKETS);
        	return 1;
    	}
	slidingWindowProtocol(windowSize);
	return 0;
}
