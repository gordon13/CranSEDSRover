#include <SPI.h>

char buf [100];
//volatile byte command = 0;
volatile byte pos;
volatile bool process_it;
char motor_index[20];
char motor_speed[20];
int i;

void setup (void)
{
  Serial.begin (115200);   // debugging

  // turn on SPI in slave mode
  SPCR |= bit (SPE);

  // have to send on master in, *slave out*
  pinMode (MISO, OUTPUT);
//  pinMode (motor, OUTPUT);

  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


ISR (SPI_STC_vect)
  {
  byte c = SPDR;
  if (pos < sizeof buf){
    buf [pos++] = c;
  }
  if (c=='\n'){
    process_it = true;
  }

} 



void loop (void)
{
  if (process_it)
    {
    if (buf[0]=='0'){
      // do operation
      Serial.println ("heartbeat");       
    }
    else if(buf[0]=='1'){
      // do operation
      Serial.println ("telemetry");
    }
    else if(buf[0]=='2'){
      // do operation
      Serial.println ("emergency stop");
    }
    else if(buf[0]=='3'){
      // do operation
      motor_index[0] = buf[1];

      for(i=2;i<5;i++){
        motor_speed[i-2] = buf[i] ;
      }
        
      Serial.print ("select motor :");
      Serial.print(motor_index);
      Serial.print(",set speed to:");
      Serial.println (atoi(motor_speed));
    }
    else if(buf[0]=='4'){
      // do operation
      Serial.println ("set motor steering");
    }
    else {
      // do operation
      Serial.println ("no command");
    }
    buf [pos] = 0;
    Serial.println (buf);
    pos = 0;
    process_it = false;
    } 
}  
