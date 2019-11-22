#include <DFRobot_sim808.h>
#include <sim808.h>
#include <SoftwareSerial.h>

#define MESSAGE_LENGTH 160
char msg[MESSAGE_LENGTH];
int messageIndex = 0;
char msgEnviar[300];

char numero[16];
char data[24];

#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Conecta RX,TX,PWR

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);

  while(!sim808.init())
  {
      Serial.print(F("Erro\r\n"));
      delay(1000);
  }
  delay(3000);
      
  Serial.println(F("Iniciado com sucesso, me mande um SMS!"));
}

void loop()
{
   messageIndex = sim808.isSMSunread();

   if (messageIndex > 0)//se tem ao menos um SMS para ler
   { 
      Serial.print(F("Index do SMS: "));
      Serial.println(messageIndex);
      
      sim808.readSMS(messageIndex, msg, MESSAGE_LENGTH, numero, data);
                 
      sim808.deleteSMS(messageIndex);
      Serial.print(F("Número: "));
      Serial.println(numero);  
      Serial.print(F("Data e hora: "));
      Serial.println(data);        
      Serial.print(F("Mensagem: "));
      Serial.println(msg); 

      Serial.print(sim808.GPSdata.year);
      Serial.print(F("/"));
      Serial.print(sim808.GPSdata.month);
      Serial.print(F("/"));
      Serial.print(sim808.GPSdata.day);
      Serial.print(F(" "));
      Serial.print(sim808.GPSdata.hour);
      Serial.print(F(":"));
      Serial.print(sim808.GPSdata.minute);
      Serial.print(F(":"));
      Serial.print(sim808.GPSdata.second);
      Serial.print(F(":"));

      Serial.println();      
  
      Serial.println(F("Sim808 iniciado com sucesso"));
      Serial.println(F("Mandando SMS"));

      sprintf(msgEnviar,"mensagem, você deverá recebê-la em seu celular");
      Serial.println(msgEnviar);
      Serial.println(numero);
     
     sim808.sendSMS(numero, msgEnviar);    
   }
}

void sendSMS()
{
  Serial.println(F("Mandando SMS"));
  
  Serial.println(msgEnviar);
  Serial.println(numero);
  
  sim808.sendSMS(numero, msgEnviar); 
}
