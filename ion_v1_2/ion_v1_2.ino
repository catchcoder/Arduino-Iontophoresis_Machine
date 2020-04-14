// Iontophoresis Electro Antiperspirant v1.1
// Code and Ciruit by Chris Hawkins 2014
// 
#define  ledPWR 13
#define button  4 
#define phasePolA 8
#define phasePolB 9
#define  pwrForward 5
#define  pwrBackward 6 // reverse polarity
int phaseNumber = 0 ;
unsigned long lastpressed = 0;
//volatile int state = LOW;  
//unsigned long lastpressed1 = 0;
//int buttonstate = LOW;
//int startedPower = LOW;

unsigned long ulngTimer = 0;

void setup(){
   Serial.begin(9600); 
  attachInterrupt(0, buttonPressed,CHANGE);
  
  // LEDS
  pinMode (pwrForward,OUTPUT);
  pinMode (pwrBackward, OUTPUT);
  pinMode (ledPWR,OUTPUT);
   digitalWrite (ledPWR, HIGH);
  
  //Switch polarity
  pinMode (phasePolA,OUTPUT);
  pinMode (phasePolB, OUTPUT);
  
  //Switches
  //pinMode (button, INPUT);
  
  //Reset outputs to low
  alloutputslow ();
 
 lastpressed = millis();
}

void loop(){

//buttonstate = digitalRead (button)
   
   
   if (phaseNumber == 1 && ((millis() - ulngTimer )>5000 ) ) {
     //Serial.print ("P1 check\n");
     //if ((millis() - ulngTimer )>10000 ) {
       //Serial.print ("P2 go\n");
       phaseTwo ();
       
       //Serial.print ("P2\n");
   //  }
   }
   if (phaseNumber == 2 && ((millis() - ulngTimer )>5000 )) {
     //Serial.print ("P2 check\n");
    // if ((millis() - ulngTimer )>10000 ) {
       //Serial.print ("P END\n");
       alloutputslow ();
       phaseNumber =0;
       flashLEDsEnd();
         flashLEDsStart ();
  flashLEDsEnd ();
  
       Serial.print ("PEnd\n");
     //    }
       }
     
} //end loop


void phaseOne (){
  alloutputslow();
  flashLEDsStart ();
  digitalWrite (phasePolA, HIGH);
  digitalWrite (pwrForward, HIGH);
  phaseNumber =1;
  ulngTimer = millis();
  //Serial.print ("P1 Started\n");
  
} // PahseOne

void phaseTwo (){
  
  alloutputslow();
  
  digitalWrite (phasePolA, HIGH);
  digitalWrite (phasePolB, HIGH);
  delay(1000); // pause before moving on
   digitalWrite (pwrBackward,HIGH);
   phaseNumber = 2;
   ulngTimer = millis();
} // PahseOne



void alloutputslow(){
  digitalWrite (phasePolA, LOW);
  digitalWrite (phasePolB, LOW);
   digitalWrite (pwrForward,LOW);
  digitalWrite (pwrBackward,LOW);
}

void flashLEDsStart (){
  for (int x = 0;x <10; x++){
  digitalWrite (phasePolA, HIGH);
  digitalWrite (phasePolB, HIGH);
  delay(500);
  digitalWrite (phasePolA, LOW);
  digitalWrite (phasePolB, LOW);
  delay(500);
  }
 
}

void flashLEDsEnd (){
  for (int x = 0;x <10; x++){
  digitalWrite (phasePolA, HIGH);
  digitalWrite (phasePolB, HIGH);
  delay(500);
  digitalWrite (phasePolA, LOW);
  digitalWrite (phasePolB, LOW);
  delay(500);
  }
}

void buttonPressed (){
Serial.print ("Pressed\n");
if ( millis()- lastpressed >1000) {
  Serial.print ("check button\n");
  

  
  if (phaseNumber ==0) {
     //Serial.print ("P1\n");
     phaseOne ();
     
   }   else 
   {
       if (phaseNumber ==1 || phaseNumber ==2) { // Stop phases
  alloutputslow ();
       ulngTimer = 0;
       phaseNumber =0;
  }
   }
  
    
  lastpressed = millis();
  }

}
