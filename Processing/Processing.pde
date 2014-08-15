import processing.serial.*;
import ddf.minim.*;
import ddf.minim.analysis.*;

FFT fft;
Serial myPort;        // The serial port
int xPos = 1;         // horizontal position of the graph 

//Variables to draw a continuous line.
int lastxPos=1;
int lastheight=height/2;

void setup () {
  // set the window size:
  size(600, 400);        

  // List all the available serial ports
  println(Serial.list());
  // Check the listed serial ports in your machine
  // and use the correct index number in Serial.list()[].

  myPort = new Serial(this, Serial.list()[1], 57600);  //
  
  background(0);      // set inital background:
}
void draw () {
  // everything happens in the serialEvent()
}

byte parsingStep = 1;
short rawEEG = 0;

void serialEvent (Serial myPort) {
  char data = myPort.readChar();
  
  switch (parsingStep)
  {
      case 1:
          rawEEG = (short)(data << 8);
          ++parsingStep;
          break;
  
      case 2:
          rawEEG |= (short)data;
          println(rawEEG);
          float eeg = map(rawEEG, -2048*2, 2048*2, 0, height); //map to the screen height.

          //Drawing a line from Last inByte to the new one.
          stroke(127,34,255);     //stroke color
          strokeWeight(1);        //stroke wider
          line(lastxPos, lastheight, xPos, eeg); 
          lastxPos = xPos;
          lastheight = int(eeg);
      
          // at the edge of the window, go back to the beginning:
          if (xPos >= width) {
              xPos = 0;
              lastxPos= 0;
              background(0);  //Clear the screen.
          } 
          else {
              // increment the horizontal position:
              xPos++;
          }
          
          parsingStep = 1;
          break;
  
      default:
          parsingStep = 1;
          break;
  }
}

