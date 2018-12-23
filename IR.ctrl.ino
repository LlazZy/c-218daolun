#include<IRremote.h>
int REC_PIN=11;
int light;
int n=1;
IRrecv irrecv(REC_PIN);
decode_results results;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(10,OUTPUT);
}
void loop()
{
     light = analogRead(A3);   
   Serial.println(light);
   delay(500);
   if(irrecv.decode(&results))
  {
    Serial.println(results.value,HEX);
    switch(results.value)
    {
      case 0xFF906F:analogWrite(10,light/50*light/50);   break;
      case 0xFF629D:digitalWrite(10,n);n=!n;break;
    }
      
    irrecv.resume();
  }
   
        
    
  
}
