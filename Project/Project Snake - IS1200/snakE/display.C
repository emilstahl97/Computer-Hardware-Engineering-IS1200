#include "include/pic32mx_io.h"
#include </opt/mcb32tools/include/pic32mx.h>

typedef unsigned char byte;

#define OLED_PAGE_MAX 4

char textbuffer[4][16];

static const byte font[] = {
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   0,   0,   0,   0,  0,
    0, 0,   0,   0,   0,   0,   0,  0, 0, 0,   0,   94,  0,   0,   0,  0,
    0, 0,   4,   3,   4,   3,   0,  0, 0, 36,  126, 36,  36,  126, 36, 0,
    0, 36,  74,  255, 82,  36,  0,  0, 0, 70,  38,  16,  8,   100, 98, 0,
    0, 52,  74,  74,  52,  32,  80, 0, 0, 0,   0,   4,   3,   0,   0,  0,
    0, 0,   0,   126, 129, 0,   0,  0, 0, 0,   0,   129, 126, 0,   0,  0,
    0, 42,  28,  62,  28,  42,  0,  0, 0, 8,   8,   62,  8,   8,   0,  0,
    0, 0,   0,   128, 96,  0,   0,  0, 0, 8,   8,   8,   8,   8,   0,  0,
    0, 0,   0,   0,   96,  0,   0,  0, 0, 64,  32,  16,  8,   4,   2,  0,
    0, 62,  65,  73,  65,  62,  0,  0, 0, 0,   66,  127, 64,  0,   0,  0,
    0, 0,   98,  81,  73,  70,  0,  0, 0, 0,   34,  73,  73,  54,  0,  0,
    0, 0,   14,  8,   127, 8,   0,  0, 0, 0,   35,  69,  69,  57,  0,  0,
    0, 0,   62,  73,  73,  50,  0,  0, 0, 0,   1,   97,  25,  7,   0,  0,
    0, 0,   54,  73,  73,  54,  0,  0, 0, 0,   6,   9,   9,   126, 0,  0,
    0, 0,   0,   102, 0,   0,   0,  0, 0, 0,   128, 102, 0,   0,   0,  0,
    0, 0,   8,   20,  34,  65,  0,  0, 0, 0,   20,  20,  20,  20,  0,  0,
    0, 0,   65,  34,  20,  8,   0,  0, 0, 2,   1,   81,  9,   6,   0,  0,
    0, 28,  34,  89,  89,  82,  12, 0, 0, 0,   126, 9,   9,   126, 0,  0,
    0, 0,   127, 73,  73,  54,  0,  0, 0, 0,   62,  65,  65,  34,  0,  0,
    0, 0,   127, 65,  65,  62,  0,  0, 0, 0,   127, 73,  73,  65,  0,  0,
    0, 0,   127, 9,   9,   1,   0,  0, 0, 0,   62,  65,  81,  50,  0,  0,
    0, 0,   127, 8,   8,   127, 0,  0, 0, 0,   65,  127, 65,  0,   0,  0,
    0, 0,   32,  64,  64,  63,  0,  0, 0, 0,   127, 8,   20,  99,  0,  0,
    0, 0,   127, 64,  64,  64,  0,  0, 0, 127, 2,   4,   2,   127, 0,  0,
    0, 127, 6,   8,   48,  127, 0,  0, 0, 0,   62,  65,  65,  62,  0,  0,
    0, 0,   127, 9,   9,   6,   0,  0, 0, 0,   62,  65,  97,  126, 64, 0,
    0, 0,   127, 9,   9,   118, 0,  0, 0, 0,   38,  73,  73,  50,  0,  0,
    0, 1,   1,   127, 1,   1,   0,  0, 0, 0,   63,  64,  64,  63,  0,  0,
    0, 31,  32,  64,  32,  31,  0,  0, 0, 63,  64,  48,  64,  63,  0,  0,
    0, 0,   119, 8,   8,   119, 0,  0, 0, 3,   4,   120, 4,   3,   0,  0,
    0, 0,   113, 73,  73,  71,  0,  0, 0, 0,   127, 65,  65,  0,   0,  0,
    0, 2,   4,   8,   16,  32,  64, 0, 0, 0,   0,   65,  65,  127, 0,  0,
    0, 4,   2,   1,   2,   4,   0,  0, 0, 64,  64,  64,  64,  64,  64, 0,
    0, 0,   1,   2,   4,   0,   0,  0, 0, 0,   48,  72,  40,  120, 0,  0,
    0, 0,   127, 72,  72,  48,  0,  0, 0, 0,   48,  72,  72,  0,   0,  0,
    0, 0,   48,  72,  72,  127, 0,  0, 0, 0,   48,  88,  88,  16,  0,  0,
    0, 0,   126, 9,   1,   2,   0,  0, 0, 0,   80,  152, 152, 112, 0,  0,
    0, 0,   127, 8,   8,   112, 0,  0, 0, 0,   0,   122, 0,   0,   0,  0,
    0, 0,   64,  128, 128, 122, 0,  0, 0, 0,   127, 16,  40,  72,  0,  0,
    0, 0,   0,   127, 0,   0,   0,  0, 0, 120, 8,   16,  8,   112, 0,  0,
    0, 0,   120, 8,   8,   112, 0,  0, 0, 0,   48,  72,  72,  48,  0,  0,
    0, 0,   248, 40,  40,  16,  0,  0, 0, 0,   16,  40,  40,  248, 0,  0,
    0, 0,   112, 8,   8,   16,  0,  0, 0, 0,   72,  84,  84,  36,  0,  0,
    0, 0,   8,   60,  72,  32,  0,  0, 0, 0,   56,  64,  32,  120, 0,  0,
    0, 0,   56,  64,  56,  0,   0,  0, 0, 56,  64,  32,  64,  56,  0,  0,
    0, 0,   72,  48,  48,  72,  0,  0, 0, 0,   24,  160, 160, 120, 0,  0,
    0, 0,   100, 84,  84,  76,  0,  0, 0, 0,   8,   28,  34,  65,  0,  0,
    0, 0,   0,   126, 0,   0,   0,  0, 0, 0,   65,  34,  28,  8,   0,  0,
    0, 0,   4,   2,   4,   2,   0,  0, 0, 120, 68,  66,  68,  120, 0,  0,
};

// WHITE 0 <----> 255 BLACK
const byte icon[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void sleep(int ms) {
  while (ms > 0)
    ms--;
}

byte spi_put_byte(byte data) {
  while (!(SPI2STAT & (1 << 3)))
    ;             // WHILE SPI2TXB IS EMPTY, TRANSMIT BUFFER OF SPI2BUF
  SPI2BUF = data; // SPI DATA/RECEIVE BUFFER = DATA
  while (!(SPI2STAT & (1 << 0)))
    ; // WHILE SPI2RBF IS EMPTY, RECEIVE BUFFER OF SPI2BUF
  return SPI2BUF;
}

void oled_host_init() {
  // SET PERIPHERAL BUS CLOCK
  OSCCONCLR = 1 < 19;
  OSCCONCLR = 1 < 20;
  OSCCONSET = 1 < 19;
  // Gör ingen skillnad?

  AD1PCFG = 1; // SET DIGITAL PIN

  // SET PINS FOR DISPLAY SIGNALS
  PORTF = 0xFFFF;
  ODCF = 0x0;        // SET NORMAL DIGITAL OUTPUT (ELSE OPEN DRAIN OUTPUT)
  ODCG = 0x0;        // SET NORMAL DIGITAL OUTPUT (ELSE OPEN DRAIN OUTPUT)
  TRISFCLR = 1 << 4; // SET VDD TO OUTPUT
  TRISFCLR = 1 << 5; // SET VBAT TO OUTPUT
  TRISFCLR = 1 << 6; // SET DATA/CMD TO OUTPUT
  TRISGCLR = 1 << 9; // SET RESET TO OUTPUT

  // SET SPI CONTROLLER
  SPI2CON = 0;          // CLEAR SPI CONTROLLER PORT 2
  SPI2BRG = 1 << 2;     // BAUD RATE DIVISOR 64
  SPI2STATCLR = 1 << 6; // CLEAR SPIROV OVERFLOW FLAG
  SPI2CONSET = 1 << 5;  // SET CKP CLOCK POLARITY, STATE: ACTIVE LOW, IDLE HIGH
  SPI2CONSET = 1 << 6;  // SET MSTEN BIT, 1 = MASTER, ELSE SLAVE
  SPI2CONSET = 1 << 15; // ENABLE SPI
}

void oled_disp_init() {
  PORTFCLR = 1 << 4; // CLEAR DATA/CMD BIT
  sleep(100);
  PORTFCLR = 1 << 6; // TURN VDD ON
  sleep(1000000);    // WAIT

  spi_put_byte(0xAE); // DISPLAY OFF COMMAND

  PORTGCLR = 1 << 9;
  sleep(10);
  PORTGSET = 1 << 9; // RESET THE RESET BIT
  sleep(10);

  spi_put_byte(0x8D);
  spi_put_byte(0x14); // SET CHARGE PUMP/DC-DC COMMAND

  spi_put_byte(0xD9);
  spi_put_byte(0xF1); // SET PRE-CHARGE PERIOD COMMAND

  PORTFCLR = 1 << 5; // TURN VCC ON
  sleep(1000000);    // WAIT

  // COMMANDS TO INVERT DISPLAY. SETS DISPLAY ORIGIN TO UPPER LEFT CORNER
  spi_put_byte(0xA1); // REMAP COLUMNS
  spi_put_byte(0xC8); // SET COM OUTPUT SCAN DIRECTION
  spi_put_byte(0xDA); // SET COM PINS HARDWARE CONFIG
  spi_put_byte(0x20); // SEQUENTIAL COM, LEFT/RIGHT REMAP ENABLED
  spi_put_byte(0xAF); // DISPLAY ON COMMAND
}

void oled_update() {
  int x, y, c, k;

  for (x = 0; x < OLED_PAGE_MAX; x++) {
    PORTFCLR = 1 << 4; // CLEAR DATA/CMD

    // SET PAGE ADDRESS
    spi_put_byte(0x22); // SET PAGE COMMAND
    spi_put_byte(x);    // PAGE NUMBER

    // START AT LEFT COLUMN
    spi_put_byte(0x00); // SET COLUMN LOW NIBBLE
    spi_put_byte(0x10); // SET COLUMN HIGH NIBBLE

    PORTFSET = 1 << 4; // SET DATA/CMD

    for (y = 0; y < 16; y++) {
      c = textbuffer[x][y];

      if (c & (1 << 7))
        continue;

      for (k = 0; k < 8; k++)
        spi_put_byte(font[c * 8 + k]);
    }
  }
}

void print_string(int line, char *string) {
  int i;

  if (line < 0 || line >= 4 || !string)
    return;

  for (i = 0; i < 16; i++) {
    if (*string) {
      textbuffer[line][i] = *string;
      string++;
    } else
      textbuffer[line][i] = ' ';
  }
}

void print_image(int line, const byte *data) {
  int i, j;

  for (i = 0; i < 4; i++) {
    PORTFCLR = 1 << 4; // CLEAR CMD/DATA

    spi_put_byte(0x22); // SET PRE-CHARGE PERIOD
    spi_put_byte(
        i); // SET MEM ADDRESSING MODE. 0b00 = HORI, 0b01 = VERT, 0b10 =
            // PAGE, 0b11 = INVALID

    spi_put_byte(line & 0xF);
    spi_put_byte(0x10 | ((line >> 4) & 0xF)); // SET HIGH COLUMN ADDRESS

    PORTFSET = 1 << 4; // SET CMD/DATA

    for (j = 0; j < 32; j++) {
      spi_put_byte(~data[i * 32 + j]);
      // sleep(100000);
    }
  }
}

int main(void) {
  en_btn(1, SET);
  en_sw(1, SET);

  PORTE = 0x0000;
  TRISE = 0;

  oled_host_init();
  oled_disp_init();

  // print_string(0, "such world");
  // print_string(1, "i am doge");
  // print_string(2, "many text");
  oled_update();
  print_image(1, icon);

  // while (1) {
  //   if (btn_pressed(1)) {
  //     print_image(96, icon);
  //     en_led(1, SET);
  //     oled_update();
  //   } else {
  //     en_led(1, CLR);
  //   }
  //
  //   if (sw_toggled(1)) {
  //     print_string(3, "wow");
  //     oled_update();
  //   } else
  //     print_string(3, "much hello");
  //   oled_update();
  // }

  return 0;
}
