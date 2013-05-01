#include <stdint.h>
#include <DHT.h>
#include "Wire.h"
#include "EggBus.h"

EggBus eggBus;

#define DHTPIN 17 //analog pin 3
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  Serial.println(F("Air Quality Egg - EPA Serial Build v1.0"));
  Serial.println(F("======================================================================\r\n"));
  Serial.println(F("Timestamp, Sensor Type, Sensor Value, Sensor Units, Sensor Resistance"));
  Serial.println(F("----------------------------------------------------------------------"));  
}

void loop(){
  uint8_t   egg_bus_address;
  float i_scaler = 0.0;
  uint32_t r0 = 0;
  uint32_t measured_value = 0;
  
  eggBus.init();
    
  while((egg_bus_address = eggBus.next())){
    uint8_t numSensors = eggBus.getNumSensors();
    for(uint8_t ii = 0; ii < numSensors; ii++){
      Serial.print(millis(), DEC);
      Serial.print(F(", "));      
      
      Serial.print(eggBus.getSensorType(ii));
      Serial.print(F(", "));
      
      i_scaler = eggBus.getIndependentScaler(ii);      
      measured_value = eggBus.getSensorIndependentVariableMeasure(ii);
      r0 = eggBus.getSensorR0(ii);
      
      //if(isnan(eggBus.getSensorValue(ii))){Serial.print("0.000000");          
//}
      /*else{Serial.print(eggBus.getSensorValue(ii), 8);}*/
      Serial.print(eggBus.getSensorValue(ii), DEC);
      Serial.print(F(", "));    
        //  Serial.print(eggBus.getSensorType(ii));
  
      //Serial.print(F(", "));      

      Serial.print(eggBus.getSensorUnits(ii));            
      Serial.print(F(", "));    
      
      if(measured_value == 0xffffffff){
        Serial.print(F("OPEN CIRCUIT"));
      }
      else{
        Serial.print(measured_value * i_scaler * r0, 8);
      }   
      
      Serial.println();
    }
  }

  Serial.print(millis(), DEC);
  Serial.print(F(", "));      
  
  Serial.print("Humidity");
  Serial.print(F(", "));
  float currHumidity = getHumidity();
  Serial.print(currHumidity, 8);
  Serial.print(F(", "));      

  Serial.println(F("%, n/a"));
  delay(2500);



  Serial.print(millis(), DEC);
  Serial.print(F(", "));      
  
  Serial.print("Temperature");
  Serial.print(F(", "));
  float currentTemperature = getTemperature();
  Serial.print(currentTemperature, 8);
  Serial.print(F(", "));      

  Serial.println(F("degrees C, n/a"));
  delay(2500);

  Serial.println();
}

void printAddress(uint8_t * address){
  for(uint8_t jj = 0; jj < 6; jj++){
    if(address[jj] < 16) Serial.print("0");
    Serial.print(address[jj], HEX);
    if(jj != 5 ) Serial.print(":");
  }
  Serial.println();
}

//--------- DHT22 humidity ---------
float getHumidity(){
  float h = dht.readHumidity();
  if (isnan(h)){
    //failed to get reading from DHT    
    delay(2500);
    h = dht.readHumidity();
    if(isnan(h)){
      return -1; 
    }
  } 
  else return h;
}

//--------- DHT22 temperature ---------
float getTemperature(){
  float t = dht.readTemperature();
  if (isnan(t)){
    //failed to get reading from DHT
    delay(2500);
    t = dht.readTemperature();
    if(isnan(t)){
      return -1; 
    }
  } 
  return t;
}



/*#include <stdint.h>

#include "Wire.h"
#include "EggBus.h"

EggBus eggBus;

void setup(){
  Serial.begin(9600);
}

void loop(){
  uint8_t   egg_bus_address;
  float x_scaler = 0.0;
  float y_scaler = 0.0;
  float i_scaler = 0.0;
  uint32_t measured_value = 0;
  uint32_t r0 = 0;
  
  eggBus.init();
  while((egg_bus_address = eggBus.next())){
    Serial.println("===========================");
    Serial.print("Egg Bus Address: 0x");
    Serial.println(egg_bus_address, HEX);

    Serial.print("Firmware Version: ");
    Serial.println(eggBus.getFirmwareVersion(), DEC);

    Serial.print("  Sensor Address: ");
    printAddress(eggBus.getSensorAddress());             
    
    uint8_t numSensors = eggBus.getNumSensors();
    for(uint8_t ii = 0; ii < numSensors; ii++){
    
      Serial.println("---------------------------");
      Serial.print("  Sensor Index: ");
      Serial.println(ii, DEC);       
      
      Serial.print("    Sensor Type: ");
      Serial.println(eggBus.getSensorType(ii));
     
      Serial.print("   Indep. Scaler: ");
      i_scaler = eggBus.getIndependentScaler(ii);
      Serial.println(i_scaler, 8);
      
      Serial.print("  Table X Scaler: ");
      x_scaler = eggBus.getTableXScaler(ii);
      Serial.println(x_scaler, 8);     
     
      Serial.print("  Table Y Scaler: ");
      y_scaler = eggBus.getTableYScaler(ii);
      Serial.println(y_scaler, 8);         

      Serial.print(" Measured Value: ");
      measured_value = eggBus.getSensorIndependentVariableMeasure(ii);
      Serial.print(measured_value, DEC);        
      Serial.print(" => ");
      Serial.println(measured_value * i_scaler, 8);

      Serial.print("              R0: ");
      r0 = eggBus.getSensorR0(ii);
      Serial.println(r0);
      
      Serial.print("Implied Resistance: ");
      if(measured_value == 0xffffffff){
        Serial.println("OPEN CIRCUIT");
      }
      else{
        Serial.println(measured_value * i_scaler * r0, 8);
      }
      
      uint8_t xval, yval, row = 0;
      while(eggBus.getTableRow(ii, row++, &xval, &yval)){
        Serial.print("     Table Row ");
        Serial.print(row);
        Serial.print(": [");
        Serial.print(xval, DEC);
        Serial.print(", ");        
        Serial.print(yval, DEC);        
        Serial.print("] => [");
        Serial.print(x_scaler * xval, 8);
        Serial.print(", ");
        Serial.print(y_scaler * yval, 8);
        Serial.println("]");
      }
      
      Serial.print("    Sensor Value: ");
      Serial.println(eggBus.getSensorValue(ii), DEC);    
      
      Serial.print("    Sensor Units: ");
      Serial.println(eggBus.getSensorUnits(ii));            
    }
  }
  
  delay(5000);
}

void printAddress(uint8_t * address){
  for(uint8_t jj = 0; jj < 6; jj++){
    if(address[jj] < 16) Serial.print("0");
    Serial.print(address[jj], HEX);
    if(jj != 5 ) Serial.print(":");
  }
  Serial.println();
}*/
