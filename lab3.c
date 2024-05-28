// Task1

// Declare global variables
char number[6];
int D1 = 0, D2 = 0, D3 = 0, D4 = 0;
int i = 0, j = 0;
int flag = 0;
unsigned char Segment[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int uart_rd;
int tagForEnter = 0, tagForDigital = 0;

// Function to update hours
void setHour() {
   // Increment hour units
   D2++;
   if(D2 >= 10){
      // Increment hour tens and reset units
      D1++;
      D2 = 0;
   }
   // Reset both tens and units when reaching 24 hours
   if(D1 >= 2 && D2 >= 4){
      D1 = 0;
      D2 = 0;
   }
}

// Function to update minutes
void setMinute() {
   // Increment minute units
   D4++;
   if(D4 >= 10){
      // Increment minute tens and reset units
      D3++;
      D4 = 0;
   }
   // Reset both tens and units when reaching 60 minutes
   if(D3 >= 6 && D4 >= 0){
      D3 = 0;
      D4 = 0;
      setHour();  // Update hours when minutes reset
   }
}

// Interrupt handler function
void interrupt() {
  if(INTCON.TMR0IF == 1){
     TMR0L = 100; //
     INTCON.TMR0IF = 0;
     if(2000<= j){
        setMinute();
        j = 0;
     }
     else j++;

     // Update the 7-segment display based on the current flag value
     switch(flag) {
        case 0:
            LATA = 0x08;
            PORTD = Segment[D1];
            flag = 1;
            break;
        case 1:
            LATA = 0x04;
            PORTD = Segment[D2];
            flag = 2;
            break;
        case 2:
            LATA = 0x02;
            PORTD = Segment[D3];
            flag = 3;
            break;
        case 3:
            LATA = 0x01;
            PORTD = Segment[D4];
            flag = 0;
            break;
     }
  }
}

// Main function
void main() {
   // Configure all pins as digital I/O
   ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = 0;

   // Configure PORTB, PORTA, and PORTD as outputs
   TRISB = TRISA = TRISD = 0x00;

   PORTB = 0x06;
   T0CON = 0xC4;
   GIE_bit = 1;
   TMR0IE_bit = 1;
   RCSTA1.CREN = 1;

   // Initialize UART module at 9600 bps
   UART1_Init(9600);
   Delay_ms(100);  // Wait for UART module to stabilize

   // Send "Hello World!" message via UART
   UART1_Write_Text("Hello World!");
   UART1_Write(13);
   UART1_Write(10);
    // Main loop
    while(1) {
    // Check if data is received through UART
    if (UART1_Data_Ready()) {
    tagForEnter = 0;
    uart_rd = UART1_Read(); // Read the received data
    number[i] = uart_rd;
    i++;
    1_Write(10);
    // Main loop
    while(1) {
    // Check if data is received through UART
    if (UART1_Data_Ready()) {
    tagForEnter = 0;
    uart_rd = UART1_Read(); // Read the received data
    number[i] = uart_rd;
    i++;
          // Initialize I2C communication
      I2C1_Init(100000);
      I2C1_Start();
      I2C1_Wr(0xA2);             // Send byte via I2C (device address + W)
      I2C1_Wr(2);                // Send byte (address of EEPROM location)
      I2C1_Wr(uart_rd);          // Send data (data to be written)
      I2C1_Stop();               // Issue I2C stop signal

      I2C1_Start();              // Issue I2C start signal
      I2C1_Wr(0xA2);             // Send byte via I2C (device address + W)
      I2C1_Wr(2);                // Send byte (data address)
      I2C1_Repeated_Start();     // Issue I2C signal repeated start
      I2C1_Wr(uart_rd);          // Send byte (device address + R)
      I2C1_Stop();               // Issue I2C stop signal

      // Convert the received character to the corresponding digit or letter
      if(48 <= uart_rd && uart_rd <= 57) uart_rd  = uart_rd - '0';
      if(65 <= uart_rd && uart_rd <= 90)  uart_rd  = uart_rd - 'A' + 10;
      if(97 <= uart_rd && uart_rd <= 122) uart_rd  = uart_rd - 'a' + 10;

      // Update the 7-segment display based on the received data
      switch(tagForDigital) {
          case 0:
              D1 = 0, D2 = 0, D3 = 0, D4 = 0;
              D1 = uart_rd;
              tagForDigital = 1;
              break;
          case 1:
              if(uart_rd == 46) {
                  tagForDigital = 0;
                  tagForEnter = 0;
                  i = 0;
                  D4 = D1;
                  D1 = 0;
                  UART1_Write_Text("Your message is :");
                  UART1_Write(number[0]);
                  UART1_Write(13);
                  UART1_Write(10);
                  break;
              }
              D2 = uart_rd;
              tagForDigital = 2;
              break;
          case 2:
              if(uart_rd == 46) {
                  tagForDigital = 0;
                  tagForEnter = 0;
                  i = 0;
                  D3 = D1;
                  D4 = D2;
                  D1 = 0;
                  D2 = 0;
                  UART1_Write_Text("Your message is :");
                  UART1_Write(number[0]);
                  UART1_Write(number[1]);
                  UART1_Write(13);
                  UART1_Write(10);
                  break;
              }
              D3 = uart_rd;
              tagForDigital = 3;
              break;
          case 3:
              if(uart_rd == 46) {
                    D4 = D3;
                    D3 = D2;
                    D2 = D1;
                    D1 = 0;
                    tagForDigital = 0;
                    tagForEnter = 0;
                    i = 0;
                    UART1_Write_Text("Your messageis :");
                    UART1_Write(number[0]);
                    UART1_Write(number[1]);
                    UART1_Write(number[2]);
                    UART1_Write(13);
                    UART1_Write(10);
                    break;
                }
                D4 = uart_rd;
                tagForDigital = 4;
                break;
         case 4:
                i = 0;
                tagForEnter = 0;
                tagForDigital = 0;
                UART1_Write_Text("Your message is :");
                UART1_Write(number[0]);
                UART1_Write(number[1]);
                UART1_Write(number[2]);
                UART1_Write(number[3]);
                UART1_Write(13);
                UART1_Write(10);
                break;
            }
        }
    }
}
