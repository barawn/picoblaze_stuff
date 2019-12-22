#line 1 "/home/barawn/pb_helix_setup/pblaze_helix_setup.c"
#line 1 "kcpsm6.h"

typedef unsigned char uint8_t;
typedef uint8_t bool_t;





extern void input(uint8_t addr, uint8_t* reg);
extern void output(uint8_t addr, uint8_t* reg);
extern void outputk(uint8_t addr, uint8_t* reg);

extern void fetch(uint8_t addr, uint8_t* reg);
extern void store(uint8_t addr, uint8_t* reg);


extern void enable_interrupt();
extern void disable_interrupt();



extern void psm(char* fmt, ...);


extern uint8_t s0;
extern uint8_t s1;
extern uint8_t s2;
extern uint8_t s3;
extern uint8_t s4;
extern uint8_t s5;
extern uint8_t s6;
extern uint8_t s7;
extern uint8_t s8;
extern uint8_t s9;
extern uint8_t sA;
extern uint8_t sB;
extern uint8_t sC;
extern uint8_t sD;
extern uint8_t sE;
#line 2 "/home/barawn/pb_helix_setup/pblaze_helix_setup.c"
#line 12 "/home/barawn/pb_helix_setup/pblaze_helix_setup.c"
#line 1 "pb_axi.h"
#line 13 "/home/barawn/pb_helix_setup/pblaze_helix_setup.c"
#line 1 "pb_axi_iic.h"
#line 4 "pb_axi_iic.h"
#line 1 "pb_func_helpers.h"
#line 4 "pb_func_helpers.h"
#line 1 "cpp_helpers.h"
#line 5 "pb_func_helpers.h"
#line 5 "pb_axi_iic.h"
#line 1 "pb_axi.h"
#line 6 "pb_axi_iic.h"
#line 20 "pb_axi_iic.h"
void i2c_writereg_fn(void) {
#line 21 "pb_axi_iic.h"
#pragma push_macro("sSpare")



    outputk( ( 0x00 + 0x8 ),0xA) ;
    outputk( ( 0x00 + 0xC ),( 0x1000 + 0x0040 ) & 0xFF) ;
    outputk( ( 0x00 + 0xC +1),( ( 0x1000 + 0x0040 ) >> 8) & 0xFF) ;
    outputk( ( 0x00 + 0x0 ),0x03) ;
    do {
        input( ( 0x00 + 0x0 ), sD ) ;
    }
    while ( sD |^ 0x8) ;
    outputk( ( 0x00 + 0x8 ),0x1) ;
    outputk( ( 0x00 + 0xC ),( 0x1000 + 0x0100 ) & 0xFF) ;
    outputk( ( 0x00 + 0xC +1),( ( 0x1000 + 0x0100 ) >> 8) & 0xFF) ;
    outputk( ( 0x00 + 0x0 ),0x03) ;
    do {
        input( ( 0x00 + 0x0 ), sD ) ;
    }
    while ( sD |^ 0x8) ;

    outputk( ( 0x00 + 0x8 +1),0x1) ;
    output( ( 0x00 + 0x8 +0),sA) ;
    outputk( ( 0x00 + 0xC ),( 0x1000 + 0x0108 ) & 0xFF) ;
    outputk( ( 0x00 + 0xC +1),( ( 0x1000 + 0x0108 ) >> 8) & 0xFF) ;
    outputk( ( 0x00 + 0x0 ),0x03) ;
    do {
        input( ( 0x00 + 0x0 ), sD ) ;
    }
    while ( sD |^ 0x8) ;

    outputk( ( 0x00 + 0x8 +1),0x0) ;
    output( ( 0x00 + 0x8 +0),sB) ;
    outputk( ( 0x00 + 0x0 ),0x03) ;
    do {
        input( ( 0x00 + 0x0 ), sD ) ;
    }
    while ( sD |^ 0x8) ;

    outputk( ( 0x00 + 0x8 +1),0x2) ;
    output( ( 0x00 + 0x8 +0),sC) ;
    outputk( ( 0x00 + 0x0 ),0x03) ;
    do {
        input( ( 0x00 + 0x0 ), sD ) ;
    }
    while ( sD |^ 0x8) ;

    do {
        outputk( ( 0x00 + 0xC ),( 0x1000 + 0x0104 ) & 0xFF) ;
        outputk( ( 0x00 + 0xC +1),( ( 0x1000 + 0x0104 ) >> 8) & 0xFF) ;
        outputk( ( 0x00 + 0x0 ),0x01) ;
        do {
            input( ( 0x00 + 0x0 ), sD ) ;
        }
        while ( sD |^ 0x8) ;
        input( ( 0x00 + 0x8 +0),sA) ;
    } while (sA |^ 0x80);
#line 43 "pb_axi_iic.h"
#pragma pop_macro("sSpare")
}
#line 14 "/home/barawn/pb_helix_setup/pblaze_helix_setup.c"

void init(void) {

    outputk( ( 0x00 + 0xC ),0x0000 & 0xFF) ;
    outputk( ( 0x00 + 0xC +1),(0x0000 >> 8) & 0xFF) ;
    outputk( ( 0x00 + 0x0 ),0x01) ;
    do {
        input( ( 0x00 + 0x0 ), sD ) ;
    }
    while ( sD |^ 0x8) ;

    input( ( 0x00 + 0x8 +0),&s0) ;

    s1 = 0x4;
    s2 = 0;

    sA = 0xD0;
    sB = 0x0;
    sC = 0x0;
    i2c_writereg_fn() ;





    while (s0 != 0) {

        output( ( 0x00 + 0xC +0),s1) ;
        outputk( ( 0x00 + 0xC +1),0x00) ;
        outputk( ( 0x00 + 0x0 ),0x01) ;
        do {
            input( ( 0x00 + 0x0 ), sD ) ;
        }
        while ( sD |^ 0x8) ;


        input( ( 0x00 + 0x8 +2),sC) ;
        if (sC != s2) {
            sA = 0xD0;
            sB = 0x0;
            i2c_writereg_fn() ;
        }

        input( ( 0x00 + 0x8 +1),sB) ;

        input( ( 0x00 + 0x8 +0),sC) ;
        sA = 0xD0;
        i2c_writereg_fn() ;

        s0 -= 1;

        s1 += 4;
    }


}

void loop(void) {

}
