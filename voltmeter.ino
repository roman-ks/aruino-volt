// include TFT and SPI libraries
#include <TFT.h>  
#include <SPI.h>
#include <stdlib.h>

// pin definition for Arduino UNO
#define cs   53
#define dc   22
#define rst  8

#define EPSILON   0.01
#define CL_HEIGHT 20
#define CL_WIDTH  80
#define COLUMNS 2

int analogInput = 0;
int inputs[] = {87,88,89,90,97,92,93,94,98,96};
int rs[] = {1,2,3,4,5,1,2,3,4,5};
bool refresh[10];
char strs[10][10];
char sumStrs[2][10];


// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);
int val_size = 10;
float vals[10];

void setup() {
  Serial.begin(9600);
  Serial.print("setup");
  Serial.println();

  for(int i=0;i<val_size;i++){
     pinMode(inputs[i], INPUT);
  }
 
  //initialize the library
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);
  //set the text size
  TFTscreen.setTextSize(2);
}


void loop() {
  Serial.print("loop");
  Serial.println();
  // first and second col
  for(int i=0;i<10;i++){
    int value = analogRead(inputs[i]);
    float vout = (value * 5.0) / 1024.0 * rs[i];

    Serial.print("Got value :");
    Serial.print(vout);
    Serial.print("On pin ");
    Serial.print(inputs[i]);
    Serial.println();

    // sum will be in the last cell, save into separate array
    if(i%5 == 4){
      dtostrf(vout, 2, 2, sumStrs[i/5]);
    }

    int colStart = i/5*5;
    for(int j = colStart; j<i; j++){
      Serial.print(" -");
      Serial.print(vals[j]);
      vout = vout - vals[j];
    }
    Serial.println();
    setNewValue(i, vout);
    
    dtostrf(vout, 2, 2, strs[i]);
  }

  Serial.print("First half: ");
  printArr(vals, 5);
  Serial.println();

  Serial.print("Second half: ");
  printArr(vals+5, 5);
  Serial.println();

  for(int i=0;i<10;i++){
    if(!refresh[i]) continue;
    int col = i/5;
    
    if(vals[i]<=3.85){
      TFTscreen.stroke(10, 10, 255);
    }else if(vals[i]>3.85 && vals[i]<=4.0){
      TFTscreen.stroke(10, 255, 255);
    }else {
      TFTscreen.stroke(10, 255, 10);
    }

    // clear rect
    TFTscreen.fill(0,0,0);
    TFTscreen.rect(col*CL_WIDTH, CL_HEIGHT*(i-5*col), CL_WIDTH, CL_HEIGHT);
    TFTscreen.text(strs[i], 6+col*CL_WIDTH, CL_HEIGHT*(i-5*col)+5);
  }
  
  for (int i = 0; i < COLUMNS; i++){
    TFTscreen.stroke(255, 255, 255);
    TFTscreen.rect(i*CL_WIDTH, CL_HEIGHT*5, CL_WIDTH, CL_HEIGHT);
    TFTscreen.text(sumStrs[i], 6+i*CL_WIDTH, CL_HEIGHT*5+5);
  }
  
  delay(500);
  
}

void setNewValue(int i, float val){
  if(fabs(vals[i]-val)>=EPSILON){
      vals[i]=val;
      refresh[i]=true;
    }else{
      refresh[i]=false;
    }
}

void printArr(float* arr, int size){
  Serial.print("[");
  for (int i = 0; i < size-1; i++){
    Serial.print(arr[i]);
    Serial.print(", ");
  }
  Serial.print(arr[size-1]);
  Serial.print("]");
}
