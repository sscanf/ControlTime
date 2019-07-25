//               LCD ( T6963C  240 X 128 ) controller with avr  ( ATmega32 )  
//               Hossein Vismeh  IRAN-ARAK 2005   Just For You


#include "t6963c.h"
#include "include\font.h"
#include "include\fullpic.h"
#include "include\iran.h"


  

    
#include "font\font1.c"
#include "font\lotus.c"
#include "font\Fully.c"
#include "font\Letter.c"





void main(){
unsigned int i;
lcd_setup();              // make sure control lines are at correct levels
lcd_init();               // initialize LCD memory and display modes
lcd_clear_text();
lcd_clear_graph();        // fill graphics memory with 0x00






         
show(iran,0,64);

lcd_print(0, 9,"Graphic LCD ( T6963 ) Project");
lcd_print(0,10,"     AVR Microcontroller");
lcd_print(0,11,"           ATMEGA16");
lcd_print(0,12,"        Just for Your");
lcd_print(0,13,"         ***********");
lcd_print(0,14,"            *****");
lcd_print(0,15,"         2005/sep/15");

delay_ms(4000);
lcd_clear_text();
lcd_clear_graph();
           
lcd_circle(120,64,2,1);
lcd_circle(120,64,4,1);
lcd_circle(120,64,20,1);
lcd_circle(165,81,40,1);
lcd_circle_half(85,61,21,1);
lcd_line(10,10,20,30,1);
lcd_line(30,45,30,100,1);
lcd_box(102,102,120,120,1);

delay_ms(5000);
lcd_clear_graph();
           
for(i=-180;i<180;i++)
	lcd_degree_line(120,64,i,5,60,1);	
for(i=-180;i<180;i++)
	lcd_degree_line(120,64,i,5,60,0);

for(i=-180;i<180;i++)
	lcd_degree_line_bold(120,64,i,5,60,1);	
for(i=-180;i<180;i++)
	lcd_degree_line_bold(120,64,i,5,60,0);

show(shotor,0,128);

delay_ms(5000);
lcd_clear_graph();


lcd_fill(30,30,70,50,0,0);
lcd_fill(80,30,120,50,0,0);
for(i=0;i<10;i++){
lcd_fill(30,30,70,50,rand(),1);
lcd_fill(80,30,120,50,rand(),1);
delay_ms(1300);
}
delay_ms(2000);
lcd_clear_graph();
lcd_clear_text();

lcd_print2(0,0,"1234567890ABCDEFGHIJKLMNOPQRSTUVWX",&f1);
lcd_print2(0,14,"JUST GO FOR UNDERSTAND",&Lotus);
lcd_print2(0,44,"JUST GO FOR ...",&Fully);
lcd_print2(0,80,"JUST FOR YOU ",&Letter);
} // end main




