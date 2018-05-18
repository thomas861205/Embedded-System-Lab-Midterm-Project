#include "mbed.h"
#include "parallax.h"
#include "bbcar.h"
#include "mbed_rpc.h"

#include <string.h>
#include <queue>

using namespace std;

queue<char *> commands;

// DigitalOut KEY(D10);
DigitalOut KEY(PTD0);

PwmOut pin12(PTD3), pin11(PTD2), buzzer(PTC8);

RpcDigitalOut myled1(PTC9, "myled1");
RpcDigitalOut myled2(PTE6, "myled2");
RpcDigitalOut myled3(PTA11, "myled3");


Serial HC05(PTC4, PTC3);

InterruptIn button(PTD11);

Ticker execTask;
Timeout hold;
parallax_servo *servo0_ptr, *servo1_ptr;


void mode(void);
void rpcFunctions(void);

void mode(void) {
    if (KEY == 1.0) {
        // pc.printf("Set to communication mode.\r\n");
        KEY = 0;
    }
    else if (KEY == 0) {
        // pc.printf("Set to AT mode.\r\n");
        KEY = 1.0;
    }

    HC05.printf("AT+RESET\r\n");
    return;
}

void rpcFunctions(void) {
    char outbuf[100] = {0};

    if (commands.empty()) {return;}

    char *cmd = commands.front();
    commands.pop();

    if (!strcmp(cmd, "1")) {
        RPC::call("/myled1/write 0     ", outbuf);
        RPC::call("/myled2/write 1     ", outbuf);
        RPC::call("/myled3/write 1     ", outbuf);
        HC05.printf("Done 1\r\n");
        return;
    }
    else if (!strcmp(cmd, "2")) {
        RPC::call("/myled1/write 1     ", outbuf);
        RPC::call("/myled2/write 0     ", outbuf);
        RPC::call("/myled3/write 1     ", outbuf);
        HC05.printf("Done 2\r\n");
        return;
    }
    else if (!strcmp(cmd, "3")) {
        RPC::call("/myled1/write 1     ", outbuf);
        RPC::call("/myled2/write 1     ", outbuf);
        RPC::call("/myled3/write 0     ", outbuf);
        HC05.printf("Done 3\r\n");
        return;
    }
    else if (!strcmp(cmd, "UP")) {
        ServoCtrl(100);
        hold.attach(ServoStop, 0.2);
        HC05.printf("Done UP\r\n");
        return;
    }
    else if (!strcmp(cmd, "RF")) {
        ServoTurn(100, -0.1);
        hold.attach(ServoStop, 0.2);
        HC05.printf("Done RF\r\n");
        return;
    }
    // else if (!strcmp(cmd, "RIGHT")) {
    //     return;
    // }
    else if (!strcmp(cmd, "RB")) {
        ServoTurn(-100, -0.1);
        hold.attach(ServoStop, 0.2);
        HC05.printf("Done RB\r\n");
        return;
    }
    else if (!strcmp(cmd, "BACK")) {
        ServoCtrl(-100);
        hold.attach(ServoStop, 0.2);
        HC05.printf("Done BACK\r\n");
        return;
    }
    else if (!strcmp(cmd, "LB")) {
        ServoTurn(-100, 0.1);
        hold.attach(ServoStop, 0.2);
        HC05.printf("Done LB\r\n");
        return;
    }
    // else if (!strcmp(cmd, "LEFT")) {
    //     return;
    // }
    else if (!strcmp(cmd, "LF")) {
        ServoTurn(100, 0.1);
        hold.attach(ServoStop, 0.2);
        HC05.printf("Done LF\r\n");
        return;
    }
    else return;
}




int main() {
    int i = 0, j = 0;
    char str[50];
    char buf[100] = {0};
    char outbuf[100];
    execTask.attach(&rpcFunctions, 0.1);
    // setup
    // pc.baud(115200);
    HC05.baud(38400);
    KEY = 0;
    bbcar_init(pin11, pin12);
    // ready to communicate
    // pc.printf("Start\r\n");
    HC05.printf("Start\r\n");
    button.rise(&mode);
    RPC::call("\n", outbuf);
    // pc.printf("%s\r\n", outbuf);

    while (1) {
        // if (pc.readable()) { // send message
        //     char ch = pc.getc();

        //     if (ch != '\r' && ch != '\n') {
        //         pc.printf("%c", ch);
        //         str[i++] = ch;
        //     }
        //     else {
        //         pc.printf("(%d)\r\n", ch); // (13) : "ENTER"
        //         str[i] = '\0';
        //         i = 0;
        //         HC05.printf("%s\r\n", str);
        //     }
        // }

        if (HC05.readable()) { // receive message
            char ch = HC05.getc();

            if (ch != '\n') {
                buf[j++] = ch;
            }
            else {
                buf[j] = '\0';
                j = 0;
                // pc.printf("GET %s\r\n", buf);
                HC05.printf("ACK %s\r\n", buf);
                commands.push(buf);
            }
        }

        wait(.001);
    }

    return 0;
}
