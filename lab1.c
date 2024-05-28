//Lab1 Task1
void main() {
  TRISC = 0;           // set direction to be output
  do {
    LATA = 0x00;       // Turn OFF LEDs on PORTA
    LATB = 0x00;       // Turn OFF LEDs on PORTB
    LATC = 0x00;       // Turn OFF LEDs on PORTC
    LATD = 0x00;       // Turn OFF LEDs on PORTD
    LATE = 0x00;       // Turn OFF LEDs on PORTE
    Delay_ms(1000);    // 1 second delay

    LATA = 0xFF;       // Turn ON LEDs on PORTA
    LATB = 0xFF;       // Turn ON LEDs on PORTB
    LATC = 0xFF;       // Turn ON LEDs on PORTC
    LATD = 0xFF;       // Turn ON LEDs on PORTD
    LATE = 0xFF;       // Turn ON LEDs on PORTE
    Delay_ms(1000);    // 1 second delay
  } while(1);          // Endless loop
}

//Lab1 Task2
// This function takes a decimal number as input and converts it to a binary number.
int covertDecimaltoBinary(int n){
    int binaryNumber = 0;
    int remainder, i = 1;
    while(n != 0){
    remainder = n % 2; // Find the remainder when dividing n by 2
    n /= 2; // Divide n by 2 and store the result
    binaryNumber += remainder*i; // Multiply the remainder by i and add it to the binary number
    i *= 10; // Multiply i by 10 to move to the next place value in the binary number
    }
    return binaryNumber; // Return the binary number
}

void main() {

    TRISC = 0; // Set the direction of PORTC to be output

    do {
    int randomNumber = rand()%256; // Generate a random integer between 1 and 255
    LATA = 0x00; // Turn OFF LEDs on PORTA
    LATB = 0x00; // Turn OFF LEDs on PORTB
    LATC = covertDecimaltoBinary(randomNumber); // Convert the random integer to binary and turn on the corresponding LEDs on PORTC
    LATD = 0x00; // Turn OFF LEDs on PORTD
    LATE = 0x00; // Turn OFF LEDs on PORTE
    Delay_ms(1000); // Delay for 1 second

    } while(1); // Endless loop
}
//Lab1 Task3
void main() {

    TRISA = 0; // set direction of PORTA as output
    TRISB = 0xFF; // set direction of PORTB as input
    TRISC = 0x00; // set direction of PORTC as output
    TRISD = 0; // set direction of PORTD as output
    TRISE = 0; // set direction of PORTE as output
    PORTC = 0xff; // turn on all LEDs connected to PORTC

    do {
        PORTC = ~PORTB; // toggle the LEDs connected to PORTC based on the value of PORTB
        } while(1); // Endless loop
}
//Lab1 Task4
// Function to get the 7-segment display code for a given number
char getDigitCode(int number) {
    char digitCode;

    switch(number) {
        case 0:
            digitCode = 0b00111111;   // Display 0
            break;
        case 1:
            digitCode = 0b00000110;   // Display 1
            break;
        case 2:
            digitCode = 0b01011011;   // Display 2
            break;
        case 3:
            digitCode = 0b01001111;   // Display 3
            break;
        case 4:
            digitCode = 0b01100110;   // Display 4
            break;
        case 5:
            digitCode = 0b01101101;   // Display 5
            break;
        case 6:
            digitCode = 0b01111101;   // Display 6
            break;
        case 7:
            digitCode = 0b00000111;   // Display 7
            break;
        case 8:
            digitCode = 0b01111111;   // Display 8
            break;
        case 9:
            digitCode = 0b01101111;   // Display 9
            break;
        default:
            digitCode = 0x00;   // Display blank
            break;
    }

    return digitCode;
}

// Main function
void main() {
  ANSELD = 0;   // Set PORTD to digital mode
  TRISD = 0;    // Set PORTD as output
  TRISA = 0xFF; // Set PORTA as input
  unsigned char pattern = 0;
  unsigned char cnt = 0;
  for(;;) {
     // Check if switch on PORTB is pressed
     if(PORTB.F0 == 1) {
         pattern = getDigitCode(rand() % 10); // Generate random number between 0 and 9
         PORTD = pattern; // Display the corresponding digit on 7-segment display
         Delay_ms(1000);
     }
     else {
         PORTD = 0; // Turn off 7-segment display
     }
  }  // Endless loop
}
//Lab1 Task5
// Function to retrieve the seven-segment display pattern for a given number
char getDigitCode(int number);

void main() {
    int count = 0;
    unsigned char Pattern = 0, cnt = 0, switchCount = 0;
    // Array to store the seven-segment display patterns for each digit
    unsigned char SEGMENT[] = {0x3f,0x06,0x5B,0x4F,
                               0x66,0x6D,0x7D,0x07,
                               0x7F,0x6F};
    // Configure the required pins as output
    ANSELD = 0;
    TRISD = 0;
    // Configure PORTC as input
    TRISC = 0xFF;

    while(1) {
        // Iterate over each bit of PORTC to count the number of switches pressed
        count = 0;
        while(count < 8) {
            if(PORTC & (1 << count)) {
                switchCount++;
            }
            count ++;
        }
        // Retrieve the seven-segment display pattern for the count of switches pressed
        Pattern = getDigitCode(switchCount % 10);
        // Display the pattern on the first digit of the seven-segment display
        PORTD = Pattern;
        // Delay to display the pattern
        Delay_ms(5);
        // Clear the switch count for the next iteration
        switchCount = 0;
    }
}
char getDigitCode(int number) {
    char digitCode;

    switch(number) {
        case 0:
            digitCode = 0b00111111;   // Display 0
            break;
        case 1:
            digitCode = 0b00000110;   // Display 1
            break;
        case 2:
            digitCode = 0b01011011;   // Display 2
            break;
        case 3:
            digitCode = 0b01001111;   // Display 3
            break;
        case 4:
            digitCode = 0b01100110;   // Display 4
            break;
        case 5:
            digitCode = 0b01101101;   // Display 5
            break;
        case 6:
            digitCode = 0b01111101;   // Display 6
            break;
        case 7:
            digitCode = 0b00000111;   // Display 7
            break;
        case 8:
            digitCode = 0b01111111;   // Display 8
            break;
        case 9:
            digitCode = 0b01101111;   // Display 9
            break;
        default:
            digitCode = 0x00;   // Display blank
            break;
    }

    return digitCode;
}

