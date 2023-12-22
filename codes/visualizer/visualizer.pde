PShape parcel;

float d2r  = 0.01745329251;
float r2d = 57.2957795131;

boolean start_flag = true;
int circleX, circleY;  // Position of circle button
int circleSize = 90;   // Diameter of circle
boolean circleOver = false;

// Serial related
import processing.serial.*;
Serial mySerial;
String myString = null;
int nl = 10;
float myVal;

float axes_scale = 1;

// Drawing related
float psi = 0 * d2r;
float theta = 90 * d2r;
float phi = 0 * d2r;

void setup()
{
  mySerial = new Serial(this, "/dev/ttyACM0", 115200);
  size(600, 600, P3D);
  parcel = loadShape("teapot.obj");
  parcel.scale(7);
  parcel.rotateX(90 * d2r);
  parcel.translate(0, -20,0);
  
  circleX = width/2;
  circleY = height/2;
  ellipseMode(CENTER);
}

void draw()
{
  if(start_flag != true)
  {
    background(0);
     stroke(255);
     ellipse(circleX, circleY, circleSize, circleSize);
     textSize(20);
     text("PLEASE PRESS ENTER!", width/2, height/4);
     textAlign(CENTER, BOTTOM);
  }
  else
  {
    listen();
    update();
  }
}

void drawAxis(float R, float G, float B, float a, float b)
{
  // Front
  beginShape(QUADS);
  fill(R, G, B);
  vertex(0, a, -a);  // g
  vertex(b, a, -a);  // a
  vertex(b, a, a);   // d
  vertex(0, a, a);   // f 
  endShape();
    
  // Back
  beginShape(QUADS);
  fill(R, G, B);
  vertex(0, -a, a);  // e
  vertex(b, -a, a);  // c
  vertex(b, -a, -a); // b
  vertex(0, -a, -a); // h
  endShape();
  
  // Left
  beginShape(QUADS);
  fill(R, G, B);
  vertex(0, a, a);   // f 
  vertex(0, -a, a);  // e
  vertex(0, -a, -a); // h 
  vertex(0, a, -a);  // g
  endShape();
  
  // Right
  beginShape(QUADS);
  fill(R, G, B);
  vertex(b, a, -a);  // a
  vertex(b, -a, -a); // b
  vertex(b, -a, a);  // c
  vertex(b, a, a);   // d
  endShape();
  
  // Top
  beginShape(QUADS);
  fill(R, G, B);
  vertex(0, a, a);   // f 
  vertex(0, -a, a);  // e
  vertex(b, -a, a);  // c
  vertex(b, a, a);   // d
  endShape();
  
  // Bottom
  beginShape(QUADS);
  fill(R, G, B);
  vertex(0, a, -a);  // g
  vertex(b, a, -a);  // a
  vertex(b, -a, -a); // b
  vertex(0, -a, -a); // h
  endShape();
}


void mouseDragged()
{
  float rate = 0.01;
  phi += (pmouseY-mouseY) * rate;
  theta += (mouseX-pmouseX) * rate;
}

void drawInertialFrame(float axis_width, float axis_height)
{
  pushMatrix();
  translate(width*0.2,height*0.9);
  fill(0,255,0);
  box(axis_height,axis_width,0);
  translate(-axis_height/2,-axis_height/2,0);
  rotateZ(PI/2);
  fill(0,0,255);
  box(axis_height,axis_width,0);
  translate(axis_height/2.0,0,0);
  fill(255,0,0);
  box(axis_width*2,axis_width*2,0);
  popMatrix();
  
  // Labels
  textSize(25);
  fill(255,0,0); 
  text("x", width*0.19 - 28, height*0.945);
  fill(0,255,0); 
  text("y", width*0.19, height*0.945);
  fill(0,0,255); 
  text("z", width*0.19 + 28, height*0.945); 
}

void drawCoordinates(float axes_width, float axes_height)
{ 
  pushMatrix();
  rotateX(PI);
  drawAxis(0, 255, 0, axes_width, axes_height);
  rotateY(PI/2);
  drawAxis(255, 0, 0, axes_width, axes_height);
  rotateZ(PI/2);
  drawAxis(0, 0, 255, axes_width, axes_height);
  popMatrix();
}

void update()
{
  lights();
  camera();
  noStroke();
  background(0);
  
  float rotx = -psi;
  float roty = -theta;
  float rotz = phi;

  pushMatrix();
  translate(width/2.0, height/2.0);
  scale(axes_scale);
  rotateZ(rotz); // z
  rotateY(rotx); // y
  rotateX(roty); // x
  drawCoordinates(2, 150);
  shape(parcel);
  popMatrix();
  drawInertialFrame(7, 100);
}

void listen()
{
   while(mySerial.available() > 0)
  {
    myString = mySerial.readStringUntil(nl);
    if(myString != null)
    {    
      println(myString);
    
         String parts[] = split(myString, ",");
         if(parts.length == 3)
         {
           psi = float(parts[0]) * d2r*0;
           theta = float(parts[1]) * d2r;
           phi = float(parts[2]) * d2r;
         }
       }
  }
}

void mousePressed()
{
  if (overCircle(circleX, circleY, circleSize))
  {
    start_flag = true;
  }
}

boolean overCircle(int x, int y, int diameter)
{
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 )
  {
    return true;
  }
  else
  {
    return false;
  }
}
