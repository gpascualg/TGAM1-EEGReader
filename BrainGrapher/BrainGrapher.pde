// Main controller / model file for the the Processing Brain Grapher.

// See README.markdown for more info.
// See http://frontiernerds.com/brain-hack for a tutorial on getting started with the Arduino Brain Library and this Processing Brain Grapher.

// Latest source code is on https://github.com/kitschpatrol/Processing-Brain-Grapher
// Created by Eric Mika in Fall 2010, updates Spring 2012 and again in early 2014.
import ddf.minim.*;
import ddf.minim.analysis.*;
import processing.serial.*;
import controlP5.*;

ControlP5 controlP5;

Serial serial;

Channel rawEEGChannel = null;
Channel[] channels = new Channel[11];
Monitor[] monitors = new Monitor[10];
Graph graph;
ConnectionLight connectionLight;

int packetCount = 0;
int globalMax = 0;
String scaleMode;

final int timeRate = 512;
final int sampleRate = 512;
FFT fft = null;
HammingWindow hamming = null;

byte parsingStep = 1;
short rawEEG = 0;
float[] rawEEGValues = new float[timeRate];
int rawEEGCount = 0;

void setup() {
  // Set up window
  size(1024, 700);
  frameRate(60);
  smooth();
  frame.setTitle("Processing Brain Grapher");  

  // Set up serial connection
  println("Find your Arduino in the list below, note its [index]:\n");
  
  for (int i = 0; i < Serial.list().length; i++) {
    println("[" + i + "] " + Serial.list()[i]);
  }
  
  // Put the index found above here:
  serial = new Serial(this, Serial.list()[1], 57600);

  // Set up the ControlP5 knobs and dials
  controlP5 = new ControlP5(this);
  controlP5.setColorLabel(color(0));    
  controlP5.setColorBackground(color(0));
  controlP5.disableShortcuts(); 
  controlP5.disableMouseWheel();
  controlP5.setMoveable(false);

  // Create the channel objects
  rawEEGChannel = new Channel("RAW", color(0, 255, 0), "");
  channels[0] = new Channel("Signal Quality", color(0), "");
  channels[1] = new Channel("Attention", color(100), "");
  channels[2] = new Channel("Meditation", color(50), "");
  channels[3] = new Channel("Delta", color(219, 211, 42), "Dreamless Sleep");
  channels[4] = new Channel("Theta", color(245, 80, 71), "Drowsy");
  channels[5] = new Channel("Low Alpha", color(237, 0, 119), "Relaxed");
  channels[6] = new Channel("High Alpha", color(212, 0, 149), "Relaxed");
  channels[7] = new Channel("Low Beta", color(158, 18, 188), "Alert");
  channels[8] = new Channel("High Beta", color(116, 23, 190), "Alert");
  channels[9] = new Channel("Low Gamma", color(39, 25, 159), "Multi-sensory processing");
  channels[10] = new Channel("High Gamma", color(23, 26, 153), "???");

  // Manual override for a couple of limits.
  rawEEGChannel.minValue = -2048;
  rawEEGChannel.maxValue = 2048;
  channels[0].minValue = 0;
  channels[0].maxValue = 200;
  channels[1].minValue = 0;
  channels[1].maxValue = 100;
  channels[2].minValue = 0;
  channels[2].maxValue = 100;
  rawEEGChannel.graphMe = true;
  rawEEGChannel.allowGlobal = false;
  channels[0].allowGlobal = false;
  channels[1].allowGlobal = false;
  channels[2].allowGlobal = false;

  // Set up the monitors, skip the signal quality
  for (int i = 0; i < monitors.length; i++) {
    monitors[i] = new Monitor(channels[i + 1], i * (width / 10), height / 2, width / 10, height / 2);
  }

  monitors[monitors.length - 1].w += width % monitors.length;

  // Set up the graph
  graph = new Graph(0, 0, width, height / 2);

  // Set yup the connection light
  connectionLight = new ConnectionLight(width - 140, 10, 20);
  
  // Construct FFT object
  fft = new FFT(timeRate, sampleRate);
  hamming = new HammingWindow();
  fft.window(hamming);
}

void draw() {
  // Keep track of global maxima
  if (scaleMode == "Global" && (channels.length > 3)) {
    for (int i = 3; i < channels.length; i++) {
      if (channels[i].maxValue > globalMax) globalMax = channels[i].maxValue;
    }
  }

  // Clear the background
  background(255);

  // Update and draw the main graph
  graph.update();
  graph.draw();

  // Update and draw the connection light
  connectionLight.update();
  connectionLight.draw();

  // Update and draw the monitors
  for (int i = 0; i < monitors.length; i++) {
    monitors[i].update();
    monitors[i].draw();
  }
}

void serialEvent(Serial p) {
  // Split incoming packet on commas
  // See https://github.com/kitschpatrol/Arduino-Brain-Library/blob/master/README for information on the CSV packet format

  char data = p.readChar();
  
  switch (parsingStep)
  {
    case 1:
    case 2:
      if (data == 0xFF)
        ++parsingStep;
      else
        parsingStep = 1;
      break;
      
    case 3:
      rawEEG = (short)(data << 8);
      ++parsingStep;
      break;

    case 4:
      rawEEG |= (short)data;
      //println(rawEEG);
      
      if (rawEEG < -2048 || rawEEG > 2048)
      {
        parsingStep = 1;
        return;
      }
      
      rawEEGChannel.addDataPoint(rawEEG);
      //float eeg = map(rawEEG, -2048, 2048, 0, 100); //map to the screen height.
      //channels[1].addDataPoint((int)eeg);
      //channels[2].addDataPoint((int)(eeg/2.));
      
      if (rawEEGCount == timeRate)
      {
        rawEEGCount = 0;
        fft.forward(rawEEGValues);
        
        channels[3].addDataPoint((int)fft.calcAvg(1, 3));
        channels[4].addDataPoint((int)fft.calcAvg(4, 7));
        channels[5].addDataPoint((int)fft.calcAvg(8, 9));  
        channels[6].addDataPoint((int)fft.calcAvg(10, 12));
        channels[7].addDataPoint((int)fft.calcAvg(13, 17));
        channels[8].addDataPoint((int)fft.calcAvg(18, 30));
        channels[9].addDataPoint((int)fft.calcAvg(31, 40));
        channels[10].addDataPoint((int)fft.calcAvg(41, 50));
      }
      
      rawEEGValues[rawEEGCount] = (float)rawEEG;
      rawEEGCount++;
      
      packetCount++;
      parsingStep = 1;
      break;

    default:
      parsingStep = 1;
      break;
  }
  
/*
  String incomingString = p.readString().trim();
  print("Received string over serial: ");
  println(incomingString);  
  
  String[] incomingValues = split(incomingString, ',');

  // Verify that the packet looks legit
  if (incomingValues.length > 1) {
    packetCount++;

    // Wait till the third packet or so to start recording to avoid initialization garbage.
    if (packetCount > 3) {
      
      for (int i = 0; i < incomingValues.length; i++) {
        String stringValue = incomingValues[i].trim();

      int newValue = Integer.parseInt(stringValue);

        // Zero the EEG power values if we don't have a signal.
        // Can be useful to leave them in for development.
        if ((Integer.parseInt(incomingValues[0]) == 200) && (i > 2)) {
          newValue = 0;
        }

        channels[i].addDataPoint(newValue);
      }
    }
  }
 */ 
}


// Utilities

// Extend Processing's built-in map() function to support the Long datatype
long mapLong(long x, long in_min, long in_max, long out_min, long out_max) { 
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Extend Processing's built-in constrain() function to support the Long datatype
long constrainLong(long value, long min_value, long max_value) {
  if (value > max_value) return max_value;
  if (value < min_value) return min_value;
  return value;
}

double sum(float[] a, int l)
{
  double s = 0.0;
  for (int i = 0; i < l; i++)
    s += a[i];
  return s;
}

double mean(float[] a, int l)
{
  return sum(a, l) / l; 
}

double var(float[] a, int l)
{
  double m = mean(a, l);
  double temp = 0;
  for (int i = 0; i < l; i++)
    temp += (m-a[i])*(m-a[i]);
  return temp/l;
}

double std(float[] a, int l)
{
  return Math.sqrt(var(a, l));
}

double kurtosis(float[] a, int l)
{
  // kurtosis = { [n(n+1) / (n -1)(n - 2)(n-3)] sum[(x_i - mean)^4] / std^4 } - [3(n-1)^2 / (n-2)(n-3)]
  double n = l;
  double m = mean(a, l);
  double s = std(a, l);
  
  double ss = 0.0;
  for (int i = 0; i < n; i++)
    ss += Math.pow((a[i] - m) / s, 4);
  
  return (((n * (n + 1)) / ((n - 1)*(n - 2)*(n - 3))) * ss) - ((3 * (n - 1) * (n - 1)) / ((n - 2) * (n - 3)));
}

