import processing.serial.*;
import de.bezier.data.sql.*;

Serial myPort;
String sensorReading="";
PFont font;
String[] list;
Double Temperature;
Double Humidity;
Double CO;
Double NO2;
int count = 0;
int id = 1;
float LAT = 57.7077;
float LNG = 11.9718;
int lat = 0;
MySQL msql;


void setup() {
  size(1000,500);
  myPort = new Serial(this, "COM9", 9600);
  myPort.bufferUntil('\n');
  font = createFont(PFont.list()[2],32);
  textFont(font);
   String user     = "test";
    String pass     = "test";    //
    String database = "testing";
    
    msql = new MySQL( this, "46.239.124.29:3306", database, user, pass);

    if ( msql.connect() )
    {
        println("Connection established");
        msql.query( "SELECT COUNT(*) FROM sensor_data_new" );
        msql.next();
        println( "number of rows: " + msql.getInt(1));
    }
    else
    {
       // connection failed !;
    }
}

void draw() {
  //The serialEvent controls the display
}  

void serialEvent (Serial myPort){
 sensorReading = myPort.readStringUntil('\n');
  if(sensorReading != null){
    sensorReading=trim(sensorReading);
     list = split(sensorReading, ',');
  }

  //writeText("Sensor Reading: " + sensorReading);
   if (sensorReading.contains("Temperature")) {
              Temperature = Double.parseDouble(list[2]);
               // writeText("Sensor Reading: " + Temperature);
               println(Temperature);
               count++;
          }
    if (sensorReading.contains("Humidity")) {
              Humidity = Double.parseDouble(list[2]);
             //writeText("Sensor Reading: " + Humidity);
               println(Humidity);
              count++;
          }
     if (sensorReading.contains("CO")) {
           //float ff = Float.parseFloat(list[2]);
              CO = Double.parseDouble(list[2]);
              //if((Double.parseDouble(CO)/1) != Double.parseDouble(CO)){
              //if(CO.equals("nan")){
                //println("Found NAN");
              //}
             // writeText("Sensor Reading: " + CO);
               println(CO);
              count++;
          }
          
       if (sensorReading.contains("NO2")) {
              NO2 = Double.parseDouble(list[2]);
              //writeText("Sensor Reading: " + NO2);
               println(NO2);
            count++;
          }
            if(lat == 1){
          LAT = 57.7077;
          LNG = 11.9718;
          }
          if(lat == 2){
          LAT = 57.708733;
          LNG = 11.938019;
          }
          if(lat == 3){
          LAT = 57.689102;
          LNG = 11.975098;
          }
          if(lat == 4){
          LAT = 57.712768;
          LNG = 11.89167;
          }
          if(lat == 5){
          LAT = 57.680293;
          LNG = 11.954498;
          }
          if(lat == 6){
          LAT = 57.736234;
          LNG = 12.019043;
          }
          if(lat == 7){
          LAT = 57.698644;
          LNG = 11.928406;
          }
          if(lat == 8){
          LAT = 57.713134;
          LNG = 12.033119;
          }
          if(lat == 9){
          LAT = 57.691121;
          LNG = 12.004967;
          }
          if(lat == 10){
          LAT = 57.71075;
          LNG = 12.00943;
          lat = 1;
          }
        
          if(count == 4){
         msql.execute( "INSERT INTO sensor_data_new (LAT, LNG, TEMP, HUMIDITY , CO, NO2) VALUES ("/*+id+","*/+LAT+"," + LNG +","+Temperature+"," + Humidity +","+CO+"," +NO2+")");
           println("Inserted");
           delay(3000);
           id++;
           lat++;
           count = 0;  
        }

}


void writeText(String textToWrite){
  background(255);
  fill(0);
  text(textToWrite, 1, 100);   
}
