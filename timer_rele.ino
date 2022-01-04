/* 
 *  Liga e desliga o rele em horários pré-determinados
 *  
 *  Funciona sem RTC. Precisa conectar o Serial Monitor antes de dar upload.
 *  
 *  Ao ligar o Arduino, ele vai perguntar a hora de ligar e de desligar. Informar duas horas válidas, separadas por espaços, vírgulas ou qualquer outro sinal.
 *  
 *  Depois, ele vai perguntar a hora atual. Informe a hora no formato hh:mm:ss (ou hh.mm.ss ou hh mm ss...)
 *  
 *  Daí em diante, é só esperar para ligar as luzes de Natal...
 *  
 *  O rele deve ser ligado em uma porta digital. Nesse caso, usei a porta D10. Se for usar outra, ajuste a variável "porta_rele"
 *  
 *  Também é possivel ajustar a velocidade do relógio (não consegui precisão absoluta sem o RTC) na variável "ajuste"
 *  
 *  TODO: por enquanto, só funciona se a hora de ligar < hora de desligar. Precisa ajustar o código caso queira ligar às 19h e desligar às 2h da madrugada.
 *  
 */

// https://electronics-project-hub.com/digital-clock-using-arduino/#:~:text=Arduino%20Digital%20Clock%20without%20RTC%20The%20circuit%20consists,interested%3A%20Analog%20Clock%20Using%20Servo%20motor%20and%20Arduino

int horaini, horafim;
int hora, minuto, segundo;
bool pronto = false, ligado = false;
const int ajuste = -474; // valores menores -> aceleram o relógio
const int porta_rele = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(porta_rele, OUTPUT);
//  Serial.begin(9600);
  Serial.begin(57600);
  Serial.setTimeout(10000000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!pronto) {
    desliga_rele();
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("Hora de ligar, hora de desligar:");
  //while (!Serial.available()) {
  horaini = Serial.parseInt();
  horafim = Serial.parseInt();
  //char buffer0[40];
  //sprintf(buffer0,"Liga %d e desliga %d",horaini,horafim);
  //Serial.println(buffer0); 
  if (Serial.read() == '\n') {
    Serial.println("Hora:min:seg atuais:");
    hora = Serial.parseInt();
    minuto = Serial.parseInt();
    segundo = Serial.parseInt();
    //Serial.println("Peguei as horas");
    char horas[9];
    sprintf(horas,"%02d:%02d:%02d",hora,minuto,segundo);
    Serial.println(horas);
    if (Serial.read() == '\n') { 
      hora = constrain(hora,0,23);
      minuto = constrain(minuto,0,59);
      segundo = constrain(segundo,0,59);
      pronto = true;
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    }
  }
  //}
  }
 if (pronto) relogio();
}

 void relogio(){
  char buffer1[80];
  sprintf(buffer1, "Hora de ligar: %02d Hora de desligar: %02d Hora atual: %02d:%02d:%02d",
    horaini, horafim, hora, minuto, segundo);
  Serial.println(buffer1);
  //Serial.println(segundo);
  delay(999);
  delayMicroseconds(1000+ajuste);
  segundo += 1;
  if (segundo == 60) {
    segundo = 0;
    minuto += 1;
  }
  if (minuto == 60) {
    minuto = 0;
    hora += 1;
  }
  if (hora == 24) {
    //Serial.println("Virada de dia");
    hora = 0;
  }
  if (hora >= horaini & hora <= horafim & !ligado) {
    liga_rele();
  }
  if (hora >= horafim & ligado) {
    desliga_rele();
  }
}

void liga_rele() {
  digitalWrite(porta_rele, LOW); // liga led verde e liga porta da esquerda
  digitalWrite(LED_BUILTIN, HIGH);
  ligado=true;
}

void desliga_rele() {
  digitalWrite(porta_rele, HIGH); // desliga led verde e liga porta da direita
  digitalWrite(LED_BUILTIN, LOW);
  ligado=false;
}
