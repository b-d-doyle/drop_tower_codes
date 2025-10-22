//GRIT code
//Wesley Chambers
//20 July 2018

//Pin assignments

int relay_pin_out = 7;

int armed_pin_sig = 2;

int contact_pin_vlt = 11;
int contact_pin_sig = 10;
int contact_pin_gnd = 9;

int armed_pin_LED = 4;
int contact_pin_LED = 3;



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
   
  bool ARMED_LOGIC   = 0;
  bool CONTACT_LOGIC = 0;

    // Read armed and contact logic
  ARMED_LOGIC   = digitalRead(armed_pin_sig);
  CONTACT_LOGIC = digitalRead(contact_pin_sig);

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
       CONTACT_LOGIC = digitalRead(contact_pin_sig);  

       digitalWrite(armed_pin_LED,  HIGH);
    
         // Falling event (false means its not in contact)
       if(CONTACT_LOGIC==false){
              //Activating Launcher
            delay(50);
            digitalWrite(relay_pin_out,   HIGH); 
            Serial.print("### Firing Solenoid ### \n");
            //Pulse length
            delay(150);
            digitalWrite(relay_pin_out, LOW);

            delay(240000);   // 4 min cooldown (enough time to turn off the arm or power switch)
    }
  } 
  
}
