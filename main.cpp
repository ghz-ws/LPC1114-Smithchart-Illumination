#include "mbed.h"

SPI spi(P0_9, P0_8, P0_6);    //mosi, miso, sclk
DigitalOut c1(P0_7);
DigitalOut c2(P1_4);
DigitalOut c3(P1_5);
DigitalOut c4(P1_8);
DigitalOut c5(P1_2);
DigitalOut str(P0_11);
DigitalOut pwm(P1_9);

DigitalIn sw1(P0_4);
DigitalIn sw2(P0_3);

void regw(uint64_t buf1, uint64_t buf2);
uint64_t buf1, buf2;
uint8_t cnt;
uint64_t r1=0xffffffffffeffffe, r2=0xfffffffffff1ffff, r3=0xfffffffffffffffe, r4=0xfffffffffffffffe;

#define wait_time 400

int main(){
    spi.format(8,0);    //rising edge strage
    spi.frequency(100000); //SPI clk 100kHz
    sw2.mode(PullUp);

    str=0;
    pwm=1;
    c1=0;
    c2=0;
    c3=0;
    c4=0;
    c5=0;

    thread_sleep_for(100);
    regw(0xffffffffffffffff, 0xffffffffffffffff);
    thread_sleep_for(1000);

    while (true){
        if((sw1==0)&&(sw2==0)){
            regw(0xffffffffffffffff, 0xffffffffffffffff);
            c1=1;
            thread_sleep_for(wait_time);
            c1=0;
            c2=1;
            thread_sleep_for(wait_time);
            c2=0;
            c3=1;
            thread_sleep_for(wait_time);
            c3=0;
            c4=1;
            thread_sleep_for(wait_time);
            c4=0;
            c5=1;
            thread_sleep_for(wait_time);
            c5=0;
            thread_sleep_for(wait_time);
        }else if((sw1==1)&&(sw2==0)){
            regw(0xffffffffffffffff, 0xffffffffffffffff);
            c1=0;
            c2=0;
            c3=1;
            c4=1;
            c5=1;
            thread_sleep_for(wait_time);
            c1=1;
            c2=1;
            c3=0;
            c4=0;
            c5=0;
            thread_sleep_for(wait_time);
            c1=0;
            c2=0;
            c3=0;
            c4=0;
            c5=0;
            thread_sleep_for(wait_time);
        }else if((sw1==0)&&(sw2==1)){
            c1=0;
            c2=0;
            c3=0;
            c4=0;
            c5=0;

            r1=((r1<<1)+1)&0xFFFFFFFFFF;
            r2=((r2>>1)+(1<<17))&0x3ffff;
            r3=((r3<<1)+1)&0x1ff;
            r4=((r4<<1)+1)&0x3ff;

            if(r1==0xFFFFeFFFFF)r1=0xffffffffffeffffe&0xFFFFFFFFFF;
            if(r2==0x3ffff)r2=0x1ffff&0x3ffff;
            if(r3==0x1ff)r3=0x1fe&0x1ff;
            if(r4==0x3ff)r4=0x3fe&0x3ff;

            buf1=r1+(r2<<40)+(r3<<58);
            buf2=(r3>>6)+(r4<<3)+(r4<<13);
            regw(buf2,buf1);

            thread_sleep_for(50);
        }else if((sw1==1)&&(sw2==1)){
            if(cnt==87){
                cnt=0;
                buf1=0;
                buf2=0;
                thread_sleep_for(1000);
            }
            c1=0;
            c2=0;
            c3=0;
            c4=0;
            c5=0;
            if(cnt<64){
                buf1=(buf1<<1)+1;
                regw(buf2,buf1);
            }else if(cnt>=64){
                buf2=(buf2<<1)+1;
                regw(buf2,buf1);
            }
            ++cnt;
            thread_sleep_for(30);
        }
    }
}

void regw(uint64_t buf2, uint64_t buf1){
    spi.write(buf2>>16);
    spi.write(buf2>>8);
    spi.write(buf2);
    spi.write(buf1>>56);
    spi.write(buf1>>48);
    spi.write(buf1>>40);
    spi.write(buf1>>32);
    spi.write(buf1>>24);
    spi.write(buf1>>16);
    spi.write(buf1>>8);
    spi.write(buf1);
    str=1;
    str=0;
}