typedef struct s_pumps{ // pump ID ranges from 1 - 15
  byte ID; // identification for each pump I suggest to use the same numbered pin on the board for the sake of sanity while troubleshooting
  float workTime; // time the pump is activated for in seconds 
} Pump;

typedef struct s_sensors{
  byte ID; // unique identifier for each sensor
  float threshold; // the threshold at which a plant will be watered
  } Sensor;


const byte  WATERING_SYSTEM_NUMBER = 1; // a constant used for easier pinout initialization
double moistureReading;
int moistureMeas = 0; // could remove this additional variable but I left it for readability

Sensor sensor1 = {A1, 30.5}; // initializing the first sensor
Pump pump1 = {2, 0.8}; // initilization of the first pump and declaration
// unfortunately aditional pumps and sensors would require more code like the one above

Sensor sensors[WATERING_SYSTEM_NUMBER] = {sensor1}; // making a array of sensors
Pump pumps[WATERING_SYSTEM_NUMBER] = {pump1}; // making an array of pumps





void sendData(){
  // TO-DO: code for sending moisture data and pump status via ethernet shield
  return;
  }

long int secondsToMillis(float seconds){ //transforms milliseconds to seconds;
  //Serial.println(seconds*1000); 
  return (long)(1000*seconds);
  }

void delaySeconds(float seconds){
    Serial.println(seconds);
    Serial.println(secondsToMillis(seconds));
    delay(secondsToMillis(seconds)); // creates a delay that lasts a specified time in seconds
    return;
  }

void delayMinuntes(int minutes){
  for(int minDelayed = 0; minDelayed < minutes; minDelayed++){
    delaySeconds(60);
  }
  return;
}

void delayHours(int hours){
  for(int hDelayed = 0; hDelayed < hours; hDelayed++){
    delayMinuntes(60);
  }
  return;
}
void pumpActivate(Pump pump){
  digitalWrite(pump.ID, HIGH);
  Serial.println("Pump on");
  Serial.println(pump.ID);
  sendData();
  }

void pumpDeactivate(Pump pump){
  digitalWrite(pump.ID, LOW);
  Serial.println("Pump off");
  Serial.println(pump.ID);
  sendData();
  }

void measureMoisture(Sensor sensor){ 
  moistureMeas = analogRead (sensor.ID);
  moistureReading = moistureMeas * (100./1024); // moisture scale range 0-100;
   Serial.println(moistureReading); //used for debugging;
  }

void initializePumpsAndPins(){
  for(int i = 0; i < WATERING_SYSTEM_NUMBER; i++){
    pinMode(sensors[i].ID, INPUT);
    pinMode (pumps[i].ID, OUTPUT); 
  }
 return; 
 }

void usePump(Pump pump){
  pumpActivate(pump);
  delaySeconds(pump.workTime); //leaves the pump activated for it's own worktime
  Serial.println("Time passed");
  pumpDeactivate(pump);
  return;
  }

void setup() {
    initializePumpsAndPins();
    Serial.begin(9600); // also used for debugging
}

void loop() {
  for(int i = 0; i < WATERING_SYSTEM_NUMBER; i++){
    measureMoisture(sensors[i]);
    if(moistureReading >= sensors[i].threshold){ // compares the moisture reading to the assigned threshold for that sensor
      usePump(pumps[i]); // sets the pump to work for it's predefined time
      }
  }
  delayMinuntes(5); // arbitrary delay of 5 minutes, will be swapped forr a delay in hours

}
