
#include "fix_fft.h"

char im[128];
char re[128];
int input;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  
  //Get input from mic
  for (int i=0; i < 128; i++){
    input = analogRead(0);
    re[i] = input;
    im[i] = 0;
  }

  //Perform FFT
  fix_fft(re,im,7,0);


  //Function to get frequency for max amplitude
  int i,j,largest;
  char str[129];

  str[128] = 0;  
  largest = 0;
  // Find the largest entry which will determine how many lines
  // are needed to print the whole histogram
  for (i=0; i< 128;i++){
    re[i] = sqrt(re[i] * re[i] + im[i] * im[i]);
    if(re[i] > largest){
      largest = re[i];
      j = i;
    }
  }

  //If frequency is above our range, throw it away
  if(j < 64){
    int output = j*40*1.7443877551020408163265306122449;
  
  //Generate tone at frequency
    tone(8,output,500);

//  //Print frequency and generate graph
//  Serial.println(j*40*1.7443877551020408163265306122449);
//  print_fft((char *)"Output");

    }
	
	//Delay to reduce feedback from microphone
       delay(50);
}

// Generate the magnitude of each bin and print it on the serial monitor
void print_fft(char *title)
{
  int i,j,largest;
  char str[129];
  char linfo[6];

  str[128] = 0;  
  largest = 0;
  // Find the largest entry which will determine how many lines
  // are needed to print the whole histogram
  for (i=0; i< 128;i++){
    re[i] = sqrt(re[i] * re[i] + im[i] * im[i]);
    if(re[i] > largest)largest = re[i];
  }
  // print a blank line just in case there's
  // garbage when the Serial monitor starts up
  Serial.println("");
  // and the title
  Serial.println(title);
  Serial.println("");
  // print the histogram starting with the highest amplitude
  // and working our way back down to zero.
  for(j=largest;j >= 0;j--) {
    for(i=0;i<128;i++) {
      // If the magnitude of this bin is at least as large as
      // the current magnitude we print an asterisk
      if(re[i] >= j)str[i] = '*';
      // otherwise print a space
      else str[i] = ' ';
    }
    sprintf(linfo,"%3d ",j);
    Serial.print(linfo);
    Serial.println(str);
  }
  // print the bin numbers along the bottom
  Serial.print("    ");
  for(i=0;i<128;i++) {
    Serial.print(i%10);
  }
  Serial.println("");
  Serial.print("    ");
  for(i=0;i<128;i++) {
    if(i < 10)Serial.print(" ");
    else Serial.print((i/10)%10);
  }
  Serial.println("");
}
