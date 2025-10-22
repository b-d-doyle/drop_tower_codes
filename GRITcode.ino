//GRIT code
//Wesley Chambers
//20 July 2018
//
//29 September 2025
//Modification by Brandon Doyle:
//  - Falling event now triggers on falling edge of contact_pin_sig, not
//    simply logical LOW. This is to avoid immediately firing the solenoid if
//    someone should accidentally flip the arm switch before mounting GRIT.
//  - 4 minute cooldown replaced with infinite while loop. Is there a reason
//    we were letting GRIT reset itself? Maybe there is and I just haven't
//    thought of it! But if not, let's not let it do that, IMO.


//Macros for detecing signal rising/falling edges. Added by Brandon.
//#define RE(signal, state) (state=(state<<1)|(signal&1)&3)==1 //Rising edge. Unused. Uncomment if needed.
#define FE(signal, state) (state=(state<<1)|(signal&1)&3)==2   //Falling edge.

//Pin assignments

int relay_pin_out = 7;

int armed_pin_sig = 2;

int contact_pin_vlt = 11;
int contact_pin_sig = 10;
int contact_pin_gnd = 9;

int armed_pin_LED = 4;
int contact_pin_LED = 3;

// Brandon moved these out of the loop:
bool ARMED_LOGIC   = false;
bool CONTACT_LOGIC = false;



void setup() {
  // put your setup code here, to run once:
  
  pinMode(armed_pin_sig, INPUT);

  pinMode(relay_pin_out, OUTPUT);

  pinMode(contact_pin_sig, INPUT);
  pinMode(contact_pin_vlt, OUTPUT);
  pinMode(contact_pin_gnd, OUTPUT); 
  
  pinMode(armed_pin_LED, OUTPUT);
  pinMode(contact_pin_LED, OUTPUT);

  //Initialize pin states  
  
  digitalWrite(relay_pin_out, LOW);
  
  digitalWrite(contact_pin_vlt, HIGH);
  digitalWrite(contact_pin_gnd, LOW);

  digitalWrite(armed_pin_LED, LOW);
  digitalWrite(contact_pin_LED, HIGH);

  Serial.begin(9600);
}

void loop() {
    // Read armed and contact logic
  ARMED_LOGIC   = digitalRead(armed_pin_sig);
  CONTACT_LOGIC = digitalRead(contact_pin_sig); //This is still okay even with my edits because we're not in the while loop yet. -Brandon

    // Show Logic by print and LED
  Serial.print("Armed Logic:");
  Serial.print(ARMED_LOGIC);
  Serial.print("\t Contact Logic:");
  Serial.println(CONTACT_LOGIC);

    //Contact Light indication
  if(CONTACT_LOGIC==true){
       digitalWrite(contact_pin_LED, HIGH);    }
  if(CONTACT_LOGIC==false){
       digitalWrite(contact_pin_LED,  LOW);    
       delay(25);
       digitalWrite(contact_pin_LED,  HIGH);
       delay(25);}
  if(ARMED_LOGIC==false){
       digitalWrite(armed_pin_LED,  LOW);    }


  while(ARMED_LOGIC==true){
    
      // Read armed and contact logic
       ARMED_LOGIC   = digitalRead(armed_pin_sig);
       //CONTACT_LOGIC = digitalRead(contact_pin_sig);  //Now handled in Falling Edge detection -Brandon

       digitalWrite(armed_pin_LED,  HIGH);
       
       // Falling event: triggers on falling edge (hehe) of signal from contact_pin_sig
       if( FE(digitalRead(contact_pin_sig),CONTACT_LOGIC) ){ // FE() stores current state and checks for falling edge
              //Activating Launcher
            delay(50);
            digitalWrite(relay_pin_out,   HIGH); 
            Serial.print("### Firing Solenoid ### \n");
            //Pulse length
            delay(150);
            digitalWrite(relay_pin_out, LOW);

            //delay(240000);   // 4 min cooldown (enough time to turn off the arm or power switch)
            while(true);       // infinity min cooldown. Just Brandon's suggestion, take or leave :)
    }
  } 
  
}
