#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
 
#define TAMANHOMAXTELL 14
#define MINPRESSURE 100
#define MAXPRESSURE 2000
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
 
 
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_GFX_Button botoes[12];
MCUFRIEND_kbv tft;
 
bool pressionado = false;
int posX;
int posY;
String senha = "";
 
 
bool obterToque(void){
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE);
    if (pressed) {
        posX = map(p.x, TS_LEFT, TS_RT, 0, 240); 
        posY = map(p.y, TS_TOP, TS_BOT , 0, 320);
        return true;
    }
    return false; 
}
 
 
 
void setup() {
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);
  telaInicial();
}
 
void telaInicial()
{
  senha = "";
  tft.fillScreen(PRETO);
  botoes[0].initButton(&tft,  60, 100, 30, 40, BRANCO, VERDE, PRETO, "1", 2);
  botoes[1].initButton(&tft, 120, 100, 30, 40, BRANCO, VERDE, PRETO, "2", 2);
  botoes[2].initButton(&tft, 180, 100, 30, 40, BRANCO, VERDE, PRETO, "3", 2);
  botoes[3].initButton(&tft, 60, 150, 30, 40, BRANCO, VERDE, PRETO, "4", 2);
  botoes[4].initButton(&tft, 120, 150, 30, 40, BRANCO, VERDE, PRETO, "5", 2);
  botoes[5].initButton(&tft, 180,  150, 30, 40, BRANCO, VERDE, PRETO, "6", 2);
  botoes[6].initButton(&tft, 60, 200, 30, 40, BRANCO, VERDE, PRETO, "7",2);
  botoes[7].initButton(&tft, 120, 200, 30, 40, BRANCO, VERDE, PRETO, "8",2);
  botoes[8].initButton(&tft, 180, 200, 30, 40, BRANCO, VERDE, PRETO, "9", 2);
  botoes[9].initButton(&tft, 120, 250, 30, 40, BRANCO, VERDE, PRETO, "0", 2);
  botoes[10].initButton(&tft, 60, 250, 60, 40, BRANCO, VERDE, PRETO, "Ligar", 2);
  botoes[11].initButton(&tft, 180, 250, 60, 40, BRANCO, VERDE, PRETO, "Dell", 2);
 
  for (int i=0; i<12; i++){
    botoes[i].drawButton(false);
  }
  escreveSenha();
  
}
 
void TeclaPressionada(bool teclaTocada){
  for (int i=0; i<12; i++){
    botoes[i].press(teclaTocada && botoes[i].contains(posX,posY));
  }
  checkPressedButton();
 
}
 
void checkPressedButton(){
  for (int i=0; i<12; i++){
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
 
void escreveSenha(){
  tft.fillRect(1, 30, 240, 30, BRANCO);
  tft.setCursor(5,35); 
  tft.setTextColor(PRETO); 
  tft.setTextSize(3);
  tft.print(senha+"_"); 
}
 
 
void apagarUltimocaractere(){
  if (senha.length()>0){
    senha.remove (senha.length () - 1);
  }
}
void checkPressed(int button){
    botoes[button].drawButton(true);
    esperaSoltar();
    botoes[button].drawButton();
    if(senha.length()<TAMANHOMAXTELL || button == 10 || button == 11){
    switch (button){
        case 0:
          senha = senha + "1";
        break;
        case 1:
          senha = senha + "2";
        break;
        case 2:
          senha = senha + "3";
        break;
        case 3:
          senha = senha + "4";        
        break;
        case 4:
          senha = senha + "5";
        break; 
        case 5:
          senha = senha + "6";
        break;
        case 6:
          senha = senha + "7";
        break;
        case 7:
          senha = senha + "8";
        break;
        case 8:
          senha = senha + "9";
        break;
        case 9:
          senha = senha + "0";
        break;
        case 10:
        //validarAcesso();
        senha ="";
        break; 
        case 11:
        apagarUltimocaractere();
        break;  
    }
    escreveSenha();
    }
}
 
 
 void ligar()
 {
  
 }
 
/*void validarAcesso(){
 
  if (senha == SENHAVALIDA){
     tft.fillScreen(PRETO);
     tft.setCursor(10,100);
     tft.setTextColor(VERDE);
     tft.setTextSize(3);
     tft.print("Acesso Autorizado");
     delay(3000);
     telaInicial();
  }
  else{ // Caso contrário
     tft.fillScreen(PRETO);
     tft.setCursor(40,100);
     tft.setTextColor(VERMELHO); 
     tft.setTextSize(3); 
     tft.print("Acesso Negado"); 
     delay(3000); 
     telaInicial();
  }
  
}
 */
void loop() {
  bool toque = obterToque();
  TeclaPressionada(toque);
}
