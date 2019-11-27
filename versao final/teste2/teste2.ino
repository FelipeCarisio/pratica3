  #include <Adafruit_GFX.h>
  #include <DFRobot_sim808.h>
  #include <sim808.h>
  #include <MCUFRIEND_kbv.h>
  #include <TouchScreen.h>
  #include <SoftwareSerial.h>
   
  #define TAMANHOMAXTELL 14
  #define MINPRESSURE 100
  #define MAXPRESSURE 2000
  #define AZUL     0x001F
  #define CIANO     0x07FF
  #define PRETO   0x0000
  #define VERMELHO     0xF800
  #define VERDE   0x07E0
  #define BRANCO   0xFFFF
   
  #define XP 6
  #define XM A2
  #define YP A1
  #define YM 7
   
  #define TS_LEFT 203
  #define TS_RT 892
  #define TS_TOP 203
  #define TS_BOT 916
  #define MESSAGE_LENGTH 160
  #define PIN_TX     50
  #define PIN_RX     16
  
  char msg[MESSAGE_LENGTH];
  int messageIndex = 0;
  char gprsBuffer[64];
  
  char msgEnviar[300];
  char numeroDiscado[16];
  char numeroRecebeu[16];
  
  char data[24];
  Adafruit_GFX_Button botoesSMS[5];
  
  Adafruit_GFX_Button btnDiscar;
  Adafruit_GFX_Button btnSMSs;
  
  SoftwareSerial mySerial(PIN_TX,PIN_RX);
  DFRobot_SIM808 sim808(&mySerial);//Conecta RX,TX,PWR
  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
  Adafruit_GFX_Button botoes[14];
  MCUFRIEND_kbv tft;
   
  bool pressionado = false;
  bool volta = false;
  int posX;
  int posY;
  String numero = "";
   
   
  bool obterToque(void){
      TSPoint p = ts.getPoint();
      pinMode(YP, OUTPUT);      
      pinMode(XM, OUTPUT);
      digitalWrite(YP, HIGH);   
      digitalWrite(XM, HIGH);
      bool pressed = (p.z > MINPRESSURE);
      if (pressed) 
      {
          posX = map(p.x, TS_LEFT, TS_RT, 0, 240); 
          posY = map(p.y, TS_TOP, TS_BOT , 0, 320);
          return true;
      }
      return false; 
  }
   
   
   
  void setup() {
    mySerial.begin(9600);
    Serial.begin(9600);
     while(!sim808.init())
    {
        Serial.print(F("Erro\r\n"));
        delay(1000);
    }
    delay(3000);
        
    Serial.println(F("Iniciado com sucesso, me mande um SMS!"));
    Serial.println();
    uint16_t ID = tft.readID();
    Serial.println(ID);
    tft.begin(ID);
    tft.setRotation(0);
    telaInicial();
  }

  void escreveSMS(int index)
  {
    tft.fillRect(1, 30, 240, 210, BRANCO);
    tft.setCursor(5,35);
    sim808.readSMS(index, msg, MESSAGE_LENGTH, numeroRecebeu, data);
    char dataNormalizada[8];
    for(int i =0; i<8; i++)
    {
      dataNormalizada[i] = data[i];
    }
    
    tft.print(dataNormalizada);
    tft.print("\n");
    tft.print(numeroRecebeu);
    tft.print("\n");
    tft.print(msg);
  }

  void telaSMS()
  {
    tft.fillScreen(PRETO);
    botoesSMS[0].initButton(&tft, 180, 300, 70, 30, BRANCO, CIANO, PRETO, ">", 2);
    botoesSMS[1].initButton(&tft, 60, 300, 70, 30, BRANCO, CIANO, PRETO, "<", 2);
    botoesSMS[2].initButton(&tft, 10, 10, 20,20, BRANCO, VERMELHO, PRETO, "<", 2);
    botoesSMS[3].initButton(&tft, 120, 300,40,40,BRANCO, VERMELHO, PRETO, "X", 2);
    botoesSMS[4].initButton(&tft, 120, 260, 60, 40, BRANCO,CIANO,PRETO, "Resp", 2);
     for (int i=0; i<5; i++){
      botoesSMS[i].drawButton(false);
    }

    tft.fillRect(1, 30, 240, 210, BRANCO);
    tft.setCursor(5,35); 
    tft.setTextColor(PRETO); 
    tft.setTextSize(2); 
    delay(5000);

    escreveSMS(1);
  }

  void checaTelaSMS(bool toque, int *indiceAtual)
  {
    int qtdSMS = 20;
    for (int i=0; i<5; i++){
      botoesSMS[i].press(toque && botoesSMS[i].contains(posX, posY));
    }
    if(botoesSMS[2].justPressed())
    {
        botoesSMS[2].drawButton(true);
        esperaSoltar();
        botoesSMS[2].drawButton();
      volta = true;
    }
    if(botoesSMS[0].justPressed())
    {
      botoesSMS[0].drawButton(true);
      esperaSoltar();
      botoesSMS[0].drawButton();
      if(*indiceAtual < qtdSMS)
      {
        *indiceAtual = *indiceAtual + 1;
        escreveSMS(*indiceAtual);
      }
    }
     if(botoesSMS[1].justPressed())
    {
      botoesSMS[1].drawButton(true);
        esperaSoltar();
        botoesSMS[1].drawButton();
      if(*indiceAtual > 0)
      {
        *indiceAtual = *indiceAtual - 1;
        escreveSMS(*indiceAtual);
      }
    }
     if(botoesSMS[3].justPressed())
    {
      botoesSMS[3].drawButton(true);
        esperaSoltar();
        botoesSMS[3].drawButton();
      sim808.deleteSMS(*indiceAtual);
     
      if(qtdSMS == 1)
      {
        volta = true;
      } else
       if(*indiceAtual == qtdSMS)
      {
        escreveSMS(*indiceAtual);
        *indiceAtual = *indiceAtual - 1;
      }
      
    }
    if(botoesSMS[4].justPressed())
    {
        botoesSMS[4].drawButton(true);
        esperaSoltar();
        botoesSMS[4].drawButton();
        respondSMS(numeroRecebeu);
    }
  }
 /* void telaLigacao(char *num)
  {
    btnDiscar.initButton(&tft, 120, 100, 100, 80, BRANCO, CIANO, PRETO, "Discar", 2);
    btnSMSs.initButton(&tft,120,200, 100, 80,BRANCO, CIANO, PRETO, "Ver SMSs",2);
  } */
  }
  void telaInicial()
  {
    tft.fillScreen(PRETO);
    btnDiscar.initButton(&tft, 120, 100, 100, 80, BRANCO, CIANO, PRETO, "Discar", 2);
    btnSMSs.initButton(&tft,120,200, 100, 80,BRANCO, CIANO, PRETO, "Ver SMSs",2);
    btnSMSs.drawButton(false);
    btnDiscar.drawButton(false);
  }

  void checaTelaInicial(bool toque)
  {
    btnSMSs.press(toque && btnSMSs.contains(posX, posY));
    btnDiscar.press(toque && btnDiscar.contains(posX, posY));
    if (btnSMSs.justPressed())
    {
        btnSMSs.drawButton(true);
        esperaSoltar();
        btnSMSs.drawButton();

        
    
       telaSMS();
       int i = 1;
       int *indice = &i;
       while(!volta)
       {
        bool toque = obterToque();
        checaTelaSMS(toque, indice);
        aceitaLigacao();
       }
       volta = false;
       telaInicial();
       
     
   }
    if(btnDiscar.justPressed())
    {
        btnDiscar.drawButton(true);
        esperaSoltar();
        btnDiscar.drawButton();
        telaDiscagem();
        while(!volta)
        {
          bool toque = obterToque();
          tecladoPressionado(toque);
          aceitaLigacao();
        }
        volta = false;
        telaInicial();
    }
  }

   
  
  void telaDiscagem()
  {
    numero = "";
    tft.fillScreen(PRETO);
    botoes[0].initButton(&tft,  60, 100, 30, 40, BRANCO, CIANO, PRETO, "1", 2);
    botoes[1].initButton(&tft, 120, 100, 30, 40, BRANCO, CIANO, PRETO, "2",2);
    botoes[2].initButton(&tft, 180, 100, 30, 40, BRANCO, CIANO, PRETO, "3", 2);
    botoes[3].initButton(&tft, 60, 150, 30, 40, BRANCO, CIANO, PRETO, "4", 2);
    botoes[4].initButton(&tft, 120, 150, 30, 40, BRANCO, CIANO, PRETO, "5", 2);
    botoes[5].initButton(&tft, 180,  150, 30, 40, BRANCO, CIANO, PRETO, "6", 2);
    botoes[6].initButton(&tft, 60, 200, 30, 40, BRANCO, CIANO, PRETO, "7",2);
    botoes[7].initButton(&tft, 120, 200, 30, 40, BRANCO, CIANO, PRETO, "8",2);
    botoes[8].initButton(&tft, 180, 200, 30, 40, BRANCO, CIANO, PRETO, "9", 2);
    botoes[9].initButton(&tft, 60, 250, 30, 40, BRANCO, CIANO, PRETO, "0", 2);
    botoes[10].initButton(&tft, 150, 250, 90, 40, BRANCO, CIANO, PRETO, "Dell", 2);
    botoes[11].initButton(&tft, 80, 300, 70, 40, BRANCO, CIANO, PRETO, "Ligar", 2);
    botoes[12].initButton(&tft, 160, 300, 70, 40, BRANCO, CIANO, PRETO, "SMS", 2);
    botoes[13].initButton(&tft, 10, 10, 20,20, BRANCO, VERMELHO, PRETO, "<", 2);
   
    for (int i=0; i<14; i++){
      botoes[i].drawButton(false);
    }
    visor();
    
  }
 
  void tecladoPressionado(bool teclaTocada){
    for (int i=0; i<14; i++){
      botoes[i].press(teclaTocada && botoes[i].contains(posX,posY));
    }
    checkPressedButton();
   
  }
   
  void checkPressedButton(){
    for (int i=0; i<14; i++){
      if (botoes[i].justPressed()){
        checkPressed(i);
      }
    }
  }
 
  void esperaSoltar(){
    while (obterToque()){
      delayMicroseconds(50);
    }
   
  }
 
  void visor(){
    tft.fillRect(1, 30, 240, 30, BRANCO);
    tft.setCursor(5,35); 
    tft.setTextColor(PRETO); 
    tft.setTextSize(3);
    tft.print(numero+"_"); 
  }
   
   
  void apagarUltimocaractere(){
    if (numero.length()>0){
      numero.remove (numero.length () - 1);
    }
  }
  void checkPressed(int button){
      botoes[button].drawButton(true);
      esperaSoltar();
      botoes[button].drawButton();
      if(numero.length()<TAMANHOMAXTELL || button == 10 || button == 11){
      switch (button){
          case 0:
            numero = numero + "1";
          break;
          case 1:
            numero = numero + "2";
          break;
          case 2:
            numero = numero + "3";
          break;
          case 3:
            numero = numero + "4";        
          break;
          case 4:
            numero = numero + "5";
          break; 
          case 5:
            numero = numero + "6";
          break;
          case 6:
            numero = numero + "7";
          break;
          case 7:
            numero = numero + "8";
          break;
          case 8:
            numero = numero + "9";
          break;
          case 9:
            numero = numero + "0";
          break;
          case 10:
            apagarUltimocaractere();
          break; 
          case 11:  
            ligar(numero);
          break;
          case 12:
            sendSMS(numero);
          break;
          case 13:
            volta = true;
          break;
      }
      visor();
      }
  }

 void aceitaLigacao()
 {
  if(sim808.readable()){
      sim808_read_buffer(gprsBuffer,32,DEFAULT_TIMEOUT);

   //************** Detect the current state of the telephone or SMS ************************
      if(NULL != strstr(gprsBuffer,"RING")) {
          sim808.answer();
      }
     sim808_clean_buffer(gprsBuffer,32);  
  }
 }
 
 void ligar(String num)
 {
    String comp = "+55" + num + "0";
    comp.toCharArray(numeroDiscado, 15);
    sim808.callUp(numeroDiscado);
    Serial.print(numeroDiscado);
    Serial.print(F("ligando"));
 }

 void sendSMS(String num)
 {
  char msgEnviar[300];
  String msgLida;
  num.toCharArray(numeroDiscado, 14);
  Serial.println(F("Mandando SMS."));
  Serial.println(F("Digite a mensagem a ser enviada."));
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      msgLida += c; //makes the string readString
    }
  }
  msgLida.toCharArray(msgEnviar, 300);
  Serial.println(msgEnviar);
  Serial.println(numero);
  
  sim808.sendSMS(numeroDiscado, msgEnviar); 
 }

 void respondSMS(char *num)
 {
  char msgEnviar[300];
  String msgLida;
  Serial.println(F("Mandando SMS."));
  Serial.println(F("Digite a mensagem a ser enviada."));
  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      msgLida += c; //makes the string readString
    }
  }
  msgLida.toCharArray(msgEnviar, 300);
  Serial.println(msgEnviar);
  Serial.println(num);
  
  sim808.sendSMS(num, msgEnviar); 
 }


 

void loop() {
  bool toque = obterToque();
  checaTelaInicial(toque);
  aceitaLigacao();
}
