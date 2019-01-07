int fire_switch = D0;
int remote_sensor = D2;
int homekit_state;

int MAIN_STATUS;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int remote_state = 0;         // current state of the button
int lastRemote_state = 0;     // previous state of the button

void setup() {

pinMode(fire_switch, OUTPUT);
digitalWrite(fire_switch,HIGH);
pinMode(remote_sensor, INPUT_PULLDOWN);

// these functions enable the fire to be toggled by another switch or service
Particle.function("fireplace",fireToggle);
Particle.function("hktogg",hktoggle);

// Variable to allo wpulling the current Z"Switch" Status - is the fire on/off currently
Particle.variable("MAIN_STATUS", MAIN_STATUS);


Particle.subscribe("Button_change", xenon_handler);
}




void loop() {
    
    
    remote_state = digitalRead(remote_sensor);
    Particle.variable("remote_state", remote_state);
    
    
    // compare the buttonState to its previous state
  if (remote_state != lastRemote_state) {
    // if the state has changed, increment the counter
    if (remote_state == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Particle.publish("button Change","on");
      Particle.publish("number of button pushes:", String(buttonPushCounter));
      Particle.variable("PushCounter",String(buttonPushCounter));
      fireToggle("1");
    } else {
      // if the current state is LOW then the button went from on to off:
      Particle.publish("button Change","off");
      fireToggle("0");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  lastRemote_state = remote_state;

    
    
    /*
   if (remote_state == 1)
    {
      // Fire should be ON
      fireToggle("1");
      // Particle.publish("Loop 1", "Toggle ON");
      delay(100);
    }
    else if (remote_state == 0)
    {
      fireToggle("0");
      // Particle.publish("Loop 2", "Toggle OFF");
      delay(100);
    }
    
    // Particle.publish("LOOP", String(remote_state));
    
    */

    delay(200);
    
}


int hktoggle(String command) {

 if (command=="1") {     // Turn ON
        delay(100);
       fireToggle("1");
       Particle.publish("Homekit:", "ON");
        
        return 1;
    }
    
    else if (command=="0") {   // turn off
       fireToggle("0");
       Particle.publish("Homekit:", "OFF");
        return 0;
    }
  
    else if (command=="?"){   // check state
        
        if (MAIN_STATUS==1) {     // Currently On
        Particle.publish("Homekit Status:", "ON");
        return 1;
        }
    
        else if (MAIN_STATUS==0) {   // Currently off
            Particle.publish("Homekit Status:", "OFF");
            return 0;
        }
        
    }  
            
    else {
        return -1;
    }



}



void xenon_handler(const char *event, const char *data)
{
fireToggle(data);
}






int fireToggle(String command) {
    /* Spark.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */



    if (command=="1") {     // Turn ON
        
        digitalWrite(fire_switch,LOW);
        delay(100);
        MAIN_STATUS=1;
        Particle.publish("Firetoggle:", String("Turned On"));
       
        
        return 1;
    }
    
    else if (command=="0") {   // turn off
        digitalWrite(fire_switch,HIGH);
        Particle.publish("Firetoggle:", String("Turned Off"));
        MAIN_STATUS=0;
        return 0;
    }
  
            
    else {
        return -1;
    }
    
    
}

