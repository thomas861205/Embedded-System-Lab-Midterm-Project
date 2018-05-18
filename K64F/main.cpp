#include "mbed.h"
#include "glibr.h"
#include "uLCD_4DGL.h"
#define SCREEN_SIZE 288


Serial pc(USBTX, USBRX);
Serial xbee(D9, D7);
uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
glibr GSensor(D14, D15);

int SetupIRGesture(void);
void RecognizeGesture(void);
void SetupuLCD(void);
void printCurser(int, bool);
void SetupXbee(void);
void handleXbeeSerial(void);
void handlePCSerial(void);
void reply_messange(char *, char *);
void check_addr(char *,char *);


int isr_flag = 0;
const int RAINBOW[7] = {RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET, PURPLE};


int main(void) {
    char buf[100] = {0};
    int i_xbee = 0;

    if (SetupIRGesture() == -1) return 0;

    // SetupuLCD();
    uLCD.printf(" ++++++Menu++++++\n");
    uLCD.printf(" +              +\n");
    uLCD.printf(" +LEFT :projects+\n");
    uLCD.printf(" +              +\n");
    uLCD.printf(" +UP   :crawler +\n");
    uLCD.printf(" +              +\n");
    uLCD.printf(" +RIGHT:shell   +\n");
    uLCD.printf(" +              +\n");
    uLCD.printf(" +DOWN :nothing +\n");
    uLCD.printf(" +              +\n");
    uLCD.printf(" ++++++++++++++++" );
    printCurser(0, false);

    SetupXbee();


    while (1) {


        RecognizeGesture();

        if (xbee.readable()){

            // handlePCSerial();
            handleXbeeSerial();
            wait(.001);
        }
        wait(0.001);
    }
}



int SetupIRGesture(void) {
    // Initialize Sensor with I2C
    if ( GSensor.ginit() ) {
        pc.printf("APDS-9960 initialization complete\r\n");
    } else {
        pc.printf("Something went wrong during APDS-9960 init\r\n");
        return -1;
    }

    // Start running the APDS-9960 gesture sensor engine
    if ( GSensor.enableGestureSensor(true) ) {
        pc.printf("Gesture sensor is now running\r\n");
        return 0;
    } else {
        pc.printf("Something went wrong during gesture sensor init!\r\n");
        return -1;
    }
}

void RecognizeGesture(void) {
    if ( GSensor.isGestureAvailable() ) {         // gesture detect
        switch ( GSensor.readGesture() ) {        // gesture differentiate
            case DIR_UP:
                // pc.printf("UP\r\n");

                // uLCD.printf("UP   ");
                printCurser(0, true);
                printCurser(-1, false);

                xbee.printf("UP\n");
                break;

            case DIR_DOWN:
                // pc.printf("DOWN\r\n");

                // uLCD.printf("DOWN ");
                printCurser(0, true);
                printCurser(1, false);

                xbee.printf("DOWN\n");
                break;

            case DIR_LEFT:
                // pc.printf("LEFT\r\n");

                // uLCD.printf("LEFT ");

                xbee.printf("LEFT\n");
                break;

            case DIR_RIGHT:
                // pc.printf("RIGHT\r\n");

                // uLCD.printf("RIGHT");

                xbee.printf("RIGHT\n");
                break;

            case DIR_NEAR:
                // pc.printf("NEAR\r\n");

                // uLCD.printf("NEAR ");
                
                xbee.printf("NEAR\n");
                break;

            case DIR_FAR:
                // pc.printf("FAR\r\n");
                
                // uLCD.printf("FAR  ");
                
                xbee.printf("FAR\n");
                break;

            default:
                // pc.printf("NONE\r\n");
                
                // uLCD.printf("NONE ");
                
                xbee.printf("NONE\n");
                break;
        }
    }
}


void SetupuLCD(void) {
    //Default Green on black text

    uLCD.printf("\nYour Gesture:\n"); 
    uLCD.color(RED);
    uLCD.text_width(3); //4X size text
    uLCD.text_height(3);
}

void printCurser(int offset, bool cover){
    static int _x = 0, _y = 0;

    // change setup temporarily
    uLCD.color(GREEN);
    uLCD.text_width(1);
    uLCD.text_height(1);

    // pc.printf("%d\r\n",offset);

    _y = (_y + offset) >= 0 ? _y + offset : _y + offset + 16;
    _y %= 16;
    uLCD.locate(_x, _y);
    if (cover) uLCD.printf(" ");
    else uLCD.printf(">");

    // recover the setup
    // uLCD.color(RED);
    // uLCD.text_width(3);
    // uLCD.text_height(3);
    // uLCD.locate(0, 2);

    // if (offset) pc.printf("%d\r\n", _y);
}

void SetupXbee(void) {
    pc.baud(9600);
    char xbee_reply[3];
    // XBee setting
    xbee.baud(9600);
    xbee.printf("+++");
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();

    if (xbee_reply[0] == 'O' && xbee_reply[1] == 'K') {
        pc.printf("enter AT mode.\r\n");
        xbee_reply[0] = '\0';
        xbee_reply[1] = '\0';
    }

    xbee.printf("ATMY 0x10\r\n");
    reply_messange(xbee_reply, "setting MY : 0x10");
    xbee.printf("ATDL 0x12\r\n");
    reply_messange(xbee_reply, "setting DL : 0x12");
    xbee.printf("ATWR\r\n");
    reply_messange(xbee_reply, "write config");
    xbee.printf("ATMY\r\n");
    check_addr(xbee_reply, "MY");
    xbee.printf("ATDL\r\n");
    check_addr(xbee_reply, "DL");
    xbee.printf("ATCN\r\n");
    reply_messange(xbee_reply, "exit AT mode");
    xbee.getc();
    // start
    pc.printf("start\r\n");
}

void handleXbeeSerial(void){
    static int total_char = 0;
    static int color_index = 0;
    char ch = xbee.getc();

    if (ch == '\n'){
        total_char += 18;

        if (total_char >= SCREEN_SIZE) {
            total_char %= SCREEN_SIZE;
            uLCD.cls();
            printCurser(0, false);
        }

        // total_char = (number of lines) * (char in a line)
        int number_of_lines = total_char / 18;
        total_char = number_of_lines * 18;
        uLCD.locate(total_char % 18 + 1, number_of_lines);
    }

    else if (ch == '\a'){
        total_char = 0;
        color_index = 0;
        uLCD.cls();

        printCurser(0, false);
    }

    else if (ch == '\b'){
        uLCD.cls();
        uLCD.text_height(2);
        uLCD.text_width(2);

        for (int i = 0; i < 27; i++){
            uLCD.locate(1,2);

            if (i % 3 == 0) {
                uLCD.color(YELLOW);
                uLCD.printf("Crawling .  \n");
            }
            else if (i % 3 == 1) {
                uLCD.color(ORANGE);
                uLCD.printf("Crawling  . \n");
            }
            else if (i % 3 == 2) {
                uLCD.color(RED);
                uLCD.printf("Crawling   .\n");
            }
            wait(0.5);
        }

        uLCD.color(RAINBOW[color_index]);
        printCurser(0, false);
    }

    else if (ch == '\f'){

        uLCD.cls();        
        uLCD.locate(0,0);
        uLCD.printf(" ++++++Menu++++++\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +LEFT :projects+\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +UP   :crawler +\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +RIGHT:shell   +\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +DOWN :nothing +\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" ++++++++++++++++" );

        printCurser(0, false);
    }

    else if (ch == '`'){
        uLCD.cls();        
        uLCD.locate(0,0);
        uLCD.color(WHITE);
        uLCD.printf(" ++++++Shell+++++\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +LEFT:+menu     \n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +UP   :ls      +\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" +RIGHT:        +\n");
        uLCD.printf(" +    git status+\n");
        uLCD.printf(" +DOWN :pwd     +\n");
        uLCD.printf(" +              +\n");
        uLCD.printf(" ++++++++++++++++" );

        uLCD.color(RAINBOW[color_index]);
        printCurser(0, false);

    }

    else if (ch == '\v'){
        uLCD.cls();
        uLCD.text_height(3);
        uLCD.text_width(3);
        uLCD.locate(0,2);

        uLCD.color(RED);
        uLCD.printf("D");

        uLCD.color(YELLOW);
        uLCD.printf("O");

        uLCD.color(GREEN);
        uLCD.printf("N");

        uLCD.color(PURPLE);
        uLCD.printf("E");

        wait(1);
        uLCD.color(RAINBOW[color_index]);
        printCurser(0, false);
    }

    else if (ch == '#'){
        uLCD.color(RAINBOW[color_index]);
        color_index = (color_index + 1) % 7;
    }

    else {
        uLCD.locate(total_char % 18 + 1, total_char / 18);
        uLCD.printf("%c", ch);
        total_char++;

        if (total_char == SCREEN_SIZE){
            total_char = 0;
            uLCD.cls();
            printCurser(0, false);
        }
    }
}

void handlePCSerial(void){
    static int total_char = 0;
    static int color_index = 0;
    char ch = xbee.getc();

    if (ch == '\n'){
        total_char += 18;
        pc.printf("\r\n");

        if (total_char >= SCREEN_SIZE) {
            total_char %= SCREEN_SIZE;
            uLCD.cls();
            printCurser(0, false);
        }

        // total_char = (number of lines) * (char in a line)
        int number_of_lines = total_char / 18;
        total_char = number_of_lines * 18;
        uLCD.locate(total_char % 18 + 1, number_of_lines);
    }

    else if (ch == '#'){
        uLCD.color(RAINBOW[color_index]);
        color_index = (color_index + 1) % 7;
    }

    else {
        pc.printf("%c", ch);
        uLCD.locate(total_char % 18 + 1, total_char / 18);
        uLCD.printf("%c", ch);
        total_char++;

        if (total_char == SCREEN_SIZE){
            pc.printf("\r\n");
            total_char = 0;
            uLCD.cls();
            printCurser(0, false);
        }
    }
}

void reply_messange(char *xbee_reply, char *messange) {
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();
    xbee_reply[2] = xbee.getc();

    if (xbee_reply[1] == 'O' && xbee_reply[2] == 'K') {
        pc.printf("%s\r\n", messange);
        xbee_reply[0] = '\0';
        xbee_reply[1] = '\0';
        xbee_reply[2] = '\0';
    }
}

void check_addr(char *xbee_reply, char *messenger) {
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();
    xbee_reply[2] = xbee.getc();
    pc.printf("%s = %c%c\r\n", messenger, xbee_reply[1], xbee_reply[2]);
    xbee_reply[0] = '\0';
    xbee_reply[1] = '\0';
    xbee_reply[2] = '\0';
}

