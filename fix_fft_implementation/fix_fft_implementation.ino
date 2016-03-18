
#include <fix_fft.h>
//int mic = A0; //electret

char im[128];
char data[128]; 
char data_avgs[128];

//mix max val to map fft
int valMin = 0;
int valMax = 30;  
//bias to reduce on low and increase on high  
int bias = 0;//+- bias to output

void setup(){
//  pinMode(mic, INPUT);
Serial.begin(9600);
}

void loop(){
  int static i = 0;
  static long tt;
  int val;
  randomSeed(analogRead(0));
 // bias = random(70,150);

  for (i=0; i < 128; i++){  
    val = analogRead(0);
    //data[i] = val / 4 - 128;
  data[i] = val;//quick and dirty data.
    im[i] = 0;
    i++;   
  }//end for

  //this could be done with the fix_fftr function without the im array.
  fix_fft(data,im,7,0);
  /*
   Performs foward or inverse Fourier transform.
   //fix_fft (char fr[], char fi[], int m, int inverse)
   //fr is a real data set,
   //fi is the imaginary data set,
   // m is log2(n) where n is number of data points (log2 (128) = 7)
   //0 is set for forward transform. 1 would be inverse transform. Apparently inverse does not work,
   */
//  // I am only interested in the absolute value of the transformation
//  for (i=0; i< 64;i++){//real val is for the amplitude 
//    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); 
//  }//end for

  //do something with the data values 1..64 and ignore im
  //data avg moved to individual func

  //shin: styles for colour mapping to frequency
  //triChn(bias);
  //triChn(0);//no bias
  //style2
  //grpChnLowPass();
  //style3
//  treshChn();

  print_fft((char *)"Testing");

}//end loop


// Generate the magnitude of each bin and print it on the serial monitor
void print_fft(char *title)
{
  int i,j,largest;
  char str[65];
  char linfo[6];

  str[64] = 0;  
  largest = 0;
  // Find the largest entry which will determine how many lines
  // are needed to print the whole histogram
  for (i=0; i< 64;i++){
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
    if(data[i] > largest)largest = data[i];
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
    for(i=0;i<64;i++) {
      // If the magnitude of this bin is at least as large as
      // the current magnitude we print an asterisk
      if(data[i] >= j)str[i] = '*';
      // otherwise print a space
      else str[i] = ' ';
    }
    sprintf(linfo,"%3d ",j);
    Serial.print(linfo);
    Serial.println(str);
  }
  // print the bin numbers along the bottom
  Serial.print("    ");
  for(i=0;i<64;i++) {
    Serial.print(i%10);
  }
  Serial.println("");
  Serial.print("    ");
  for(i=0;i<64;i++) {
    if(i < 10)Serial.print(" ");
    else Serial.print((i/10)%10);
  }
  Serial.println("");
}
