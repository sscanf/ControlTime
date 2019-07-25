//                           1001   Graphic   Text    Curser_enable   Blink_enable

/* ----------------------------------------------------------
 * Program to control a T6963C-based 240x64 pixel LCD display
 * using the PC's Parallel Port (LPT1:) in bidirectional mode
 * written in Microsoft Quick C
 *
 * Written by John P. Beale May 3-4, 1997  beale@best.com
 *
 *  Based on information from Steve Lawther,
 *  "Writing Software for T6963C based Graphic LCDs", 1997 which is at
 *  http://ourworld.compuserve.com/homepages/steve_lawther/t6963c.pdf
 *
 *  and the Toshiba T6963C data sheet, also on Steve's WWW page
 *
 *  and info at: http://www.citilink.com/~jsampson/lcdindex.htm
 *               http://www.cs.colostate.edu/~hirsch/LCD.html
 *               http://www.hantronix.com/
 * ----------------------------------------------------------
 */

#include <mega32.h>
#include <delay.h>
#include <stdio.h>
#include <stdlib.h>     // rand()
#include <string.h>     // strlen()
#include <math.h>       // cos(),sin()

/*
 *  FG                (1)  frame ground
 *  GND               (2)  ground 
 *  VDD               (3)  LCD logic supply  +5 DC
 *  VO                (4)  Contrast
 *  /WR               (5)  <--> 
 *  /RD               (6)  <--> 
 *  /CE or /CS        (7)  <--> 
 *  CD or RS          (8)  <-->
 *  /RST              (9)  <-->   

 *  D0                (10) <--> 
 *  D1                (11) <--> 
 *  D2                (12) <--> 
 *  D3                (13) <--> 
 *  D4                (14) <--> 
 *  D5                (15) <--> 
 *  D6                (16) <--> 
 *  D7                (17) <--> 

 *  FS                (18)           font select       HI : narrow            LOW : Wide   ( normal )
                                                       40 with 6x8 font       30 with 8x8 font 
 *  Vout              (19)           Variable Resistor with GND and centre with VO ( Pin 4 )
 *  BLA               (20)           Back light +
 *  BLK               (21)           Back light -
 *  DSPON             (22)           I don't Know   but i think means display ON or ...
 */



#define WRHI   PORTD.4=1
#define WRLO   PORTD.4=0
#define WR_DDR DDRD.4

#define RDHI   PORTD.3=1
#define RDLO   PORTD.3=0
#define RD_DDR DDRD.3

#define CEHI   PORTD.2=1
#define CELO   PORTD.2=0
#define CE_DDR DDRD.2


#define CDHI   PORTD.0=1
#define CDLO   PORTD.0=0
#define CD_DDR DDRD.0



#define RESETHI   PORTD.7=1
#define RESETLO   PORTD.7=0
#define RESET_DDR DDRD.7


#define DATAIN  DDRC=0
#define DATAOUT DDRC=0XFF

#define DATAPIN  PINC
#define DATAPORT PORTC

/* ----- Definitions concerning LCD internal memory  ------ */

#define G_BASE 0x0200            // base address of graphics memory
#define T_BASE 0x0000            // base address of text memory
#define BYTES_PER_ROW 30         // how many bytes per row on screen

/* ----------------------------------------------------------- */


void dput(int byte); 			 // write data byte to LCD module
int  dget(void);    			 // get data byte from LCD module
int  sget(void);  			 // check LCD display status pbrt
void cput(int byte);			 // write command byte to LCD module
void lcd_setup();			 // make sure control lines are at correct levels
void lcd_init();     			 // initialize LCD memory and display modes
void lcd_print(int,int,flash char *string);  	 // send string of characters to LCD
void lcd_print_ram(int,int,char *string);  	 // send string of characters to LCD
void lcd_set_address(unsigned int addr);
void lcd_clear_graph();    		 // clear graphics memory of LCD
void lcd_clear_text();  		 // clear text memory of LCD
void lcd_xy(int x, int y); 		 // set memory pointer to (x,y) position (text)
void lcd_clrpixel(int column, int row);  // set single pixel in 240x64 array
void lcd_setpixel(int column, int row);  // set single pixel in 240x64 array
void lcd_pixel(int column, int row,char show);

void show(flash char *,int,int);
void lcd_line(int x1, int y1, int x2, int y2, unsigned char show);
void lcd_circle(int x, int y, int radius, unsigned char show);
void lcd_circle_half(int x, int y, int radius, unsigned char show);
void lcd_box(int x1, int y1, int x2, int y2, unsigned char show);
void lcd_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
void lcd_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
void lcd_fill(int x1, int y1, int x2, int y2, unsigned char persent,char first);




#define XMAX 240        // limits of (x,y) LCD graphics drawing
#define XMIN 0
#define YMAX 128
#define YMIN 0





void lcd_set_address(unsigned int addr){
dput(addr%256);
dput(addr>>8);
cput(0x24);
}



void lcd_clear_graph()    // clear graphics memory of LCD
{
int i;
lcd_set_address(G_BASE);     // addrptr at address G_BASE
for (i=0;i<3840;i++) {   // must be  3840 ?????????????????     5120
	dput(0); cput(0xc0);               // write data, inc ptr.
	}	 // end for(i)
} // end lcd_clear_graph()

//----------------------------------------------------------------

void lcd_clear_text()
{
 int i;   
 lcd_set_address(T_BASE);     // addrptr at address T_BASE

 for (i=0;i<480;i++) {  //  must be 480   ???????????     640  
      dput(0); cput(0xc0);               // write data, inc ptr.
 } // end for(i)

} // lcd_clear_text()

//-------------------------------------------------------------------------------


void lcd_print(int x,int y,flash char *string)  // send string of characters to LCD
{
int i;
int c;
lcd_xy(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     // convert ASCII to LCD char address
      if (c<0) c=0;
      dput(c);
      cput(0xc0);               // write character, increment memory ptr.
  } // end for

} // end lcd_string

//--------------------------------------------------------------------------------




void lcd_print_ram(int x,int y,char *string)  // send string of characters to LCD
{
int i;
int c;
lcd_xy(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     // convert ASCII to LCD char address
      if (c<0) c=0;
      dput(c);
      cput(0xc0);               // write character, increment memory ptr.
  } // end for

} // end lcd_string

//--------------------------------------------------------------------------------


void lcd_setpixel(int column, int row)  // set single pixel in 240x64 array
{

int addr;       // memory address of byte containing pixel to write
  if( (column>=XMAX) || (row>=YMAX) )return;
  addr =  G_BASE + (row*BYTES_PER_ROW)  + (column/8);
  lcd_set_address(addr);  // set LCD addr. pointer
  cput(0xf8 | ((7-column%8)) );  // set bit-within-byte command
}

//--------------------------------------------------------------------------------

void lcd_clrpixel(int column, int row)
{
int addr;       // memory address of byte containing pixel to write
  if( (column>=XMAX) || (row>=YMAX) )return;
  addr =  G_BASE + (row*BYTES_PER_ROW)  + (column/8);
  lcd_set_address(addr);  // set LCD addr. pointer
  cput(0xf0 | ((7-column%8)) );  // set bit-within-byte command


}

//---------------------------------------------------------------------------------

void lcd_pixel(int column, int row,char show)
{
int addr;       // memory address of byte containing pixel to write
if( (column>=XMAX) || (row>=YMAX) )return;
addr =  G_BASE + (row*BYTES_PER_ROW)  + (column/8);
lcd_set_address(addr);   // set LCD addr. pointer
if(show)
  cput(0xf8 | ((7-column%8)) );  // set bit-within-byte command
else
  cput(0xf0 | ((7-column%8)) );  // set bit-within-byte command
}

//---------------------------------------------------------------------------------


void show(flash char * s,int start_line,int how_many_line){
int addr,i;
addr =  G_BASE +start_line*30;
lcd_set_address(addr);   // set LCD addr. pointer
cput(0xB0);
for(i=0;i<how_many_line*30;i++){
	dput(s[i]); 
	}
cput(0xB2);
}



void lcd_xy(int x, int y)  // set memory pointer to (x,y) position (text)
{
int addr;

  addr = T_BASE + (y * BYTES_PER_ROW) + x;
  lcd_set_address(addr);   // set LCD addr. pointer

} // lcd_xy()



/* ==============================================================
 * Low-level I/O routines to interface to LCD display
 * based on four routines:
 *
 *          dput(): write data byte
 *          cput(): write control byte
 *          dget(): read data byte         (UNTESTED)
 *          sget(): read status
 * ==============================================================
 */

void lcd_setup()  // make sure control lines are at correct levels
{
 CE_DDR=1;
 RD_DDR=1;
 WR_DDR=1;
 CD_DDR=1;
 RESET_DDR=1;
 CEHI;  // disable chip
 RDHI;  // disable reading from LCD
 WRHI;  // disable writing to LCD
 CDHI;  // command/status mode
 DATAOUT; // make 8-bit parallel port an output port
 RESETHI;
 delay_ms(5);
 RESETLO;
 delay_ms(5);
 RESETHI;
} // end lcd_setup()

//---------------------------------------------------------------------------

void lcd_init()  // initialize LCD memory and display modes
{
 dput(G_BASE%256);
 dput(G_BASE>>8);
 cput(0x42);       // set graphics memory to address G_BASE

 dput(BYTES_PER_ROW%256);
 dput(BYTES_PER_ROW>>8);
 cput(0x43);  // n bytes per graphics line

 dput(T_BASE%256);
 dput(T_BASE>>8);
 cput(0x40);       // text memory at address T_BASE

 dput(BYTES_PER_ROW%256);
 dput(BYTES_PER_ROW>>8);
 cput(0x41);  // n bytes per text line


 cput(0x81);  // mode set: Graphics OR Text, ROM CGen

 cput(0xa7);  // cursor is 8 lines high
 dput(0x00);
 dput(0x00);
 cput(0x21);  // put cursor at (x,y) location

 cput(0x9C);  
	      // (For cursor to be visible, need to set up position

} // end lcd_init()


// -------------------------------------------------------------------------------

int sget(void)  // get LCD display status byte
{
int lcd_status;

  DATAIN;       // make 8-bit parallel port an input
  CDHI;         // bring LCD C/D line high (read status byte)
  RDLO;         // bring LCD /RD line low (read active)
  CELO;         // bring LCD /CE line low (chip-enable active)
  #asm("nop");
  #asm("nop");
  lcd_status = DATAPIN;      // read LCD status byte
  CEHI;         // bring LCD /CE line high, disabling it
  RDHI;         // deactivate LCD read mode
  DATAOUT; // make 8-bit parallel port an output port

  return(lcd_status);
} // sget()

//------------------------------------------------------------------------------------------

void dput(int byte) // write data byte to LCD module over par. port
		    // assume PC port in data OUTPUT mode
{
  do {} while ((0x03 & sget()) != 0x03); // wait until display ready
  CDLO;
  WRLO;         // activate LCD's write mode
  DATAPORT=byte;          // write value to data port
  CELO;                       // pulse enable LOW > 80 ns (hah!)
  #asm("nop");
  #asm("nop");
  CEHI;                       // return enable HIGH
  WRHI;                       // restore Write mode to inactive

 // using my P5/75 MHz PC with ISA bus, CE stays low for 2 microseconds

} // end dput()

//------------------------------------------------------------------------------------------

int dget(void)      // get data byte from LCD module
{
int lcd_byte;

  do {} while ((0x03 & sget()) != 0x03); // wait until display ready
  DATAIN; // make PC's port an input port
  WRHI;   // make sure WRITE mode is inactive
  CDLO;   // data mode
  RDLO;   // activate READ mode
  CELO;   // enable chip, which outputs data
  lcd_byte = DATAPIN;  // read data from LCD
  CEHI;   // disable chip
  RDHI;   // turn off READ mode
  DATAOUT; // make 8-bit parallel port an output port

  return(lcd_byte);
} // dget()

//----------------------------------------------------------------------------------------

void cput(int byte) // write command byte to LCD module
		    // assumes port is in data OUTPUT mode
{
  do {} while ((0x03 & sget()) != 0x03); // wait until display ready

  DATAPORT=byte;  // present data to LCD on PC's port pins

  CDHI;         // control/status mode
  RDHI;         // make sure LCD read mode is off
  WRLO;         // activate LCD write mode
  CELO;         // pulse ChipEnable LOW, > 80 ns, enables LCD I/O
  CEHI;         // disable LCD I/O
  WRHI;         // deactivate write mode

} // cput()










/***********************************************************************
Draws a line from x1,y1 go x2,y2. Line can be drawn in any direction.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_line(int x1, int y1, int x2, int y2, unsigned char show) 
{
  int dy ;
  int dx ;
  int stepx, stepy, fraction;
  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0) 
  {
    dy = -dy;
    stepy = -1;
  }
  else 
  {
    stepy = 1;
  }
  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
  {
    stepx = 1;
  }
  dy <<= 1;
  dx <<= 1;
  lcd_pixel(x1,y1,show);
  if (dx > dy)
  {
    fraction = dy - (dx >> 1); 
    while (x1 != x2)
    {
      if (fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;  
      lcd_pixel(x1,y1,show);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      lcd_pixel(x1,y1,show);
    }
  }
}







/***********************************************************************
Draws a circle with center at x,y with given radius.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_circle(int x, int y, int radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)  
  {
    lcd_pixel(x + xc, y + yc, show);
    lcd_pixel(x + xc, y - yc, show);
    lcd_pixel(x - xc, y + yc, show);
    lcd_pixel(x - xc, y - yc, show);
    lcd_pixel(x + yc, y + xc, show);
    lcd_pixel(x + yc, y - xc, show);
    lcd_pixel(x - yc, y + xc, show);
    lcd_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}

 
void lcd_circle_half(int x, int y, int radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)  
  {
//    lcd_pixel(x + xc, y + yc, show);
    lcd_pixel(x + xc, y - yc, show);
//    lcd_pixel(x - xc, y + yc, show);
    lcd_pixel(x - xc, y - yc, show);
//    lcd_pixel(x + yc, y + xc, show);
    lcd_pixel(x + yc, y - xc, show);
//    lcd_pixel(x - yc, y + xc, show);
    lcd_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}








void lcd_box(int x1, int y1, int x2, int y2, unsigned char show)
{
lcd_line(x1,y1,x2,y1,show);  // up
lcd_line(x1,y2,x2,y2,show);  // down
lcd_line(x2,y1,x2,y2,show);  // right
lcd_line(x1,y1,x1,y2,show);  // left
}




/***********************************************************************
Draws a line from x,y at given degree from inner_radius to outer_radius.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  lcd_line(fx,fy,tx,ty,show);
}


void lcd_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  lcd_line(fx,fy,tx,ty,show);
  lcd_line(fx+1,fy+1,tx+1,ty+1,show);
  lcd_line(fx-1,fy-1,tx-1,ty-1,show);
}







 
void lcd_fill(int x1, int y1, int x2, int y2, unsigned char persent,char first)
{
char M,horizon_line,horizon_line2,i,str1[10];
if(persent>100)return;
if(!first){
	lcd_line(x1,y2,x2,y2,1);  // down
	lcd_line(x2,y1,x2,y2,1);  // right
	lcd_line(x1,y1,x1,y2,1);  // left
	first=1;
	}
M=100/abs(y2-y1);

horizon_line=persent/M;
for(i=0;i<horizon_line;i++)
	lcd_line(x1+2,y2-2-i,x2-2,y2-2-i,1);

horizon_line2=100/M;
for(i=horizon_line;i<horizon_line2;i++)
	lcd_line(x1+2,y2-2-i,x2-2,y2-2-i,0);


sprintf(str1,"%02d%% ",persent);
lcd_print_ram( (x2+x1)/16-1 , (y2+y1)/16,str1);
}





