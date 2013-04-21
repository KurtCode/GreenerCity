import processing.serial.*;
Serial mySerial;
PrintWriter output;
int count = 0;
void setup() {
   mySerial = new Serial( this, Serial.list()[0], 9600 );

    output = createWriter( "newdata"+count+".txt" );

}
void loop() {
  while(true){   
    count++;
    if (mySerial.available() > 0 ) {
         String value = mySerial.readString();
         if ( value != null ) {
              output.println( value );
         }
             output = createWriter( "newdata"+count+".txt" );
             output.

       }
    }
}
