#define enable 4 //A1IN
#define phase 5 //A2IN

String command;
const int OPEN = 0;
const int CLOSED = 1;
const float A = 0.4200; //inch^2
const int RELAY_PIN0 = 2;
const int RELAY_PIN1 = 3;
const float  OffSet = 0.2195 ;
float V, P, F;
unsigned long elapsed_Time;
int valve_Flag = 0;
void setup() {
  pinMode(enable, OUTPUT);
  pinMode(phase, OUTPUT);
  open_Valve();
  digitalWrite(enable, LOW);
  digitalWrite(phase, LOW);

  // Print to console water pressure
  Serial.begin(9600);        // open serial port, set the baud rate to 9600 bps
  Serial.println("/** Water Pressure Log **/");

  pinMode(RELAY_PIN0, OUTPUT);
  pinMode(RELAY_PIN1, OUTPUT);
}

void read_Pressure(){
  //Connect sensor to Analog 0
  V = analogRead(0) * 5.00 / 1024;     //Sensor output voltage
  P = (V - OffSet) * 400/6.895;             //Calculate water pressure, coverted to psi
  F = A * P;

  elapsed_Time = millis();
  Serial.print(elapsed_Time/1000.00);
  Serial.print(", ");
  Serial.print(P, 1);   //psi pressure
  Serial.print(", ");
  Serial.print(F, 3);   //lbf force
  if ((command.equals("v_c") || (command.equals("2")))){
    Serial.print(", ");
    Serial.print("valve_closed");
  }
  Serial.print('\n');
  delay(30);
}

void pump0_TurnON(){
  digitalWrite(RELAY_PIN0, HIGH); // turn on pump 5 seconds
}
void pump0_TurnOFF(){
  digitalWrite(RELAY_PIN0, LOW);  // turn off pump 5 seconds
}

void pump1_TurnON(){
  digitalWrite(RELAY_PIN1, HIGH); // turn on pump 5 seconds
}
void pump1_TurnOFF(){
  digitalWrite(RELAY_PIN1, LOW);  // turn off pump 5 seconds
}

void open_Valve(){
  // enable HIGH, phase LOW, positive pulse to allows water to flow

    digitalWrite(enable, HIGH); // 'turn on' motor driver
    digitalWrite(phase, LOW); // set direction
    delay(30);
    digitalWrite(enable, LOW);
}

void close_Valve(){
  // enable HIGH, phase HIGH, negative pulse stops watter from flowing

    digitalWrite(enable, HIGH); //'turn on' motor driver 
    digitalWrite(phase, HIGH); // set direction
    delay(30);
    digitalWrite(enable, LOW);
}

void read_Commands(){
  if (Serial.available()){
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equals("v_o") || (command.equals("1"))){
      open_Valve();
    }
    else if ((command.equals("v_c")) || (command.equals("2"))){
      close_Valve();
    }
    if (command.equals("p0_on")){
      pump0_TurnON();
    }
    else if (command.equals("p0_off")){
      pump0_TurnOFF();
    }
    else if (command.equals("p1_on")){
      pump1_TurnON();
    }
    else if (command.equals("p1_off")){
      pump1_TurnOFF();
    }
    else if (command.equals("all_on") || (command.equals("3"))){
      pump0_TurnON();
      pump1_TurnON();
    }
    else if (command.equals("all_off") || (command.equals("."))){
      pump0_TurnOFF();
      pump1_TurnOFF();
    }
    else if (command.equals("pv_off") || (command.equals("/"))){
      pump0_TurnOFF();
      pump1_TurnOFF();
      close_Valve();
    }
  }
}


void loop() {
  read_Pressure();
  read_Commands();
}
