


bool isTable=true;
#include <FS.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
//#include "mariomap.h"
#include "painting2.h"
 //#include "movie3.h"

#include <WebSocketsServer.h>



//ESP8266WiFiMulti WiFiMulti;

//WebSocketsServer webSocket = WebSocketsServer(81);

//#define FASTLED_FORCE_SOFTWARE_SPI
#define USE_SPI 1
#define FASTLED_ALLOW_INTERRUPTS 0
#define INTERRUPT_THRESHOLD 1
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#define FASTLED_SHOW_CORE 0
#include "fontamstrad.h"
int cos_table[123];
#define USE_SERIAL Serial
char mess[255];
static int anim=0;
WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
//#include <Artnet.h>
const char* ssid     = "yourssid";
const char* password = "yourpasswd";

WiFiUDP Udp2;

//#define PINS_MASK calculMask()
//#define calculMask() ({ int t[(NUM_STRIPS)]={PINS_OUTPUT};int result = 0; for(int i=0;i<(NUM_STRIPS);i++){result=result+ (1<<t[i]);} result; })

#define PORT_MASK 0b110111011111111000000110100  //0b1001111111000000111101
//#define PORT_MASK   0b000000000000000000
#define NUM_STRIPS 16
#define PINS_OUTPUT  0,1,2,4,5,14,23,25,26,21,22,16


#define LED_WIDTH 123
#define LED_HEIGHT_PER_STRIP 3
#define LED_HEIGHT NUM_STRIPS*LED_HEIGHT_PER_STRIP
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP
#define NUM_LEDS_PER_STRIP LED_HEIGHT_PER_STRIP*LED_WIDTH
//Define table orientation this is where the pixel 0 is
#define DOWN_RIGHT   0 //natural mode no transpostion to be made
#define DOWN_LEFT    1 //We turn 90° clock wise
#define UP_LEFT      2
#define UP_RIGHT     3
#define DOWN_RIGHT_INV   4 //natural mode no transpostion to be made
#define DOWN_LEFT_INV    5 //We turn 90° clock wise
#define UP_LEFT_INV      6
#define UP_RIGHT_INV     7
  char OUTPUT_PINS[]={0,1,2,7,8,9,26,14,22,18,19,23};

CRGB solidColor = CRGB(0, 0, 0);
CRGB bgColor = CRGB(10,10,10);
CRGB Color = CRGB :: Blue;
CRGB Color2 = CRGB :: Green;
CRGB fraiseCalc[110];
CRGB transparent = CRGB(1, 1, 1);
CRGB lettrefont1[35];
CRGB lettrefont2[64];
CRGB leds[NUM_LEDS+1];
CRGB Tpic[NUM_LEDS+1];
//unsigned char Tpic2[NUM_LEDS*3+1];
int tableOrientation=DOWN_RIGHT; //DOWN_RIGHT_INV;
long int k=0;
int tableBrightness;
int r=0;
int dire=0;
int pas=45;
float maxi=0;
float mini=9999999;
int pacx=3;
int pacy=2;
int face=0;
int mapy=0;
char *artnetPacket2;
const int numLeds = 240; // change for your setup
const int numberOfChannels = numLeds * 3; // Total number of channels you want to receive (1 led = 3 channels)
const byte dataPin = 2;


// Artnet settings
//Artnet artnet;
const int startUniverse = 0; // CHANGE FOR YOUR ma most software this is 1, some software send out artnet first universe as 0.
int previousDataLength = 0;

byte broadcast[] = {10, 0, 1, 255};




void replaceled()
{
  int offset=0;
  for(int i=0;i<123;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s;
   //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
 }
 offset=24*LED_WIDTH;
  for(int i=0;i<24*LED_WIDTH;i++)
 {
   byte s=leds[i+offset].g;
  // char buff[9];
   // my_itoa (s,buff,16,8);
    //Serial.println(buff);
   leds[i+offset].g= leds[i+offset].r;
   leds[i+offset].r= s; 
   //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
 }
 //on met les boards en noir

 for (int i=0;i<LED_HEIGHT;i++)
 {
  leds[i*LED_WIDTH]=CRGB::Black;
  leds[(i+1)*LED_WIDTH-1]=CRGB::Black;
 }
}
uint8_t  gamma8[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,22,23,23,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,85,87,88,89,91,92,93,95,96,98,99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,122,123,125,127,128,130,132,133,135,137,138,140,142,144,145,147,149,151,153,155,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,197,199,201,203,205,207,210,212,214,216,219,221,223,226,228,230,233,235,237,240,242,245,247,250,252,255,
    
};
uint8_t  gammar[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,13,13,14,14,14,15,15,16,16,17,17,18,18,19,19,20,21,21,22,22,23,23,24,25,25,26,27,27,28,29,29,30,31,31,32,33,34,34,35,36,37,37,38,39,40,41,42,42,43,44,45,46,47,48,49,50,51,52,52,53,54,55,56,57,59,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,77,78,79,80,82,83,84,85,87,88,89,91,92,93,95,96,98,99,100,102,103,105,106,108,109,111,112,114,115,117,119,120,122,123,125,127,128,130,132,133,135,137,138,140,142,144,145,147,149,151,153,155,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,197,199,201,203,205,207,210,212,214,216,219,221,223,226,228,230,233,235,237,240,242,245,247,250,252,255,
    
};
void gammareplace()
{
  int offset=0;
  for(int j=0;j<48;j++)
  {
    
  
      for(int i=0;i<123;i++)
     {
      int offset=j*123;
      // byte s=leds[i+offset].g;
      // char buff[9];
       // my_itoa (s,buff,16,8);
        //Serial.println(buff);
       Tpic[i+offset].g= gamma8[Tpic[i+offset].g];
       Tpic[i+offset].r= gammar[Tpic[i+offset].r];;
       Tpic[i+offset].b=gamma8[Tpic[i+offset].b];
       //CRGB((s&0x0F000)>>8,(s&0x00FF0000)>>16 ,s & 0xFF) ;  //(leds[i+offset] & 0xFF) |  ( (leds[i+offset] & 0x00FF00L)<<8   ) |  (  (leds[i+offset] & 0xFF0000L)>>8  );
     }
  }

}

CRGB artnetled[32*32];
word readyd=0;
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data, IPAddress remoteIP)
{
  int universe_size=128;
  //Serial.printf("un:%d ",universe);
  // read universe and put into the right part of the display buffer
/*  for (int i = 0; i < 128; i++)
  {
    int offset = 3*i;//+universe_size*(universe-1)*3;
    if (offset< NUM_LEDS)
      artnetled[i+universe_size*(universe-1)]=CRGB( data[offset], data[offset + 1], data[offset + 2]);
  }*/
  //  memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
memcpy(&artnetled[universe_size*(universe-1)],data,128*3);
 if(universe<5)
  readyd=readyd | (1<<(universe-1));
  //Serial.printf("ser:%d ",readyd);
  previousDataLength = length;
  if(readyd==15)
  {
       displayPicNew(artnetled,0,0,16,32);
       replaceled();
  FastLEDshowESP32();
  readyd=0;
 // Serial.println();
  
  }
}

void onSync(IPAddress remoteIP) {
     replaceled();
  FastLEDshowESP32();
}


#include "lamp.h"

float gammaCorrection=2.3;


void calculatecos()
{
  Serial.println("'cosq");
  for(int i=0;i<123;i++)
 {
  //jj
   cos_table[i]=(int)abs((48-1)*(abs(cos(3.14*i/60)) ));
   Serial.println(cos_table[i]);
 } 
 
}


void calculateGammaTable()
{
    Serial.print("bri");
    Serial.println(tableBrightness);
    Serial.println((float)tableBrightness/255);
    float correctGamma=powf((float)tableBrightness/255,1/gammaCorrection);
    Serial.print("gamma correction:");
    Serial.println(correctGamma);
    correctGamma=1;
    for (int i=0;i<256;i++)
    {
 
        float newValue=255*powf((float)i/255,correctGamma*gammaCorrection);
        //Serial.println(newValue);
        gamma8[i]=(int)newValue;
        //Serial.println(gamma8[i]);
        newValue=255*powf((float)i/255,correctGamma*(gammaCorrection+0.2));
        gammar[i]=(int)newValue;
        //Serial.println(gamma8[i]);
    }
}

void setTableBrightness(int b)
{
   
   FastLED.setBrightness(b);
    tableBrightness=b;
    calculateGammaTable();
}

void displayBitmap(unsigned char *pic,int x0,int y0,int h,int w)
{
  //frame size
    int max_wide=0;
    int wide=w+x0;
    int offsety;
    int offsetx;
    int newx=0;
    int newy=0;
    if(wide<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_wide=LED_WIDTH;
    else
        max_wide=LED_HEIGHT;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<max_wide)
            wide=w;
        else
            wide=max_wide-x0;
    }
    else
    {
        newx=0;
        offsetx=-x0;
        if(w+x0<max_wide)
            wide=w+x0;
        else
            wide=max_wide;
    }
    
    
    
    
    int max_height=0;
    int height=h+y0;
    if(height<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_height=LED_HEIGHT;
    else
        max_height=LED_WIDTH;
    
    if (y0>=max_height) //on est en dehors du cadre
        return;
    if(y0>=0)
    {
        newy=y0;
        offsety=0;
        if(h+y0<max_height)
            height=h;
        else
            height=max_height-y0;
    }
    else
    {
        newy=0;
        offsety=-y0;
        if(h+y0<max_height)
            height=h+y0;
        else
            height=max_height;
    }
    
    
    
    int r;
    int g;
    int b;
    // Serial.println("r1");


   


    CRGB *bitmapRGB =(CRGB*)malloc(wide*height*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        Serial.println(wide);
        Serial.println(height);
        Serial.println(x0);
        Serial.println(y0);
        return;
    }
    //Serial.println("image cree");
    for(int y=0;y<height;y++){
        for(int x=0;x<wide;x++){
          
            r=gammar[(int)pic[3*((y+offsety)*w+x+offsetx)]];
            g=gamma8[ (int)pic[3*((y+offsety)*w+x+offsetx)+1]];
            b= gamma8[(int)pic[3*((y+offsety)*w+x+offsetx)+2]];
           bitmapRGB[x+y*wide]=CRGB(r,g,b); //I have a program that switches directly from
        }
    }
    displaypic(bitmapRGB,newx,newy,height,wide);
    free(bitmapRGB);
    
    
    
}





void displayBitmapFromProgmem(const unsigned char *pic,int x0,int y0,int h,int w )
{

    int max_wide=0;
    int wide=w+x0;
    int offsety;
    int offsetx;
    int newx=0;
    int newy=0;
    if(wide<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_wide=LED_WIDTH;
    else
        max_wide=LED_HEIGHT;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<=max_wide)
            wide=w;
        else
            wide=max_wide-x0;
    }
    else
    {
        newx=0;
        offsetx=-x0;
        if(w+x0<max_wide)
            wide=w+x0;
        else
            wide=max_wide;
    }
    
    
    
    
    int max_height=0;
    int height=h+y0;
    if(height<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_height=LED_HEIGHT;
    else
        max_height=LED_WIDTH;
    
    if (y0>=max_height) //on est en dehors du cadre
        return;
    if(y0>=0)
    {
        newy=y0;
        offsety=0;
        if(h+y0<max_height)
            height=h;
        else
            height=max_height-y0;
    }
    else
    {
        newy=0;
        offsety=-y0;
        if(h+y0<max_height)
            height=h+y0;
        else
            height=max_height;
    }
    
    
    int r;
    int g;
    int b;

    CRGB *bitmapRGB =(CRGB*)malloc(wide*height*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }

    for(int y=0;y<height;y++){
        for(int x=0;x<wide;x++){
            
             r= gammar[(int)pgm_read_byte(pic+ 3*((y+offsety)*w+x+offsetx))];
            g= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+1)];
            b= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+2)];
            bitmapRGB[x+y*wide]=CRGB(r,g,b); //I have a program that switches directly from
        }
    }
    //displayPicNew(bitmapRGB,newx,newy,height,wide);
    
     putOrientation2(bitmapRGB,height,wide,newx,newy);
    
    free(bitmapRGB);
    
}



void putOrientationInv(CRGB *pic,int h,int w,int x0,int y0)
{
    //CRGB * Tpic = (CRGB*)calloc(h*w,sizeof(CRGB));
    if(!Tpic)
    {
        Serial.println("pas possible");
        return ;//NULL;
    }
    switch(tableOrientation){
            
        case DOWN_RIGHT: //on ne fait rien
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( ((x+x0)+LED_WIDTH*(y+y0)) <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+w-1-x];
                }
            }
            
            break;
            
        case DOWN_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0 <NUM_LEDS && LED_HEIGHT-1-x-x0 >=0 && y+y0>=0 && LED_HEIGHT-1-x-x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                        Tpic[LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0]=pic[w*y+w-1-x];
                }
            }
            break;
            
            
        case UP_RIGHT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)) <NUM_LEDS  && (x+x0) >=0 && LED_WIDTH-1-(y+y0)>=0 && (x+x0)<LED_HEIGHT && LED_WIDTH-1-(y+y0) <LED_WIDTH  )
                        Tpic[LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)]=pic[w*y+w-1-x];
                }
            }
            break;
            
        case UP_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && LED_WIDTH-1-(x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && LED_WIDTH-1-(x+x0) <LED_WIDTH )
                        Tpic[LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+w-1-x];
                }
            }
            break;
            
            //on inverse la lecture des pixels au niveau des X on lit de gauche a droite
        case DOWN_RIGHT_INV: //on ne fait rien
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0) <NUM_LEDS && LED_WIDTH-1-x-x0 >=0 && y+y0>=0 && LED_WIDTH-1-x-x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0)]=pic[w*y+w-1-x];
                }
            }
            
            break;
            
        case DOWN_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(x+x0)+y+y0 <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                        Tpic[LED_WIDTH*(x+x0)+y+y0]=pic[w*y+w-1-x];
                }
            }
            break;
            
            
        case UP_RIGHT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+(y+y0)) <NUM_LEDS  && (x+x0) >=0 && (y+y0)>=0 && (x+x0)<LED_HEIGHT && (y+y0) <LED_WIDTH  )
                        Tpic[LED_WIDTH*((x+x0))+(y+y0)]=pic[w*y+w-1-x];
                }
            }
            break;
            
        case UP_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && (x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && (x+x0) <LED_WIDTH )
                        Tpic[(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+w-1-x];
                }
            }
            break;
            
            
            
    }
    //return Tpic;
    
}




void displayPicInv( CRGB *pica, int x0, int y0, int h, int w)
{
    putOrientationInv(pica,h,w,x0,y0);
    if(Tpic)
    {
        int hmin=LED_HEIGHT;
        int  wmin=LED_WIDTH;
        x0=0;
        y0=0;
        for (int y = y0; y < hmin + y0; y++)
        {
            if (y >= 0) {
                if (y % 2 == 0)
                {
                    if (x0 < 0)
                    {
                        int nh = wmin + x0;
                        if (nh > 0)
                            memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
                    }
                    else
                        memcpy(&leds[LED_WIDTH * y + x0], &Tpic[(y - y0)*wmin], wmin * sizeof(CRGB));
                }
                else
                {
                    for (int x = x0; x < wmin + x0; x++)
                    {
                        if (x >= 0)
                            //leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - x] = Tpic[wmin * (y - y0) + x - x0 ];
                             leds[(LED_WIDTH <<1)* ( (y>>1) + 1) - (1 + x)] = Tpic[wmin * (y - y0) + x - x0 ];
                    }
                }
            }
        }
        
    }
    
}


void my_itoa(long int value, char *buf, int base,int size){
    
    
    
    //buf = "";
    
    //for(; value && i ; --i, value /= base)
    for (int i=0;i<size;i++)
    {
        //printf("%d  ",value%base);
        //printf("%c ",(char)"0123456789abcdef"[value % base]);
        //printf("i:%d\n",i);
        buf[size-1-i] = "0123456789abcdef"[value % base];
        value/=base;
    }
    buf[size]=0;
}


void PixelOn(int x,int y,CRGB Color)
{
    //int offset=0;
    if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return ;
    if(y%2==0)
        leds[x+y*LED_WIDTH]=Color;
    else
    {
        //int offset2=2 * LED_WIDTH * ((int)floor(y /2)  + 1) - 1 - x;
        leds[(LED_WIDTH<<1) * ((y >>1)  + 1) - 1 - x]=Color;
        // leds[LED_WIDTH*y+(LED_WIDTH<<1)-1-x]=Color;
        
    }
    
    // leds[ (y%2==0) ? x+y*LED_WIDTH : (LED_WIDTH<<1) * ((y >>1)  + 1) - 1 - x]=Color;
    //leds[offset]=Color;
    
}

 CRGB getPixelValue(int x, int y)
{
  if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return CRGB(0,0,0);
    if(y%2==0)
        return leds[x+y*LED_WIDTH];
    else
    {
        //int offset2=2 * LED_WIDTH * ((int)floor(y /2)  + 1) - 1 - x;
       return leds[(LED_WIDTH<<1) * ((y >>1)  + 1) - 1 - x];
        // leds[LED_WIDTH*y+(LED_WIDTH<<1)-1-x]=Color;
        
    }
}

#include "graphicfunction.h"
byte font1[][7]=
{
    
    //32->47
    {B00000,  B00000, B00000, B00000, B00000, B00000, B00000  },{B01000,  B01000, B01000, B01000, B01000, B00000, B01000  },  {B01010,  B01010, B01010, B00000, B00000, B00000, B00000  },  {B01010,  B01010, B11111, B01010, B11111, B01010, B01010  },  {B00100,  B01111, B10100, B01110, B00101, B11110, B00100  },  {B11000,  B11001, B00010, B00100, B01000, B10011, B00011  },  {B01000,  B10100, B10100, B01000, B10101, B10010, B01101  },  {B01100,  B00100, B01000, B00000, B00000, B00000, B00000  },  {B00010,  B00100, B01000, B01000, B01000, B00100, B00010  },  {B01000,  B00100, B00010, B00010, B00010, B00100, B01000  },  {B10101,  B01110, B00100, B01110, B10101, B00000, B00000  },  {B00100,  B00100, B00100, B11111, B00100, B00100, B00100  },  {B00000,  B00000, B00000, B00000, B11000, B01000, B10000  },  {B00000,  B00000, B00000, B11100, B00000, B00000, B00000  },  {B00000,  B00000, B00000, B00000, B00000, B01100, B01100  },  {B00000,  B00001, B00010, B00100, B01000, B10000, B00000  },
    
    //48->57
    {B01110,  B10001, B10011, B10101, B11001, B10001, B01110  },  {B00100,  B01100, B00100, B00100, B00100, B00100, B01110  },  {B01110,  B10001, B00001, B00010, B00100, B01000, B11111  },  {B11111,  B00010, B00100, B00010, B00001, B10001, B01110  },  {B00010,  B00110, B01010, B10010, B11111, B00010, B00010  },  {B11111,  B10000, B11110, B00001, B00001, B10001, B01110  },  {B00110,  B01000, B10000, B11110, B10001, B10001, B01110  },  {B11111,  B00001, B00010, B00100, B01000, B10000, B10000  },  {B01110,  B10001, B10001, B01110, B10001, B10001, B01110  },  {B01110,  B10001, B10001, B01111, B00001, B00010, B01100  },
    
    //58->64
    {B00000,  B01100, B01100, B00000, B01100, B01100, B00000  },  {B00000,  B01100, B01100, B00000, B01100, B00100, B01000  },  {B00010,  B00100, B01000, B10000, B01000, B00100, B00010  },  {B00000,  B00000, B11111, B00000, B11111, B00000, B00000  },  {B10000,  B01000, B00100, B00010, B00100, B01000, B10000  },  {B01110,  B10001, B00001, B00010, B00100, B00000, B00100  },  {B01110,  B10001, B10011, B10100, B10101, B10001, B01110  },
    
    //65->90
    {B01110,  B10001, B10001, B11111, B10001, B10001, B10001  },  {B11110,  B10001, B10001, B11110, B10001, B10001, B11110  },  {B01110,  B10001, B10000, B10000, B10000, B10001, B01110  },  {B11110,  B10001, B10001, B10001, B10001, B10001, B11110  },  {B11111,  B10000, B10000, B11110, B10000, B10000, B11111  },  {B11111,  B10000, B10000, B11110, B10000, B10000, B10000  },  {B01110,  B10001, B10000, B10111, B10001, B10001, B01110  },  {B10001,  B10001, B10001, B11111, B10001, B10001, B10001  },  {B01110,  B00100, B00100, B00100, B00100, B00100, B01110  },  {B00111,  B00010, B00010, B00010, B00010, B10010, B01100  },  {B10001,  B10010, B10100, B11000, B10100, B10010, B10001  },  {B10000,  B10000, B10000, B10000, B10000, B10000, B11111  },  {B10001,  B11011, B10101, B10101, B10001, B10001, B10001  },  {B10001,  B10001, B11001, B10101, B10011, B10001, B10001  },  {B01110,  B10001, B10001, B10001, B10001, B10001, B01110  },  {B11110,  B10001, B10001, B11110, B10000, B10000, B10000  },  {B01110,  B10001, B10001, B10001, B10101, B10010, B01101  },  {B11110,  B10001, B10001, B11110, B10100, B10010, B10001  },  {B01111,  B10000, B10000, B01110, B00001, B00001, B11110  },  {B11111,  B00100, B00100, B00100, B00100, B00100, B00100  },  {B10001,  B10001, B10001, B10001, B10001, B10001, B01110  },  {B10001,  B10001, B10001, B10001, B10001, B01010, B00100  },  {B10001,  B10001, B10001, B10001, B10101, B10101, B01010  },  {B10001,  B10001, B01010, B00100, B01010, B10001, B10001  },  {B10001,  B10001, B10001, B01010, B00100, B00100, B00100  },  {B11111,  B00001, B00010, B00100, B01000, B10000, B11111  },
    
    //91->96
    {B01110,  B01000, B01000, B01000, B01000, B01000, B01110  },  {B10001,  B01010, B11111, B00100, B11111, B00100, B00100  },  {B01110,  B00010, B00010, B00010, B00010, B00010, B01110  },  {B00100,  B01010, B10001, B00000, B00000, B00000, B00000  },  {B00000,  B00000, B00000, B00000, B00000, B00000, B11111  },  {B01000,  B00100, B00010, B00000, B00000, B00000, B00000  },
    
    //97->122
    {B00000,  B00000, B01110, B00001, B01111, B10001, B01111  },  {B10000,  B10000, B10110, B11001, B10001, B10001, B11110  },  {B00000,  B00000, B01110, B10000, B10000, B10001, B01110  },  {B00001,  B00001, B01101, B10011, B10001, B10001, B01111  },  {B00000,  B00000, B01110, B10001, B11111, B10000, B01110  },  {B00110,  B01001, B01000, B11100, B01000, B01000, B01000  },  {B00000,  B01111, B10001, B10001, B01111, B00001, B01110  },  {B10000,  B10000, B10110, B11001, B10001, B10001, B10001  },  {B00100,  B00000, B01100, B00100, B00100, B00100, B01110  },  {B00010,  B00000, B00110, B00010, B00010, B10010, B01100  },  {B10000,  B10000, B10010, B10100, B11000, B10100, B10010  },  {B01100,  B00100, B00100, B00100, B00100, B00100, B01110  },  {B00000,  B00000, B11010, B10101, B10101, B10001, B10001  },  {B00000,  B00000, B10110, B11001, B10001, B10001, B10001  },  {B00000,  B00000, B01110, B10001, B10001, B10001, B01110  },  {B00000,  B00000, B11110, B10001, B11110, B10000, B10000  },  {B00000,  B00000, B01110, B10001, B01111, B00001, B00001  },  {B00000,  B00000, B10110, B11001, B10000, B10000, B10000  },  {B00000,  B00000, B01110, B10000, B01110, B00001, B11110  },  {B01000,  B01000, B11100, B01000, B01000, B01001, B00110  },  {B00000,  B00000, B10001, B10001, B10001, B10011, B01101  },  {B00000,  B00000, B10001, B10001, B10001, B01010, B00100  },  {B00000,  B00000, B10001, B10001, B10001, B10101, B01010  },  {B00000,  B00000, B10001, B01010, B00100, B01010, B10001  },  {B00000,  B00000, B10001, B10001, B01111, B00001, B01110  },  {B00000,  B00000, B11111, B00010, B00100, B01000, B11111  },
    
    
};

void afficheLettre(int let,int x0,int y0)
{
    
    int taille=5;
    int hauteur=7;
    
    for(int x=x0;x<taille+x0;x++)
    {
        for (int y=y0;y<hauteur+y0;y++)
        {
            if ( ( (1 << (x0-x+taille-1)) &   font1[let][hauteur-1 - y + y0] ) > 0 )
            {
                lettrefont1[(x-x0)+(y-y0)*5]=CRGB::Red;
            }
            else
            {
                lettrefont1[(x-x0)+(y-y0)*5]=CRGB(bgColor);
            }
        }
    }
    //Serial.printf("pic :%d %d\n",x0,y0);
    displayPicNew(lettrefont1,x0,y0,7,5);
    
}


void afficheLettre2(int let,int x0,int y0)
{
    
    int taille=8;
    int hauteur=8;
    
    for(int x=x0;x<taille+x0;x++)
    {
        for (int y=y0;y<hauteur+y0;y++)
        {
            if ( ( (1 << (x0-x+taille-1)) &   fontamstrad[let][hauteur-1 - y + y0] ) > 0 )
            {
                lettrefont2[(x-x0)+(y-y0)*taille]=CRGB(255,255,0);
            }
            else
            {
                lettrefont2[(x-x0)+(y-y0)*taille]=CRGB(bgColor);
            }
        }
    }
    //Serial.printf("pic :%d %d\n",x0,y0);
    displayPicNew(lettrefont2,x0,y0,hauteur,taille);
    
}



 void afficheMessage2(char *mess,int x,int y)
{
    if (!mess)
        return;
    int taille=strlen(mess);
   // Serial.println(mess);
    //Serial.println(taille);
    int d=0;
    int dx=0;
    
    for (long int i=0;i<taille;i++)
    {
//Serial.printf("%d ",mess[i]);
  
       if(mess[i]=='\n')
      {
        d++;
        Serial.printf("\n d:%d \n",d);
        dx=i+1;
        if (y-d*8<=-8)
         return;
      }
      else
      {
       if(x+(i-dx)*8 > -8 and x+(i-dx)*8 <LED_WIDTH)
        afficheLettre2(mess[i]-32, x+(i-dx)*8,y-d*8);
        
        //if (x+i*8>= LED_WIDTH)
          //  return;
    }
    }
  Serial.println("fin message");  
}


void afficheMessage(char *mess,int x,int y)
{
    if (!mess)
        return;
    int taille=strlen(mess);
   // Serial.println(mess);
    //Serial.println(taille);
    
    for (long int i=0;i<taille;i++)
    {
      if(mess[i]==13)
      {
        y=y-6;
        if (y<=-6)
         return;
      }
      else
      {
       if(x+i*6 > -6 and x+i*6 <LED_WIDTH)
        afficheLettre(mess[i]-32, x+i*6,y);
        
        //if (x+i*6>= LED_WIDTH)
          //  return;
      }
    }
    
}



//manage the the core0


// -- Task handles for use in the notifications
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t FastLEDshowTaskHandle2 = 0;
static TaskHandle_t userTaskHandle = 0;


void FastLEDshowESP32()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
       // noInterrupts();
        userTaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(FastLEDshowTaskHandle);

        // -- Wait to be notified that it's done
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS( 500 ));//portMAX_DELAY);
        //delay(100);
        //interrupts();
        userTaskHandle = 0;
    }
}



void FastLEDshowESP322()
{
    if (userTaskHandle == 0) {
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 200 );
        // -- Store the handle of the current task, so that the show task can
        //    notify it when it's done
       // noInterrupts();
        userTaskHandle = xTaskGetCurrentTaskHandle();
        
        // -- Trigger the show task
        xTaskNotifyGive(FastLEDshowTaskHandle2);
        //to thge contrary to the other one we do not wait for the display task to come back
    }
}

void FastLEDshowTask(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        

    
        FastLED.show();

    
        xTaskNotifyGive(userTaskHandle);
    }
}

static long time3=ESP.getCycleCount();
void FastLEDshowTask2(void *pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 500 );
    // -- Run forever...
    for(;;) {
        // -- Wait for the trigger
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        
            
            if (anim==0)
                      displayPicNew(artnet.getframe(),0,0,48,123); //for the artnet
            else
                memcpy(leds,Tpic,123*48*sizeof(CRGB));
            
            replaceled();
            FastLED.show();
            
               userTaskHandle=0; //so we can't have two display tasks at the same time
                 
           }
}


// Note: this can be 12 if you're using a teensy 3 and don't mind soldering the pads on the back

int pacmab[256]={
 9,9,9,9,9,9,9,9,
 9,1,1,1,9,9,9,9,
 1,1,1,1,1,9,9,9,
 1,1,1,1,1,9,9,9,
 1,1,1,1,1,9,9,9,
 9,1,1,1,9,9,9,9,
 9,9,9,9,9,9,9,9,
 9,9,9,9,9,9,9,9,

 9,9,9,9,9,9,9,9,
 9,1,1,1,9,9,9,9,
 1,1,1,1,1,9,9,9,
 9,9,1,1,1,9,9,9,
 1,1,1,1,1,9,9,9,
 9,1,1,1,9,9,9,9,
 9,9,9,9,9,9,9,9,
 9,9,9,9,9,9,9,9,

 9,9,9,9,9,9,9,9,
 9,1,1,1,9,9,9,9,
 9,9,1,1,1,9,9,9,
 9,9,9,1,1,9,9,9,
 9,9,1,1,1,9,9,9,
 9,1,1,1,9,9,9,9,
 9,9,9,9,9,9,9,9,
 9,9,9,9,9,9,9,9,

 9,9,9,9,9,9,9,9,
 9,1,1,1,9,9,9,9,
 1,1,1,1,1,9,9,9,
 9,9,1,1,1,9,9,9,
 1,1,1,1,1,9,9,9,
 9,1,1,1,9,9,9,9,
 9,9,9,9,9,9,9,9,
 9,9,9,9,9,9,9,9,
  
};

int ghostp[72]={
  
  1,9,9,9,1,9,9,9,1,
    1,1,9,1,1,1,9,1,1,
  1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,
  1,3,2,1,1,3,2,1,1,
  1,2,2,1,1,2,2,1,9,
  9,1,1,1,1,1,1,1,9,
  9,9,1,1,1,1,1,9,9,
};
CRGB ghostpc[72];

CRGB palghost[4] = {CRGB::Black, CRGB::Red, CRGB::White, CRGB::Blue};

CRGB pacmancalc[256];



CRGB  ghost[14][14] = {
  {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 1},
  {1, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1},
  {0, 1, 1, 3, 3, 2, 2, 1, 1, 3, 3, 2, 2, 1},
  {0, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1},
  {0, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0}
};
CRGB ghostred[296];
CRGB ghostyellow[296];
CRGB ghostgreen[296];
CRGB ghostPurple[296];
CRGB ghostCyan[296];


CRGB fraise[11][10] = {
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 1, 2, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 2, 1, 0, 0},
  {0, 1, 2, 1, 2, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 2, 1, 0},
  {1, 1, 2, 1, 2, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 2, 1, 2, 1},
  {1, 2, 1, 2, 1, 3, 1, 1, 1, 1},
  {0, 1, 1, 1, 3, 3, 3, 1, 2, 0},
  {0, 0, 3, 3, 3, 2, 3, 3, 3, 0},
  {0, 0, 0, 0, 0, 2, 0, 0, 0, 0}
};


int YO = 0;

int cerise[196] = {
  100, 100, 100, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100,
  100, 100, 0, 1, 1, 1, 1, 0, 100, 100, 100, 100, 100, 100,
  100, 0, 1, 1, 2, 2, 1, 1, 0, 0, 0, 0, 100, 100,
  100, 0, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 100,
  100, 0, 1, 1, 1, 1, 1, 1, 0, 2, 2, 1, 1, 0,
  100, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 2, 1, 0,
  100, 100, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0,
  100, 100, 100, 0, 0, 3, 0, 1, 1, 1, 1, 1, 1, 0,
  100, 100, 100, 0, 3, 0, 100, 0, 1, 1, 1, 1, 0, 100,
  100, 100, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 100, 100,
  100, 0, 0, 3, 0, 3, 3, 0, 100, 100, 100, 100, 100, 100,
  0, 3, 3, 3, 3, 0, 0, 100, 100, 100, 100, 100, 100, 100,
  0, 3, 3, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100,
  100, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100
};

int mario[238]={
   100,3,3,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
   100,3,3,3,2,2,2,2,2,2,100,100,100,100,100,100,100, 
   100,100,3,3,2,2,2,2,2,2,2,2,2,2,3,3,100,
   100,100,100,100,2,2,2,2,4,2,2,2,2,2,3,3,100,
    100,5,5,1,1,1,1,2,2,2,2,4,2,2,3,3,100, 
  5,5,1,1,1,1,1,2,1,1,1,2,100,100,3,3,100, 
    5,5,1,1,1,1,2,1,1,1,2,1,1,100,100,3,100,
  100,100,100,100,100,5,5,5,5,5,5,5,1,1,100,100,100,
  100,100,100,3,3,5,5,5,5,6,6,6,6,1,100,100,100,
  100,100,100,3,5,3,3,5,5,5,6,5,5,5,1,100,100,
  100,100,100,3,5,3,5,5,5,6,5,5,5,1,1,100,100,
  100,100,100,100,3,3,3,5,5,6,5,100,1,1,1,100,100, 
  100,100,100,100,1,1,1,1,1,1,1,1,1,5,5,100,100,
  100,100,100,100,100,1,1,1,1,1,100,100,5,5,5,100,100            
};




CRGB palec[4] = {CRGB::Black, CRGB::Red, CRGB::White, CRGB::Green};
CRGB palpacman[2]={CRGB::Black,CRGB::Yellow};

CRGB palm[7]={CRGB:: Red,CRGB::Red, CRGB::Blue,CRGB(98,65,7),CRGB::Yellow,CRGB(238,151,64),CRGB::Black};
CRGB pall[7]={CRGB:: Red,CRGB::Green, CRGB::Blue,CRGB(98,65,7),CRGB::Yellow,CRGB(238,151,64),CRGB::Black};

// Pin layouts on the teensy 3/3.1:
// WS2811_PORTD: 2,14,7,8,6,20,21,5
// WS2811_PORTC: 15,22,23,9,10,13,11,12,28,27,29,30 (these last 4 are pads on the bottom of the teensy)
// WS2811_PORTDC: 2,14,7,8,6,20,21,5,15,22,23,9,10,13,11,12 - 16 way parallel
//
// Pin layouts on the due
// WS2811_PORTA: 69,68,61,60,59,100,58,31 (note: pin 100 only available on the digix)
// WS2811_PORTB: 90,91,92,93,94,95,96,97 (note: only available on the digix)
// WS2811_PORTD: 25,26,27,28,14,15,29,11
//






byte stayalivewhenalive[9] ={0,0,1,1,0,0,0,0,0};
byte bornwhendead [9]={0,0,0,1,0,0,0,0,0};


//byte countNeighbours(byte x,byte y);
//byte isPixelOn(byte x,byte y);
//byte newStatus(byte x,byte y);

bool ongoing=false;



void displayTpic()
{
    for (int y=0;y<LED_HEIGHT;y++)
    {
        if(y%2==0)
            memcpy(&leds[LED_WIDTH * y], &Tpic[y *LED_WIDTH ],LED_WIDTH * sizeof(CRGB));
        else
            for (int x = 0; x < LED_WIDTH; x++)
            {
                   // leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - x] = Tpic[LED_WIDTH * y  + x  ];
                   leds[ (LED_WIDTH<<1) * ( (y>>1) + 1)   -(1+x)]=Tpic[LED_WIDTH * y  + x  ];
                   // leds[ (LED_WIDTH<<1)*( ((y+y0)>>1)+1)     -( 1+x+x0)]   =pic[w*y+x];
            }

    }
}


byte isPixelOn(byte x,byte y)
{
    int offset=0;
    if (x<0 or y<0 or x>=LED_WIDTH or y>=LED_HEIGHT)
        return 0;
    if(y%2==0)
        offset=x+y*LED_WIDTH;
    else
       // offset=2 * LED_WIDTH * ((int)floor(y / 2) + 1) -( 1 + x);
        offset=(LED_WIDTH<<1) * ( (y>>1) + 1)   -(1+x);
    if(leds[offset]!=bgColor and leds[offset]!=CRGB(0,0,0))
        return 1;
    else
        return 0;
}


byte countNeighbours(byte x,byte y)
{
    //byte nb=0;
    return isPixelOn(x-1,y)+isPixelOn(x+1,y)+isPixelOn(x-1,y-1)+isPixelOn(x,y-1)+isPixelOn(x+1,y-1)+isPixelOn(x-1,y+1)+isPixelOn(x,y+1)+isPixelOn(x+1,y+1);

}

byte newStatus(byte x,byte y)
{
    byte nb=countNeighbours(x,y);
    if(isPixelOn(x,y))
        return stayalivewhenalive[nb];
    else
        return bornwhendead[nb];
}

void gameOflife()
{
   long time1=ESP.getCycleCount();
   if(ongoing)
   {
    for(byte i=0;i<LED_WIDTH;i++)
        for(byte j=0;j<LED_HEIGHT;j++)
        {
            if(newStatus(i,j))
                Tpic[i+j*LED_WIDTH]=CRGB::Red;
            else
                Tpic[i+j*LED_WIDTH]=bgColor;
        }
   }
    else
    {
        fill(bgColor);
       /* for (int y=0;y<LED_HEIGHT;y++)
            Tpic[15+y*LED_WIDTH]=CRGB::Red;*/
        for(int l=0 ;l<LED_WIDTH*LED_HEIGHT;l++)
            if(random(20)%3==0)
                Tpic[l]=CRGB::Red;
        ongoing=true;
    }
    
    displayTpic();
    
    //delay(50);
}


/*

void displaypic( CRGB *pic, int x0, int y0, int h, int w)
{
  if (x0 >= LED_WIDTH or y0 >= LED_HEIGHT)
    return;
  int maxx = LED_WIDTH - x0;
  int maxxy = LED_HEIGHT - y0 ;
  int hmin = (h > maxxy) ? maxxy : h;
  int wmin = (w > maxx) ? maxx : w;
  // Serial.println(hmin);
  //Serial.println(wmin);
  for (int y = y0; y < hmin + y0; y++)
  {
    if (y >= 0) {
      if (y % 2 == 0)
      {
        if (x0 < 0)
        {
          int nh = wmin + x0;
          if (nh > 0)
            memcpy(&leds[LED_WIDTH * y], &pic[(y - y0) *w - x0], nh * sizeof(CRGB));
        }
        else
          memcpy(&leds[LED_WIDTH * y + x0], &pic[(y - y0)*w], wmin * sizeof(CRGB));
      }
      else
      {
        for (int x = x0; x < wmin + x0; x++)
        {
          if (x >= 0)
            leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - x] = pic[w * (y - y0) + x - x0 ];
        }
      }
    }
  }
}


*/
struct sortie
{
    int x0;
    int y0;
    int xmax;
    int ymax;
};
/*
struct sortie putOrientation(CRGB *pic,long int h,long int w,long int x0,long int y0)
{
 struct  sortie s;
  s.x0=0;
  s.y0=0;
  s.xmax=0;
  s.ymax=0;
    //CRGB * Tpic = (CRGB*)calloc(h*w,sizeof(CRGB));
    if(!Tpic)
    {
        Serial.println("pas possible");
        return s;//NULL;
    }
    switch(tableOrientation){
            
        case DOWN_RIGHT: //on ne fait rien
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( ((x+x0)+LED_WIDTH*(y+y0)) <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }
            }
            
            break;
            
        case DOWN_LEFT:
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0 <NUM_LEDS && LED_HEIGHT-1-x-x0 >=0 && y+y0>=0 && LED_HEIGHT-1-x-x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                    Tpic[LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT:
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)) <NUM_LEDS  && (x+x0) >=0 && LED_WIDTH-1-(y+y0)>=0 && (x+x0)<LED_HEIGHT && LED_WIDTH-1-(y+y0) <LED_WIDTH  )
                    Tpic[LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT:
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && LED_WIDTH-1-(x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && LED_WIDTH-1-(x+x0) <LED_WIDTH )
                        Tpic[LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
//on inverse la lecture des pixels au niveau des X on lit de gauche a droite
        case DOWN_RIGHT_INV: //on ne fait rien
          if(y0<0)
            s.y0=0;
          else
           s.y0=y0;
           
           if(y0+h>=LED_HEIGHT)
             s.ymax=LED_HEIGHT-1;
           else
              s.ymax=y0+h-1;

          if(LED_WIDTH-1-w-x0<0)
            s.x0=0;
           else
            s.x0=LED_WIDTH-1-w-x0;

           if(LED_WIDTH-1-x0>=LED_WIDTH)
           s.xmax=LED_WIDTH-1;
           else
           s.xmax=LED_WIDTH-1-x0;

            
            for(long int y=s.y0;y<s.ymax;y++)
            {
              //if(y+y0>=0 && y+y0 <LED_HEIGHT)
                for (long int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-x-x0+LED_WIDTH*y <NUM_LEDS && LED_WIDTH-1-x-x0 >=0  && LED_WIDTH-1-x-x0<LED_WIDTH && y>=0 && y <LED_HEIGHT)
                    {
                        Tpic[LED_WIDTH-1-x-x0+LED_WIDTH*y]=pic[w*(y-y0)+x];
                         // s.xmax=LED_WIDTH-1-x-x0;
                          
                    }
                }
            }
            
            break;
            
        case DOWN_LEFT_INV:
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(x+x0)+y+y0 <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                        Tpic[LED_WIDTH*(x+x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT_INV:
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+(y+y0)) <NUM_LEDS  && (x+x0) >=0 && (y+y0)>=0 && (x+x0)<LED_HEIGHT && (y+y0) <LED_WIDTH  )
                        Tpic[LED_WIDTH*((x+x0))+(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT_INV:
            for(long int y=0;y<h;y++)
            {
                for (long int x=0;x<w;x++)
                {
                    if( (x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && (x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && (x+x0) <LED_WIDTH )
                        Tpic[(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
            
            
    }
     return s;
    
}
*/

void putOrientation(CRGB *pic,int h,int w,int x0,int y0)
{
    //CRGB * Tpic = (CRGB*)calloc(h*w,sizeof(CRGB));
    if(!Tpic)
    {
        Serial.println("pas possible");
        return ;//NULL;
    }
    switch(tableOrientation){
            
        case DOWN_RIGHT: //on ne fait rien
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( ((x+x0)+LED_WIDTH*(y+y0)) <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }
            }
            
            break;
            
        case DOWN_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0 <NUM_LEDS && LED_HEIGHT-1-x-x0 >=0 && y+y0>=0 && LED_HEIGHT-1-x-x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                    Tpic[LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)) <NUM_LEDS  && (x+x0) >=0 && LED_WIDTH-1-(y+y0)>=0 && (x+x0)<LED_HEIGHT && LED_WIDTH-1-(y+y0) <LED_WIDTH  )
                    Tpic[LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && LED_WIDTH-1-(x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && LED_WIDTH-1-(x+x0) <LED_WIDTH )
                        Tpic[LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
//on inverse la lecture des pixels au niveau des X on lit de gauche a droite
        case DOWN_RIGHT_INV: //on ne fait rien
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0) <NUM_LEDS && LED_WIDTH-1-x-x0 >=0 && y+y0>=0 && LED_WIDTH-1-x-x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }
            }
            
            break;
            
        case DOWN_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(x+x0)+y+y0 <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                        Tpic[LED_WIDTH*(x+x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+(y+y0)) <NUM_LEDS  && (x+x0) >=0 && (y+y0)>=0 && (x+x0)<LED_HEIGHT && (y+y0) <LED_WIDTH  )
                        Tpic[LED_WIDTH*((x+x0))+(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && (x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && (x+x0) <LED_WIDTH )
                        Tpic[(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
            
            
    }
     //return Tpic;
    
}






void putOrientation2(CRGB *pic,int h,int w,int x0,int y0)
{
    //CRGB * Tpic = (CRGB*)calloc(h*w,sizeof(CRGB));
    if(!Tpic)
    {
        Serial.println("pas possible");
        return ;//NULL;
    }
    switch(tableOrientation){
            
        case DOWN_RIGHT: //on ne fait rien
         /*   for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( ((x+x0)+LED_WIDTH*(y+y0)) <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        Tpic[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }
            }
*/

        for(int y=0;y<h;y++)
            {

           // if(pic[w*y+x]!=bgColor)
            //{
              if((y+y0)%2==0)
              {
             /* for (int x=0;x<w;x++)
                {
                   // if( LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0) <NUM_LEDS && LED_WIDTH-1-x-x0 >=0 && y+y0>=0 && LED_WIDTH-1-x-x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        //leds[LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                      //  if(pic[w*y+x]!=bgColor)
                      leds[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }*/
               memcpy((char *) leds +(LED_WIDTH*(y+y0) +x0)*sizeof(CRGB), (char *) pic +(w*y)*sizeof(CRGB), w*sizeof(CRGB));
              }
              else
              {
                for (int x=0;x<w;x++)
                {
                   // if( LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0) <NUM_LEDS && LED_WIDTH-1-x-x0 >=0 && y+y0>=0 && LED_WIDTH-1-x-x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                   //leds[(LED_WIDTH <<1)* ( (y>>1) + 1) - (1 + x)]
                       // leds[LED_WIDTH-1-x-x0+ LED_WIDTH*(y+y0)]=pic[w*y+x];
                       //if(pic[w*y+x]!=bgColor) 
                       leds[ (LED_WIDTH<<1)*( ((y+y0)>>1)+1)     -( 1+x+x0)]   =pic[w*y+x];
                      
                }
               //  memcpy((char *) leds +(  (LED_WIDTH<<1) * ( ((y+y0)>>1)  + 1) - (1+x0+w-1) )*sizeof(CRGB), (char *) pic +(w*y)*sizeof(CRGB), w*sizeof(CRGB));
              }
         //  }
              
            }
            
            
            
            break;
            
        case DOWN_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0 <NUM_LEDS && LED_HEIGHT-1-x-x0 >=0 && y+y0>=0 && LED_HEIGHT-1-x-x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                    Tpic[LED_WIDTH*(LED_HEIGHT-1-x-x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)) <NUM_LEDS  && (x+x0) >=0 && LED_WIDTH-1-(y+y0)>=0 && (x+x0)<LED_HEIGHT && LED_WIDTH-1-(y+y0) <LED_WIDTH  )
                    Tpic[LED_WIDTH*((x+x0))+LED_WIDTH-1-(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && LED_WIDTH-1-(x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && LED_WIDTH-1-(x+x0) <LED_WIDTH )
                        Tpic[LED_WIDTH-1-(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
//on inverse la lecture des pixels au niveau des X on lit de gauche a droite
        case DOWN_RIGHT_INV: //on ne fait rien
      // Serial.printf("%d %d %d %d\n",x0,y0,h,w);
            for(int y=0;y<h;y++)
            {

              if((y+y0)%2==0)
              {
              for (int x=0;x<w;x++)
                {
                   // if( LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0) <NUM_LEDS && LED_WIDTH-1-x-x0 >=0 && y+y0>=0 && LED_WIDTH-1-x-x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                        leds[LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                      // leds[x+x0+LED_WIDTH*(y+y0)]=pic[w*y+x];
                }
              }
              else
              {
                for (int x=0;x<w;x++)
                {
                   // if( LED_WIDTH-1-x-x0+LED_WIDTH*(y+y0) <NUM_LEDS && LED_WIDTH-1-x-x0 >=0 && y+y0>=0 && LED_WIDTH-1-x-x0<LED_WIDTH && y+y0 <LED_HEIGHT)
                   //leds[(LED_WIDTH <<1)* ( (y>>1) + 1) - (1 + x)]
                       // leds[LED_WIDTH-1-x-x0+ LED_WIDTH*(y+y0)]=pic[w*y+x];
                       leds[ (LED_WIDTH<<1)*( ((y+y0)>>1)+1)     -( 1+LED_WIDTH-1-x-x0)]   =pic[w*y+x];
                }
              }
            }
            
            break;
            
        case DOWN_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( LED_WIDTH*(x+x0)+y+y0 <NUM_LEDS && x+x0 >=0 && y+y0>=0 && x+x0<LED_HEIGHT && y+y0 <LED_WIDTH)
                        Tpic[LED_WIDTH*(x+x0)+y+y0]=pic[w*y+x];
                }
            }
            break;
            
            
        case UP_RIGHT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (LED_WIDTH*((x+x0))+(y+y0)) <NUM_LEDS  && (x+x0) >=0 && (y+y0)>=0 && (x+x0)<LED_HEIGHT && (y+y0) <LED_WIDTH  )
                        Tpic[LED_WIDTH*((x+x0))+(y+y0)]=pic[w*y+x];
                }
            }
            break;
            
        case UP_LEFT_INV:
            for(int y=0;y<h;y++)
            {
                for (int x=0;x<w;x++)
                {
                    if( (x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0)) <NUM_LEDS  && (x+x0)>=0 && LED_WIDTH*(LED_HEIGHT-1-(y+y0))>=0 && (LED_HEIGHT-1-(y+y0))<LED_HEIGHT && (x+x0) <LED_WIDTH )
                        Tpic[(x+x0)+LED_WIDTH*(LED_HEIGHT-1-(y+y0))]=pic[w*y+x];
                }
            }
            break;
            
            
            
    }
     //return Tpic;
    
}


void displaygif( CRGB *pica, long int x0, long int y0, long int h, long int w,long pic)
{

 CRGB *bitmapRGB =(CRGB*)malloc(w*h*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }

 memcpy ( bitmapRGB, pica+(h*w*pic), h*w*sizeof(CRGB) );
 displayPicNew( bitmapRGB, x0, y0,  h,  w);

    free(bitmapRGB);  
}


void displaygifdir( CRGB *pica, long int x0, long int y0, long int h, long int w,long  pic,int direc)
{

 CRGB *bitmapRGB =(CRGB*)malloc(w*h*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }

 memcpy ( bitmapRGB, pica+(h*w*pic), h*w*sizeof(CRGB) );
//Serial.println((h*w*pic)*sizeof(CRGB));
switch(direc){

  case 1:
  for(int i=0 ;i<h;i++)
  {
    for(int j=0;j<int(w/2);j++)
    {
      CRGB dd=bitmapRGB[i*w+j];
      bitmapRGB[i*w+j]=bitmapRGB[i*w+(w-1-j)];
      bitmapRGB[i*w+(w-j-1)]=dd;
    }
      
  }
  
  break;


  case 2:
  
  //Symetrie diagoonale
  for(int i=0 ;i<=h-1;i++)
  {
    for(int j=i+1;j<w;j++)
    {
      CRGB dd=bitmapRGB[i*w+j];
      bitmapRGB[i*w+j]=bitmapRGB[j*w+i];
      bitmapRGB[j*w+i]=dd;
    }
      
  }

//sympetrie horizonatle

   for(int i=0 ;i<int(h/2);i++)
  {
    for(int j=0;j<w;j++)
    {
      CRGB dd=bitmapRGB[i*w+j];
      bitmapRGB[i*w+j]=bitmapRGB[(h-1-i)*w+j];
      bitmapRGB[(h-1-i)*w+j]=dd;
    }
      
  } 
  
  break;

case 4:

for(int i=0 ;i<h;i++)
  {
    for(int j=0;j<int(w/2);j++)
    {
      CRGB dd=bitmapRGB[i*w+j];
      bitmapRGB[i*w+j]=bitmapRGB[i*w+(w-1-j)];
      bitmapRGB[i*w+(w-j-1)]=dd;
    }
      
  }

  
  //Symetrie diagoonale
  for(int i=0 ;i<=h-1;i++)
  {
    for(int j=i+1;j<w;j++)
    {
      CRGB dd=bitmapRGB[i*w+j];
      bitmapRGB[i*w+j]=bitmapRGB[j*w+i];
      bitmapRGB[j*w+i]=dd;
    }
      
  }

//sympetrie horizonatle

   for(int i=0 ;i<int(h/2);i++)
  {
    for(int j=0;j<w;j++)
    {
      CRGB dd=bitmapRGB[i*w+j];
      bitmapRGB[i*w+j]=bitmapRGB[(h-1-i)*w+j];
      bitmapRGB[(h-1-i)*w+j]=dd;
    }
      
  } 
  


  
  
  break;
  

  
}
 
 displayPicNew( bitmapRGB, x0, y0,  h,  w);

    free(bitmapRGB);  
}

/*
void displaypic( CRGB *pica, long int x0, long int y0, long int h, long int w)
{
    struct sortie s =putOrientation(pica,h,w,x0,y0);
    if(Tpic)
    {
       long int hmin=LED_HEIGHT;
       long int  wmin=LED_WIDTH;
       long int off;
       long int off2;
       
        x0=s.x0;
        y0=s.y0;
        wmin=s.xmax-s.x0;
        if (s.xmax <0)
        return;
        if(s.ymax<0)
        return;
       Serial.printf("%d \n",s.ymax);
        for (long int y = y0; y < s.ymax; y++)
        {
            if (y >= 0) {
                if (y % 2 == 0)
                {
                  if (x0 < 0)
                    {
                        long int nh = wmin + x0;
                        if (nh > 0)
                            memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
                    }
                    else
                        memcpy(&leds[LED_WIDTH * y + x0], &Tpic[(y - y0)*wmin], wmin * sizeof(CRGB));
                }
                else
                {
                  
                 //CRGB *pTpic=Tpic+wmin * (y - y0);
                 // CRGB *pLeds=leds+(LED_WIDTH <<1)* ( (y>>1) + 1) - 1;
                    for (long int x = x0; x < wmin + x0; x++)
                    {
                        //if (x >= 0)                                           
                          //leds[2 * LED_WIDTH * ((long int)floor(y / 2) + 1) - 1 - x] = Tpic[wmin * (y - y0) + x - x0 ];
                           leds[(LED_WIDTH <<1)* ( (y>>1) + 1) - (1 + x)] = Tpic[wmin * (y - y0) + x - x0 ];
                        //   *pLeds=*pTpic;
                         //  *pLeds--;
                         //  *pTpic++;
                           
                    }
                }
            }
        }
       
    }
}
*/



void displayPicDouble(CRGB *pica, int x0, int y0, int h, int w)
{
  CRGB *newpic=(CRGB*)malloc(4*h*w*sizeof(CRGB));
    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
        {
            
                //   Serial.print("e:");
                //Serial.println(activeBlocks[blockType1 + (7 * state1)][x][y]);
            newpic[2*y*2*w+2*x]=pica[x+y*w];
            newpic[2*y*2*w+2*x+1]=pica[x+y*w];
            newpic[(2*y+1)*2*w+2*x]=pica[x+y*w];
            newpic[(2*y+1)*2*w+2*x+1]=pica[x+y*w];
          
            
        }
        
  displayPicNewInv(newpic,x0,y0,2*h,2*w);
  free(newpic);
 }


void displayPicNew(CRGB *pica, int x0, int y0, int h, int w)
{

    int max_wide=0;
    int wide=w+x0;
    int offsety;
    int offsetx;
    int newx=0;
    int newy=0;
    if(wide<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_wide=LED_WIDTH;
    else
        max_wide=LED_HEIGHT;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<max_wide)
            wide=w;
        else
            wide=max_wide-x0;
    }
    else
    {
        newx=0;
        offsetx=-x0;
        if(w+x0<max_wide)
            wide=w+x0;
        else
            wide=max_wide;
    }
    
    
    
    
    int max_height=0;
    int height=h+y0;
    if(height<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_height=LED_HEIGHT;
    else
        max_height=LED_WIDTH;
    
    if (y0>=max_height) //on est en dehors du cadre
        return;
    if(y0>=0)
    {
        newy=y0;
        offsety=0;
        if(h+y0<max_height)
            height=h;
        else
            height=max_height-y0;
    }
    else
    {
        newy=0;
        offsety=-y0;
        if(h+y0<max_height)
            height=h+y0;
        else
            height=max_height;
    }
    
    
    int r;
    int g;
    int b;

   CRGB *bitmapRGB =(CRGB*)malloc(wide*height*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.printf("immossibnle de créer l'image im %d %d\n",wide,height);
        return;
    }
//Serial.println("debut dessin");
    for(int y=0;y<height;y++){
       /* for(int x=0;x<wide;x++){
          
            
        //    r= gammar[(int)pgm_read_byte(pic+ 3*((y+offsety)*w+x+offsetx))];
         //   g= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+1)];
          //  b= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+2)];
            bitmapRGB[x+y*wide]=pica[(y+offsety)*w+x+offsetx];
            //Serial.printf("%03X ",pica[(y+offsety)*w+x+offsetx]);//I have a program that switches directly from
        }*/
        // memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
        //Serial.println("ici create bitmaap");
        memcpy(bitmapRGB+wide*y,&pica[(y+offsety)*w+offsetx],wide*sizeof(CRGB));
     // Serial.println();
    }
    //putOrientation(bitmapRGB,newx,newy,height,wide);
    // Serial.println("fin dessin");
    putOrientation2(bitmapRGB,height,wide,newx,newy);
   // displaypic(bitmapRGB,newx,newy,height,wide);
    free(bitmapRGB);
    
}




void displayPicNewInv(CRGB *pica, int x0, int y0, int h, int w)
{

    int max_wide=0;
    int wide=w+x0;
    int offsety;
    int offsetx;
    int newx=0;
    int newy=0;
    if(wide<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_wide=LED_WIDTH;
    else
        max_wide=LED_HEIGHT;
    
    if (x0>=max_wide) //on est en dehors du cadre
        return;
    if(x0>=0)
    {
        newx=x0;
        offsetx=0;
        if(w+x0<max_wide)
            wide=w;
        else
            wide=max_wide-x0;
    }
    else
    {
        newx=0;
        offsetx=-x0;
        if(w+x0<max_wide)
            wide=w+x0;
        else
            wide=max_wide;
    }
    
    
    
    
    int max_height=0;
    int height=h+y0;
    if(height<=0)
        return; // l'image ne peut etre affichée
    if(tableOrientation%2==0)
        max_height=LED_HEIGHT;
    else
        max_height=LED_WIDTH;
    
    if (y0>=max_height) //on est en dehors du cadre
        return;
    if(y0>=0)
    {
        newy=y0;
        offsety=0;
        if(h+y0<max_height)
            height=h;
        else
            height=max_height-y0;
    }
    else
    {
        newy=0;
        offsety=-y0;
        if(h+y0<max_height)
            height=h+y0;
        else
            height=max_height;
    }
    
    
    int r;
    int g;
    int b;

   CRGB *bitmapRGB =(CRGB*)malloc(wide*height*sizeof(CRGB)) ;
    if(bitmapRGB==NULL)
    {
        Serial.println("immossibnle de créer l'image");
        return;
    }
//Serial.println("debut dessin");
    for(int y=0;y<height;y++){
        for(int x=0;x<wide;x++){
          
            
        //    r= gammar[(int)pgm_read_byte(pic+ 3*((y+offsety)*w+x+offsetx))];
         //   g= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+1)];
          //  b= gamma8[(int)pgm_read_byte(pic+3*((y+offsety)*w+x+offsetx)+2)];
            bitmapRGB[x+y*wide]=pica[(y+offsety)*w+w-1-x-offsetx];
            //Serial.printf("%03X ",pica[(y+offsety)*w+x+offsetx]);//I have a program that switches directly from
        }
        // memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
        //Serial.println("ici create bitmaap");
      // memcpy(bitmapRGB+wide*y,&pica[(y+offsety)*w+offsetx],wide*sizeof(CRGB));
     // Serial.println();
    }
    //putOrientation(bitmapRGB,newx,newy,height,wide);
    // Serial.println("fin dessin");
    putOrientation2(bitmapRGB,height,wide,newx,newy);
   // displaypic(bitmapRGB,newx,newy,height,wide);
    free(bitmapRGB);
    
}




void displaypic( CRGB *pica, int x0, int y0, int h, int w)
{
    putOrientation(pica,h,w,x0,y0);
    if(Tpic)
    {
       int hmin=LED_HEIGHT;
       int  wmin=LED_WIDTH;
        x0=0;
        y0=0;
        for (int y = y0; y < hmin + y0; y++)
        {
            if (y >= 0) {
                if (y % 2 == 0)
                {
                    if (x0 < 0)
                    {
                        int nh = wmin + x0;
                        if (nh > 0)
                            memcpy(&leds[LED_WIDTH * y], &Tpic[(y - y0) *wmin - x0], nh * sizeof(CRGB));
                    }
                    else
                        memcpy(&leds[LED_WIDTH * y + x0], &Tpic[(y - y0)*wmin], wmin * sizeof(CRGB));
                }
                else
                {
                    for (int x = x0; x < wmin + x0; x++)
                    {
                        if (x >= 0)
                            //leds[2 * LED_WIDTH * ((int)floor(y / 2) + 1) - 1 - x] = Tpic[wmin * (y - y0) + x - x0 ];
                             leds[(LED_WIDTH <<1)* ( (y>>1) + 1) - (1 + x)] = Tpic[wmin * (y - y0) + x - x0 ];
                    }
                }
            }
        }
       
    }
}

void calculpic(int *pic, CRGB *palette, CRGB color, int h, int w,CRGB *result)
{
  Serial.println("on est là");
  int to = h * w;
 // static CRGB result[296];
  /*for (int m = 0; m < h; m++)
  {
    for (int y = 0; y < w; y++)
    {

      result[m * w + y] = palette[pic[m * w + y]];
      //Serial.print(palette[pic[m * w + y]]);
      //Serial.printf(":%d\n", pic[m * w + y]);


      if (pic[m * w + y] == 100)
      {
        result[m * w + y] = color;

      }
      if (pic[m * w + y] == 9)
      {
        result[m * w + y] = color;

      }

    }
  }*/

  for(int i=0;i<h*w;i++)
  {
    result[i] = palette[pic[i]];
    if (pic[i] == 100)
      {
        result[i] = color;

      }
      if (pic[i] == 9)
      {
        result[i] = color;

      }
  }
  
 // return result;

}




void calculghosts(CRGB color)
{
  for (int m = 0; m < 14; m++)
  {
    for (int y = 0; y < 14; y++)
    {
      if (ghost[m][y] == (CRGB)1)
      {
        ghostred[m * 14 + y] = CRGB :: Red;
        ghostyellow[m * 14 + y] = CRGB :: Yellow;
        ghostgreen[m * 14 + y] = CRGB :: Green;
        ghostPurple[m * 14 + y] = CRGB :: Purple;
        ghostCyan[m * 14 + y] = CRGB(64, 222, 255);
      }
      if (ghost[m][y] == (CRGB)2)
      {
        ghostred[m * 14 + y] = CRGB :: Blue;
        ghostyellow[m * 14 + y] = CRGB :: Blue;
        ghostgreen[m * 14 + y] = CRGB :: Blue;
        ghostPurple[m * 14 + y] = CRGB :: Blue;
        ghostCyan[m * 14 + y] = CRGB :: Blue;
      }

      if (ghost[m][y] == (CRGB)3)
      {
        ghostred[m * 14 + y] = CRGB :: White;
        ghostyellow[m * 14 + y] = CRGB :: White;
        ghostgreen[m * 14 + y] = CRGB :: White;
        ghostPurple[m * 14 + y] = CRGB :: White;
        ghostCyan[m * 14 + y] = CRGB :: White;
      }
      if (ghost[m][y] == (CRGB)0)
      {
        ghostred[m * 14 + y] = color;
        ghostyellow[m * 14 + y] = color;
        ghostgreen[m * 14 + y] = color;
        ghostPurple[m * 14 + y] = color;
        ghostCyan[m * 14 + y] = color;
      }

    }
  }
}




CRGB cerisecalc[196];

CRGB mariocalc[238];
CRGB luigicalc[238];

void calculfraise(CRGB color)
{
  for (int m = 0; m < 11; m++)
  {
    for (int y = 0; y < 10; y++)
    {
      if (fraise[m][y] == (CRGB)1)
      {
        fraiseCalc[m * 10 + y] = CRGB :: Red;

      }
      if (fraise[m][y] == (CRGB)2)
      {
        fraiseCalc[m * 10 + y] = CRGB :: White;

      }

      if (fraise[m][y] == (CRGB)3)
      {
        fraiseCalc[m * 10 + y] = CRGB :: Green;

      }
      if (fraise[m][y] == (CRGB)0)
      {
        fraiseCalc[m * 10 + y] = color;

      }

    }
  }
}


void fill(CRGB color)
{
     fill_solid(leds, NUM_LEDS, color);
    fill_solid(Tpic, NUM_LEDS, color);
}


#include "pacmanlib.h"
int directionp=1;
int onbouge=-1;


//position virtuelle de pacman sur le plateau
int cpacx=0;
int vpacy=2;






bool canMoveLeft(int x,int y)
{
  int tilex=((x-1)+2)>>2; // on enleve 1 pour le decalge par de l'affichage de la map
  int tiley=(y+2)>>2;
if ( pacboard[(tilex-1)+28*tiley]==15)
  return true;
 else
 return false;
    

}




bool canMoveRight(int x,int y)
{
int tilex=((x-1)+2)>>2; // on enleve 1 pour le decalge par de l'affichage de la map
  int tiley=(y+2)>>2;
 Serial.printf("droite %d %d : %d\n",tilex,tiley,pacboard[(tilex+1)+28*tiley]);
if ( pacboard[(tilex+1)+28*tiley]==15)
  return true;
 else
 return false;

}






bool canMoveUp(int x,int y)
{

    int tilex=((x-1)+2)>>2; // on enleve 1 pour le decalge par de l'affichage de la map
  int tiley=(y+2)>>2;
if ( pacboard[(tilex)+28*(tiley+1)]==15)
  return true;
 else
 return false;

}




bool canMoveDown(int x,int y)
{
int tilex=((x-1)+2)>>2; // on enleve 1 pour le decalge par de l'affichage de la map
  int tiley=(y+2)>>2;
if ( pacboard[tilex+28*(tiley-1)]==15)
  return true;
 else
 return false;
    

}



void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
           // USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
          // USE_SERIAL.printf("[%u] get Text: %sdd\n", num, payload);
           if (strcmp("UP", (const char *)payload) == 0)  {

                //if(pacy<LED_HEIGHT/2-5)
                 // pacy++;
                Serial.println("UP");
                if(onbouge==-1)
                {
                  face++;
                  directionp=2;
                  onbouge=0;
                }
                
            }
            if (strcmp("DOWN", (const char *)payload) == 0)  {
                   //             if(pacy>0)
                  //pacy--;
                // Serial.println("on va a droite");
                 if(onbouge==-1)
                {
                face++;
                 directionp=4;
                 onbouge=0;
                }
            }
            if (strcmp("RIGHT", (const char *)payload) == 0)  {
                //if(pacx<LED_WIDTH-5)
                 // pacx++;                
                 Serial.println("on va a droite");
                if(onbouge==-1)
                {
                face++;
                 directionp=1;
                 onbouge=0;
                }
            }
            if (strcmp("LEFT", (const char *)payload) == 0)  {
                
                // Serial.println("on va a droite");
                //if(pacx>0)
                 // pacx--;
                  if(onbouge==-1)
                {
                  face++;
                  directionp=3;
                  onbouge=0;
                }
            }
            // send message to client
            // webSocket.sendTXT(num, "message here");

            // send data to all connected clients
            // webSocket.broadcastTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[%u] get binary length: %u\n", num, length);
           // hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}

#include "tetris3.h"

#include "tetris2.h"
 #include "paint.h"

bool firsttime=true;


void initServer()
{
    server.serveStatic("/", SPIFFS, "/index.html");
    server.serveStatic("/index.html", SPIFFS, "/index.html");
    server.serveStatic("/jquery.js", SPIFFS, "/jquery.js");
    
    server.serveStatic("/farbtastic.js", SPIFFS, "/farbtastic.js");
    
    server.serveStatic("/farbtastic.css", SPIFFS, "/farbtastic.css");
    
    server.serveStatic("/marker.png", SPIFFS, "/marker.png");
    
    server.serveStatic("/mask.png", SPIFFS, "/mask.png");
    
    server.serveStatic("/wheel.png", SPIFFS, "/wheel.png");


server.on("/anim", HTTP_GET, [] {
        //fill_solid(leds, NUM_LEDS, solidColor);
        //FastLED.show();
        Serial.println("anim");
         anim=(anim+1)%11;
         if (anim==8)
     {
           timerAlarmDisable(timer);
          inGame=false;
     }
         if(anim==0)
         {
          //free(artnetPacket);
           // Udp.stop();
         anim=1;
          
         }
        if( anim==6)
        {
            free(artnetPacket2);
            Udp2.stop();
    }
         firsttime=true;
              server.send ( 200, "text/plain", "this works as well" );
        
        // i=0;
    });
    
      server.on("/changetext", HTTP_GET, []() {
        //fill_solid(leds, NUM_LEDS, solidColor);
        //FastLED.show();
        
     String g = server.arg("v");
        Serial.println(g);
        //strcopy(g);
        //mess = g;
       // k2 = 0;
        //Serial.println(mess);
        int lens = g.length();
        g.toCharArray(mess, lens+1);
        //Serial.println(lens);
        //fill_solid(leds, NUM_LEDS, bgColor);
       // FastLED.show();
        server.send(200, "text/html", "done");
        // i=0;
    });


    
    server.on("/up", HTTP_GET, [] {
        //fill_solid(leds, NUM_LEDS, solidColor);
        //FastLED.show();
         Serial.printf("hhh");
        //String g = server.arg("v");
              tableOrientation=(tableOrientation+1)%8;
              server.send ( 200, "text/plain", "this works as well" );
        
        // i=0;
    });
    server.on("/down", HTTP_GET, [] {
        //fill_solid(leds, NUM_LEDS, solidColor);
        //FastLED.show();
        Serial.printf("hhh");
        //String g = server.arg("v");
        tableOrientation=abs((tableOrientation-1))%8;
        server.send ( 200, "text/plain", "this works as well" );
        
        // i=0;
    });

    initlamp();
  initTetris();
  initPaint();
    
}




  uint32_t syncmax1=0;
  uint32_t syncmax2=0;
void setup() {
  String g="message depart";
 // artnet.setframe((CRGB*)malloc(48*64*sizeof(CRGB))) ;
  g.toCharArray(mess, g.length()+1);
  anim=5;
  xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 1000, NULL,2, &FastLEDshowTaskHandle, FASTLED_SHOW_CORE);
  xTaskCreatePinnedToCore(FastLEDshowTask2, "FastLEDshowTask2", 2000, NULL,3, &FastLEDshowTaskHandle2, FASTLED_SHOW_CORE);
  Serial.begin(115200);
  calculatecos();
  // LEDS.addLeds<WS2812_PORTA,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);
  // LEDS.addLeds<WS2811_PORTB,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);
  // LEDS.addLeds<WS2811_PORTD,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP).setCorrection(TypicalLEDStrip);

//Serial.printf("mask %lu\n",PINS_MASK);
  FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,PORT_MASK>(leds, NUM_LEDS_PER_STRIP);
 //FastLED.addLeds<WS2811_PORTA,NUM_STRIPS,0>(leds, NUM_LEDS_PER_STRIP);
// FastLED.addLeds<APA102>(leds, NUM_LEDS);
 //FastLED.addLeds<APA102, 3,12, RGB>(leds, NUM_LEDS_PER_STRIP);
// FastLED.addLeds<APA102, 4,12,RGB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
 //FastLED.addLeds<APA102, 5,12,RGB>(leds, NUM_LEDS_PER_STRIP*2, NUM_LEDS_PER_STRIP);
//FastLED.addLeds<NEOPIXEL, 12>(leds, 0, NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 11, starting at index 60 in the led array
  //FastLED.addLeds<NEOPIXEL, 13>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  // tell FastLED there's 60 NEOPIXEL leds on pin 12, starting at index 120 in the led array
  //FastLED.addLeds<NEOPIXEL, 14>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  //FastLED.setBrightness(32);
setTableBrightness(64);
  fill(CRGB::Black);
  FastLED.show();
  FastLED.delay(2000);
  
    Color=CRGB(255,0,0);
        fill(CRGB(0, 2555, 255));
FastLEDshowESP32();
//delay(1000);
    fill(CRGB(0, 0, 255));
//FastLED.show();
FastLEDshowESP32();
//FastLED.show();
    afficheMessage("INIT",5,5);
    FastLEDshowESP32();
   //   FastLED.show();
     //show();
      FastLED.delay(20 );
  // delay(1000);
  solidColor=bgColor;
  calculghosts(solidColor);
  calculfraise(solidColor);
  calculpic(cerise, palec, solidColor, 14, 14,cerisecalc);
   calculpic(mario, palm, solidColor, 14, 17,mariocalc);
     
  calculpic(mario, pall, bgColor, 14, 17, luigicalc);
  bgColor=CRGB::Black;
   calculpic(pacmab, palpacman, CRGB(0,0,0),32,8,pacmancalc);
   
   Serial.printf("taille pacman :%d",sizeof(pacmancalc));
calculpic(ghostp, palghost, CRGB(0,0,0), 8, 9,ghostpc);
//calculpacboard();
//FastLEDshowESP32();
//FastLED.delay(200 );
static uint8_t buf[2500];

 WiFi.mode(WIFI_STA);
    Serial.printf("Connecting to %s\n", "WiFi-2.4-E19C");
    Serial.printf("Connecting ");
    WiFi.begin("WiFi-2.4-E19C", "yvesyves");
//WiFi.begin("DomainePutterie", "Jeremyyves");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println(WiFi.status());
      /*
Connecting ........[D][WiFiGeneric.cpp:293] _eventCallback(): Event: 5 - STA_DISCONNECTED
[W][WiFiGeneric.cpp:298] _eventCallback(): Reason: 2 - AUTH_EXPIRE
....[D][WiFiGeneric.cpp:293] _eventCallback(): Event: 5 - STA_DISCONNECTED
[W][WiFiGeneric.cpp:298] _eventCallback(): Reason: 201 - AUTH_FAIL
*/
      
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    

         SPIFFS.begin();
   /*  int levels=1;
     File root = SPIFFS.open("/");
      File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
 
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
             file.read(buf, 512);
            Serial.printf("bytes in %d\n %s\n",2,buf);
        }
        file = root.openNextFile();
    }*/

 //ws.onEvent(onWsEvent);
  //server.addHandler(&ws);

  //events.onConnect([](AsyncEventSourceClient *client){
   // client->send("hello!",NULL,millis(),1000);
  //});
  //server.addHandler(&events);


    //initTetris();
    initServer();
     server.begin();
     //webSocket.begin();
    //webSocket.onEvent(webSocketEvent);
    //initTetrisScoketControl();
       fill(CRGB(10, 10, 10));
       // FastLED.show();

         //artnet.begin(123*48,170);
           
           
 

  //leds.begin();
  //artnet.setBroadcast(broadcast);
  //initTest();

  // this will be called for each packet received
 // artnet.setArtDmxCallback(onDmxFrame);
  //artnet.setArtSyncCallback(onSync);

  
    afficheMessage("DONE",0,0);
     FastLEDshowESP32();
    // FastLEDshowESP32();
    // FastLED.show();
     //FastLED.delay(2000);
     fill(CRGB(10, 10, 10));

      FastLEDshowESP32();
     //FastLED.show();
     
     Serial.printf("ful\n");
  //fill(CRGB(0, 0, 0));
    // FastLED.show();
     k=0;
    // newGame();
int nbNeededUniverses=24;
    if(nbNeededUniverses<=32)
    {
        if(nbNeededUniverses<32)
            syncmax1=(1<<nbNeededUniverses)-1;
        else
            syncmax1=0xFFFFFFFF;
        syncmax2=0;
    }
    else
    {
            syncmax1=0xFFFFFFFF;
        if(nbNeededUniverses-32<32)
            syncmax2=(1<<(nbNeededUniverses-32))-1;
        else
            syncmax2=0xFFFFFFFF;
            //syncmax2=0;

    }
     
}

//int r=0;
//int dire=0;

int tempo(int k)
{
  if((k)%3==0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int ghostx=112-12;
int ghosty=47-9;


bool disp=false;

int oldmove=0;
int newmove=0;
bool firstpacket=false;

void loop() {
  //char mess[]="MOI JE SUIS QUI JE SUIS PAMELA";
 
  k = k % ((35+1)*5000-1);
  
    int offset = LED_WIDTH;
    long time0=ESP.getCycleCount();
       // fill(bgColor);
        long time1=ESP.getCycleCount();
        
    int  Y1;
/*
#define NUM_STRIPS 16
#define LED_WIDTH 30
#define LED_HEIGHT_PER_STRIP 14*/


switch (anim)
{

case 5:
{

  if(firsttime)
  {
    firstpacket=false;
    firsttime=false;
    fill(bgColor);
    afficheMessage("READY TO STREAM", 0, 20);
    replaceled();
    FastLEDshowESP32();
    FastLEDshowESP32();
    Udp2.begin(100);
     artnetPacket2=(char*)malloc((123*3*3+1)*sizeof(char));
     disp=false;
 
  }
  //Serial.println("here");
  uint32_t sync=0;
  uint32_t sync2=0;

 while (sync!=syncmax1 or sync2!=syncmax2)//sync!=syncmax or sync2!=syncmax2 
 { 
int packetSize = Udp2.parsePacket();
      if(packetSize>0)
      {
       // Serial.printf("size:%d\n",packetSize);
        firstpacket=true;
      Udp2.read(artnetPacket2, packetSize);
      memcpy(&Tpic[123*2*(artnetPacket2[0])],artnetPacket2 + 1,123*3*2);
//      Serial.printf("univers:%d\n",artnetPacket[0]);
      if(artnetPacket2[0]==255)
      {
        Serial.printf("new value bru:%d\n",artnetPacket2[1]);
        FastLED.setBrightness(artnetPacket2[1]);
      }
      else
      {
            if (artnetPacket2[0]==0)
            {
              sync=1;
              sync2=0;
            }
             else
             {
               if(artnetPacket2[0]<32)
                 sync=sync  | (1<<artnetPacket2[0]);
               else
               sync2=sync2  | (1<<(artnetPacket2[0]-32));
             
             }
      } 
      //free(artnetPacket);
      }
      else
      { 
        if (!firstpacket)
        {
         break;   
        }
    }
 }
 if(firstpacket)
 FastLEDshowESP322();
}

  break;

  
  case 0:
  {

    if(firsttime)
  {
    firsttime=false;
    fill(bgColor);
    afficheMessage("READY FOR ARTNET", 1, 20);
    replaceled();
    FastLEDshowESP32();
    FastLEDshowESP32();
  }
    disp=false;
  //artnet.read();

              FastLEDshowESP322();
               //xTaskNotifyGive(FastLEDshowTaskHandle2);
    //          artnet.resetsync();
             // Serial.println("on affiche");
              
///          }

  }
  break;
case 1: 
{
   if(firsttime)
 {
  setTableBrightness(64);
  firsttime=false;
 }
  disp=true;
bgColor=CRGB(10,10,10);
fill(bgColor);

 if(dire>=0)
             {
                r=r+1;
                if(r>100)
                  dire=-1;
             }
              else
              {
                r=r-1;
                if(r<1)
                  dire=0;
              }
              //r=(r+(int)k/10)%20;
             //fill(CRGB(5, 5, 5));
              dessinePoly(61,24, r,k*pi/360,3,CRGB::Red);
             int r1=r-10;
             dessinePoly(61,24, r1,k*1.5*pi/360,6,CRGB::Green);
  
              r1=r-20;
 
             dessinePoly(61,24, r1,-k*pi/360,5,CRGB::Yellow);
              r1=r-30;
           dessinePoly(61,24, r1,+k*2*pi/360,4,CRGB::Blue);
                     
              r1=r-40;      
             dessinePoly(61,24, r1,-k*1.5*pi/360,3,CRGB::Purple);
             r1=r-50;
             dessinePoly(61,24, r1,k*1.5*pi/360,6,CRGB::Green);
  
              r1=r-60;
 
             dessinePoly(61,24, r1,-k*pi/360,5,CRGB::Yellow);
              r1=r-70;
             dessinePoly(61,24, r1,+k*2*pi/360,4,CRGB::Red);
                     
              r1=r-80;      
             dessinePoly(61,24, r1,-k*1.5*pi/360,3,CRGB::Blue);
             replaceled();

}
break;

case 2:
{
   if(firsttime)
 {
  setTableBrightness(64);
  firsttime=false;
 }
            // k=k+15;
      disp=true;
bgColor=CRGB(10,10,10);
fill(bgColor);

    if(dire>=0)
             {
                r=r+1;
                if(r>60)
                  dire=-1;
             }
              else
              {
                r=r-1;
                if(r<-10)
                  dire=0;
              }
                 //fill(CRGB(5, 5, 5));
                 //solidfill(leds,
            circleFilled(15,8,r+15,CRGB::Yellow);
             circleFilled(50,8,r+10,CRGB::Green);
             circleFilled(15,20,r+5,CRGB::Red);
             circleFilled(15,10,r,CRGB::Purple);
             circleFilled(110,12,r-5,CRGB::Cyan);
             circleFilled(90,20,r-10,CRGB:: Blue);
             circleFilled(70,8,r-15,CRGB::Yellow);
             circleFilled(20,20,r-15,CRGB::Yellow);
             circleFilled(30,12,r-20,CRGB::Red);
             circleFilled(45,12,r-25,CRGB::Cyan);
             circleFilled(7,36,r-25,CRGB::Purple);
              circleFilled(15,8,r+15,CRGB::Yellow);
             circleFilled(65,8,r+10,CRGB::Blue);
             circleFilled(80,10,r+5,CRGB::Red);
             circleFilled(15,10,r,CRGB::Purple);
             circleFilled(15,0,r-5,CRGB::Cyan);
             circleFilled(90,13,r-10,CRGB:: Blue);
             circleFilled(70,8,r-15,CRGB::Black);
             circleFilled(10,25,r-15,CRGB::Yellow);
             circleFilled(35,12,r-20,CRGB::Red);
             circleFilled(50,40,r-25,CRGB::Cyan);
             circleFilled(110,40,r-25,CRGB::Purple);
             replaceled();
             delay(1);

 
/*
              circleFilled(7,20,r-25,CRGB::Purple);
              circleFilled(15,16,r+15,CRGB::Yellow);
             circleFilled(100,30,r+10,CRGB::Blue);
             circleFilled(40,22,r+5,CRGB(30,57,255));
             circleFilled(15,19,r,CRGB::Purple);
             circleFilled(0,23,r-5,CRGB(78,23,90));*/
             //delay(500);
             //ligne(x1,y1,x2,y2);
             //BresenhamEntier(x1,y1,x2,y2);

}
break;

case 3:
{
           if(firsttime)
 {
  setTableBrightness(64);
  firsttime=false;
 }   
        disp=true;
bgColor=CRGB(10,10,10);
int f=110+30+260+10+30+120+70+20;
fill(bgColor);
// displayBitmapFromProgmem(mariomap,-k%1500+30,YO+r,40,703);
 // displayBitmapFromProgmem(mariomap ,0,YO,40,792);
for(int l=0 ;l<1;l++)
{
    Y1=0;
    displayPicDouble(cerisecalc, -k%f+ offset -20,cos_table[(int)(k/2)%LED_WIDTH], 14, 14);
    displayPicNewInv(fraiseCalc, -k%f+ offset + 14 , Y1 + 2, 11, 10);
    displayPicNewInv(ghostred, -k%f + offset + 30, Y1, 14, 14);
    displayPicNewInv(ghostyellow, -k%f + offset +46, cos_table[(int)(2*(k+10)/3)%LED_WIDTH], 14, 14);
    displayPicNewInv(ghostgreen, -k%f + offset + 62, Y1, 14, 14);
    displayPicNewInv(ghostPurple, -k%f + offset +78, Y1, 14, 14);
    displayPicNewInv(ghostCyan, -k%f + offset + 94, Y1, 14, 14);
    displayPicNewInv(mariocalc, -k%f + offset+110 , Y1, 14, 17);
   displaygif(pacmancalc,-k%f+offset+110+30,0,8,8,(k/7) %4);
    displaygif(pacmancalc,-k%f+offset+110+30+50,9,8,8,(k/7) %4);
      displaygif(pacmancalc,-k%f+offset+110+30+100,9,8,8,(k/5) %4);
        displaygif(pacmancalc,-k%f+offset+110+30+150,9,8,8,(k/7) %4);
          displaygif(pacmancalc,-k%f+offset+110+30+200,9,8,8,(k/5) %4);
            afficheMessage(mess, -k%f+ offset + 110+40, 30);
   afficheMessage2(mess, -k%f+ offset + 110+40, 0);
     displaygif(pacmancalc,-k%f+offset+110+30+250,0,8,8,(k/5) %4);
      displaygif(pacmancalc,-k%f+offset+110+30+260,0,8,8,(k/5) %4);
  //displayPicNewInv(mariocalc, -k%f+offset+110+30+260+10 , Y1, 14, 17);
Y1=Y1+14;
// displayPicNewInv(ghostgreen, -k%f + offset + 62+10, Y1, 14, 14);
    displayPicNewInv(ghostPurple, -k%f + offset +78+10, Y1, 14, 14);
    displayPicDouble(ghostCyan, -k%f + offset + 94+10, 19,14,14);//cos_table[k%LED_WIDTH] , 14, 17);
displayPicDouble(luigicalc, -k%f + offset+110+30+260+10+30,13,14,17);//cos_table[(int)(2*(k+10)/3)%LED_WIDTH] , 14, 17);
   displaygif(pacmancalc,-k%f+offset+110+30+10,cos_table[(10+k)%LED_WIDTH],8,8,(k/7) %4);
    displaygif(pacmancalc,-k%f+offset+110+30+50+10,14,8,8,(k/7) %4);
}
replaceled();
}
break;




case 10:
{
           if(firsttime)
 {
 String g="Enter you message:http://192.168.1.57/index.html";

 // artnet.setframe((CRGB*)malloc(48*64*sizeof(CRGB))) ;
  g.toCharArray(mess, g.length()+1);
  setTableBrightness(64);
  firsttime=false;
 }   
        disp=true;
bgColor=CRGB(10,10,10);
int f=100+strlen(mess)*8;
fill(bgColor);
// displayBitmapFromProgmem(mariomap,-k%1500+30,YO+r,40,703);
 // displayBitmapFromProgmem(mariomap ,0,YO,40,792);
for(int l=0 ;l<1;l++)
{
    Y1=0;
     afficheMessage(mess, -k%f+ offset +70, 15);
            afficheMessage2(mess, -k%f+ offset , 30);
   afficheMessage2(mess, -k%f+ offset +20, 0);
       afficheMessage(mess, -k%f+ offset +100, 40);
 
}
replaceled();
}
break;
/*
int pacx=0;
int pacy=0;
int face=0;*/

//fill(CRGB::Black);

//displayPicNew(boardpacman,0,mapy,48,28*4); 
//calculpacboard(mapy);
/*
if(ghostx-pacx <0)
{
  
   if(canMoveRight(ghostx,ghosty) and oldmove !=3)
   {
      ghostx+=tempo(k);
      newmove=1;
      
   }
    else
    {
      if(ghosty-pacy<0)
      {
        if(canMoveUp(ghostx,ghosty) and oldmove !=4)
        {
          ghosty+=tempo(k);
          newmove=2;
        }
        else
        {
          if(canMoveDown(ghostx,ghosty) 
          
          
          
          )
          {
           ghosty-=tempo(k);
           newmove=4;
          }
    
        }
      }
      else
      {
        if(canMoveDown(ghostx,ghosty) and oldmove!=1)
        {
          ghosty-=tempo(k);
          newmove=4;
        }
        else
        {
          if(canMoveUp(ghostx,ghosty))
            {
              ghosty+=tempo(k);
              newmove=1;
            }
        }
      }
    }

  if(newmove==0)
  {
    ghostx-=tempo(k);
    newmove=3;
  }
}
else
{
 Serial.println("a gauche"); 
   if(canMoveLeft(ghostx,ghosty) and oldmove!=1)
   {
      ghostx-=tempo(k);
      newmove=3;
      
   }
    else
    {
      Serial.println("on peut pas gauche"); 
      if(ghosty-pacy<0)
      {
        if(canMoveUp(ghostx,ghosty) and oldmove!=4)
        {
          ghosty+=tempo(k);
          newmove=1;
        }
        else
        {
          Serial.println("on tente descente"); 
          if(canMoveDown(ghostx,ghosty) )
          {
           ghosty-=tempo(k);
           newmove=4;
          }
        }
      }
      else
      {
        Serial.println("on tente descente 2"); 
        if(canMoveDown(ghostx,ghosty) and oldmove!=2)
        {
          ghosty-=tempo(k);
          newmove=4;
        }
        else
        {
          Serial.println("on monte"); 
          if(canMoveUp(ghostx,ghosty) )
            {
              ghosty+=tempo(k);
              newmove=2;
            }
        }
      }
    }
if(newmove==0)
  {
    ghostx+=tempo(k);
    newmove=1;
  }

    
}
oldmove=newmove;
newmove=0;

Serial.printf("pac %d %d g %d %d\n",pacx,pacy,ghostx,ghosty);
*/
//fill(CRGB::Black);
/*
static uint8_t hue = 0;
   for(int i = 0; i < LED_HEIGHT;i++) {
                    for(int j = 0; j < LED_WIDTH; j++) {
                      leds[i*LED_WIDTH + j] = CHSV((32*i) + k,192,255);
                    }
                  }
                  k+=10;*/


/*
if(onbouge==0)
{
    switch (directionp){
      case 1:
      if( canMoveRight(pacx,pacy) )  
      {
        onbouge=1;
            
      }
      else
       onbouge=-1;
      break;
      case 2:
       // f(r<=(-130+LED_HEIGHT/2)-2)
        if( canMoveUp(pacx,pacy) )  
      {
        onbouge=1;
      }
            else
       onbouge=-1;
      break;
      case 3:
        if( canMoveLeft(pacx,pacy) )  
      {
        onbouge=1;
      }
            else
       onbouge=-1;
      break;
      case 4:
      if( canMoveDown(pacx,pacy) )
      {
        onbouge=1;
      }
            else
       onbouge=-1;
      break;
    }
}

if(onbouge>=1)
{
 switch (directionp){
    case 1:
      pacx+=1;
     break;
    case 3:
      pacx-=1;
 break;
    case 2:
        if(pacy>=24 and mapy >  (-31*4+48))
         {
          mapy-=1;
          pacy+=1;
          vpacy=24;
         }
         else
         {
           //if(pacy<48-5){
              pacy+=1;
              vpacy+=1;
            //}   
         }

 break;
    case 4:
        if(vpacy>=24 and vpacy<25 and mapy < 0)
         {
          mapy+=1;
          pacy-=1;
          vpacy=24;
         }
         else
         {
          // if(pacy>0){
              pacy-=1;
              vpacy-=1;
            //}   
         }
     break;
  
 }
 onbouge=(onbouge+1)%5;
 if(onbouge==0)
    onbouge=-1; //on stoppe pacman
}
displayBitmapFromProgmem(pacboardfull,1,mapy,31*4,112); 
displaygifdir(pacmancalc,pacx,vpacy,8,8,(k/7)%4,directionp);
displayPicNew(ghostpc,ghostx,ghosty,8,9);

replaceled();
*/

//fill(CRGB::Black);



case 4:
{
  disp=true;
 if(firsttime)
 {
  setTableBrightness(64);
  firsttime=false;
 }
//delay(200);
bgColor=CRGB(10,10,10);
if(k%600==0)
  ongoing=false;
gameOflife();
replaceled();
}

break;
    
/*
   displayBitmapFromProgmem(mariomap,-k%1500+30,0,40,703);
               displayBitmapFromProgmem(mariomap3,-k%1500+30+703,0,40,792); */
               
    /*            
            if(dire>=0)
             {
              
               // r=r+(k%pas/(pas-1));
                if(r>=0)
                  dire=-1;
             }
              else
              {
               // r=r-(k%pas/(pas-1));
                if(r<-14)
                  dire=1;
              }
              if(k%(pas-1)==0)
              {
                //FastLED.show();
                 FastLEDshowESP32();
               for(int g=0;g<5;g++)
               {
              
                displayBitmapFromProgmem(mariomap,-k%1500+30,YO+r,40,703);
                   displayBitmapFromProgmem(mariomap3,-k%1500+30+703,YO+r,40,792);
                 //  FastLED.show();
                  FastLEDshowESP32();
               r+=dire;
               }
               r-=dire;
              }
              // k++;*/

   
     //mariocalc = calculpic(mario, palm, solidColor, 17,
     //mariocalc = calculpic(mario, palm, solidColor, 17, 14);
  

/*
    static uint8_t hue = 0;
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
    }
  }

  // Set the first n leds on each strip to show which strip it is
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j <= i; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::Red;
    }
  }

  hue+=10;*/
/*
case 5:
{
 

   //setTableBrightness(64);
 //displayBitmapFromProgmem(movie[k%nbframes],0,0,48,123); 
    delay(1000/12);
    replaceled();


}
break;
*/
case 6:
{
    disp=true;
 if(firsttime)
 {
  setTableBrightness(64);
  firsttime=false;
 }
 displayBitmapFromProgmem(paintingsLD[((int)(k/1000))%nbpaintings],0,0,48,123); 
   // delay(4000);
    replaceled();
    // FastLEDshowESP32();
      //displayBitmapFromProgmem(pacboardfull,2,0,36*4,28*4);
      k++;
 /*   if(changecolor)
      {
    changecolor=false;
    replaceled();
      }*/ 
      //for lamp;
}
break;
/*
bgColor=CRGB(0,0,48);
fill(bgColor);
char mess3[]="10 PRINT \"HELLO WOLRD\"\n20 POKE 23,123\n30 GOTO 10\nRUN";
       afficheMessage2(mess3, 1, 40);
*/


/*
//to plug the new board
fill(CRGB::Black);
  
for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 1; j <= i+1; j++) {
      //leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::Red;
      PixelOn(j,i*3,CRGB::Red);
      PixelOn(j,i*3+1,CRGB::Red);
      PixelOn(j,i*3+2,CRGB::Red);
    }
  }
replaceled();
*/

case 7:
{
  //Serial.println("on arrive tertirs");
 disp=false;
//tetris
 if(firsttime)
 {
  setTableBrightness(64);
  firsttime=false;
 }
   if(!inGame)
  { 
    if(justLost)
    {
            
        // timerStop(timer1);
        timerAlarmDisable(timer);
        for(int i=0 ;i<LED_HEIGHT;i++)
        {
          for(int y=0;y<LED_WIDTH;y++)
          {
            leds[i*LED_WIDTH+y]=CRGB::Green;
          }
          FastLEDshowESP32();
          delay(50);
          justLost=false;
        }
    newGame();
 
    }
    else
    {
      newGame();
    }

  }
  else
  {
    if( authdisplay)
    {
      Serial.println("on stop le timer");
            stopfall=true;
      timerStop(timer);

      FastLEDshowESP32();
       authdisplay=false;
       Serial.println("on relance le timer");
      if(!gamePaused)
        stopfall=false;
       timerStart(timer);
      Serial.println("timer relancer");
      //delay(2);
     
      
    }
    else

    {
     delay(1);// Serial.println("displah off");
    }
  }


 
}
break;

case 8:
{
  if(firsttime)
  {
    firsttime=false;
    fill(bgColor);
    afficheMessage("READY TO LAMP", 1, 20);
    replaceled();
    FastLEDshowESP32();
    FastLEDshowESP32();
  }
  disp=true;
}
break;

case 9:
{
  if(firsttime)
  {
    firsttime=false;
    fill(bgColor);
    afficheMessage("READY TO PAINT", 1, 20);
    replaceled();
    FastLEDshowESP32();
    FastLEDshowESP32();
    //delay(2000);
    //fill(CRGB(10,10,10));
  }
  executePaintSocketControl();
  disp=true;
}
break;


}
/*

 */
     
long time3=ESP.getCycleCount();
//FastLED.show();
  /* if( authdisplay)
    {
      timerStop(timer);
      FastLEDshowESP32();
       timerStart(timer);
      //delay(2);
      authdisplay=false;
      
    }*/
    if(disp)
   FastLEDshowESP32();
                                 
                          /*       long time2=ESP.getCycleCount();
                             
long delta=time3-time2;
     if(maxi<(float)240000000/(delta))
     {
       maxi=(float)240000000/(delta);
     //  Serial.printf("new max%f\n",maxi);
     }
      if(mini>(float)240000000/(delta))
            {
       mini=(float)240000000/(delta);
       //Serial.printf("new mini%f\n",mini);
     }
    // Serial.printf("totoal fps: %f\n",(float)240000000/(time2-time1));
 //FastLED.delay(10);
 */
 

//delay(5);

if(inGame)
timerStop(timer); //for tetris
//if(anim!=5)
 server.handleClient();
  // webSocket.loop();
  if(anim==8)
       executelampSocketControl();
     if(inGame)
     {
    timerStart(timer);
   executeTetrisSocketControl();
     }
    k++;
 //tetris
    //Serial.println("loop du main");
}





