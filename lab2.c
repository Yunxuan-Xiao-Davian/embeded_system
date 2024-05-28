// Digital Clock using PIC18F45K22 and 4-digit 7-segment display
#include <stdint.h>

// Global variables
//A number of global variables are defined, such as hours, minutes, 
//flag bits etc., which are used to track and manipulate the state of the clock throughout the program.
int switch_time_flag = 0;
int hour_left = 2;
int hour_right = 2;
int minute_left = 0;
int minute_right = 0;
int flag = 0;
int flash = 0;
int state = 0;
unsigned char Segment[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};//This is an array that stores the code corresponding to each digit (0-9) of the 7-segment display
uint8_t pre_clock[] = {0, 0, 0, 0};//The pre_clock[] array stores the previous clock values.
// Function to handle flashing effect for hour and minute setting modes
void handleFlashingEffect() {
    if (switch_time_flag == 1) {
        flash = 1;
        Delay_Ms(500);
        LATA = 0x00;
        flag = 0;
        flash = 0;
        Delay_Ms(500);
        if (PORTB.F1 == 1) {
            switch_time_flag = 2;
        }
    }
    if (switch_time_flag == 2) {
        flash = 1;
        Delay_Ms(500);
        LATA = 0x00;
        flag = 0;
        flash = 0;
        Delay_Ms(500);
        if (PORTB.F1 == 1) {
            switch_time_flag = 0;
            state = 0;
        }
    }
    if (switch_time_flag == 3) {
        flash = 1;
        Delay_Ms(500);
        LATA = 0x00;
        flag = 0;
        flash = 0;
        Delay_Ms(500);
        if (PORTB.F2 == 1) {
            switch_time_flag = 4;
        }
    }

    if (switch_time_flag == 4) {
        flash = 1;
        Delay_Ms(500);
        LATA = 0x00;
        flag = 0;
        flash = 0;
        Delay_Ms(500);
        if (PORTB.F2 == 1) {
            switch_time_flag = 0;
            state = 1;
        }
    }
}
// Function to exit hour and minute setting modes
void exitSettingModes() {
    if (switch_time_flag == 1 && PORTB.F1 == 1) {
        switch_time_flag = 2;
    }
    if (switch_time_flag == 2 && PORTB.F1 == 1) {
        switch_time_flag = 0;
        state = 0;
    }
    if (switch_time_flag == 3 && PORTB.F2 == 1) {
        switch_time_flag = 4;
    }
    if (switch_time_flag == 4 && PORTB.F2 == 1) {
        switch_time_flag = 0;
        state = 1;
    }
}
void setHour() {
    // Add a delay to prevent bouncing effect
    Delay_Ms(300);
    hour_right++;  // Increment hour and handle rollover
    if (hour_right >= 10) {
        hour_left++;
        hour_right = 0;
    }
    if (hour_left >= 2 && hour_right >= 4) {
        hour_left = 0;
        hour_right = 0;
    }
}

// Function to set the minute
void setMinute() {
    // Add a delay to prevent bouncing effect
    Delay_Ms(300);
    minute_right++;  // Increment minute and handle rollover
    if (minute_right >= 10) {
        minute_left++;
        minute_right = 0;
    }
    if (minute_left >= 6 && minute_right >= 0) {
        minute_left = 0;
        minute_right = 0;
        setHour();
    }
}

//the interrupt function
void interrupt() {
    PORTB = 0x00;
    if (INTCON.TMR0IF == 1) {
        TMR0L = 100;  //
        INTCON.TMR0IF = 0;

        if (switch_time_flag == 5) {
            Sound_Init(&PORTE,            1);
            Sound_Play(880, 1000);
            switch_time_flag = 0;
            hour_left = pre_clock[0];
            hour_right = pre_clock[1];
            minute_left = pre_clock[2];
            minute_right = pre_clock[3];
            state = 0;
        }

        // Display hour and minute segments with flashing effect based on switch_time_flag
        if ((switch_time_flag == 1 || switch_time_flag == 3) && flash == 1) {
            switch (flag) {
                case 0:
                    LATA = 0x08;
                    PORTD = Segment[hour_left];
                    flag = 1;
                    break;
                case 1:
                    LATA = 0x04;
                    PORTD = Segment[hour_right] | 128;
                    flag = 0;
                    break;
            }
        }

        if ((switch_time_flag == 2 || switch_time_flag == 4) && flash == 1) {
            switch (flag) {
                case 0:
                    LATA = 0x02;
                    PORTD = Segment[minute_left];
                    flag = 1;
                    break;
                case 1:
                    LATA = 0x01;
                    PORTD = Segment[minute_right];
                    flag = 0;
                    break;
            }
        }
    }

    // Check for button press to set hour or minute
    if (INTCON.INT0IF == 1) {
        switch (switch_time_flag) {
            case 1:
                setHour();
                break;
            case 2:
                setMinute();
                break;
            case 3:
                setHour();
                break;
            case 4:
                setMinute();
                break;
        }
        INTCON.INT0IF = 0;
    }
}
// Function to decrement the minute
void alarm() {
    minute_right--;
    pre_clock[3]++;
    if (pre_clock[3] >= 10) {
        pre_clock[2]++;
        pre_clock[3] = 0;
    }
    if (pre_clock[2] >= 6 && pre_clock[3] >= 0) {
        pre_clock[2] = 0;
        pre_clock[3] = 0;
        pre_clock[1]++;
    }
    if (pre_clock[1] >= 10) {
        pre_clock[0]++;
        pre_clock[1] = 0;
    }
    if (pre_clock[0] >= 2 && pre_clock[1] >= 4) {
        pre_clock[0] = 0;
        pre_clock[1] = 0;
        pre_clock[2] = 0;
        pre_clock[3] = 0;
    }
    if (minute_right < 0) {
        minute_left--;
        minute_right = 9;
    }
    if (minute_left < 0) {
        hour_right--;
        minute_left = 5;
        minute_right = 9;
    }
    if (hour_right < 0) {
        hour_left--;
        hour_right = 9;
        minute_left = 6;
        minute_right = 0;
    }
    if (hour_left < 0) {
        switch_time_flag = 5;
        state = 2;
    }
}
void main() {
    // Initialize ports and set ANSELx registers to digital
    ANSELA = 0;
    ANSELB = 0;
    ANSELD = 0;
    ANSELE = 0;

    TRISA = 0x00;
    TRISB = 0xFF;
    TRISD = 0x00;

    PORTA = 0x0F;
    PORTB = 0x00;
    PORTD = 0x00;

    // Timer0 configuration
    T0CON = 0xC4;
    GIE_bit = 1;
    TMR0IE_bit = 1;

    while (1) {
        if (switch_time_flag == 0) {
            if (state == 0) {
                setMinute();
                pre_clock[0] = hour_left;
                pre_clock[1] = hour_right;
                pre_clock[2] = minute_left;
                pre_clock[3] = minute_right;
            } else if (state == 1) {
                alarm();
            }
            Delay_Ms(1000);

            // Check button press to enter hour or minute setting mode
            if (PORTB.F1 == 1) {
                switch_time_flag = 1;
            }
            if (PORTB.F2 == 1) {
                switch_time_flag = 3;
            }
        }

        // Handle flashing effect for hour and minute setting modes
        handleFlashingEffect();

        // Exit hour and minute setting modes
        exitSettingModes();
    }
}
