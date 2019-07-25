

flash  struct FONT_DEF {
   unsigned int glyph_width;                 /* glyph width in pixels              */
                                             /*     0 for variable width fonts     */
   unsigned int glyph_height;                /* glyph height for storage           */
   flash unsigned char *glyph_table;         /* font table start address in memory */
   flash unsigned char *mapping_table;       /* used to find the index of a glyph  */
   flash unsigned char *width_table;         /* width table start adress           */
                                             /*        NULL for fixed width fonts  */
   flash unsigned int *offset_table;         /* ks the offsets of the first byte*/
                                             /*                      of each glyph */
} ;





unsigned char pow2(unsigned char b){
switch(b){
	case 0: return 1;
	case 1: return 2;
	case 2: return 4;
	case 3: return 8;
	case 4: return 16;
	case 5: return 32;
	case 6: return 64;
	case 7: return 128;
	}
}            
            
            
void lcd_print2(unsigned int x,unsigned int y,flash char*in,flash struct  FONT_DEF * strcut1){
register unsigned int offset,width;
register unsigned char i,j,map,ertefah,k,allwidth=0;
//char sss[40];
for(k=0 ; in[k]!=0 ; k++){
	map    =strcut1->mapping_table[in[k]];
	if(strcut1->glyph_width ==0)	width  =strcut1->width_table[map];
	else                            width  =strcut1->glyph_width;
	offset =strcut1->offset_table[map];
	ertefah=strcut1->glyph_height;
	
	
	for(j=0 ; j<ertefah * (((width-1)/8)+1) ; j+=(((width-1)/8)+1)    ){   // ertefah
		for(i=0 ; i<width  ; i++){   //  width
 			if( strcut1-> glyph_table[ offset+j+(i/8) ] & pow2( 7 - ( i % 8 ) ))
				lcd_setpixel(  x+i+allwidth , y+j/ (((width-1)/8)+1)  );
 			else
 				lcd_clrpixel(  x+i+allwidth , y+j/ (((width-1)/8)+1)  );
			}//End i
		}// End j
	allwidth+=width;
	}// End K
}                                     

