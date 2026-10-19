#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

String pedirParaApiClima(String codigo);

const char* nomeRedeWiFi = "SENAI EDUCACIONAL";
const char* senhaRedeWiFi = "info@134";
const char* enderecoApiClima = "https://wttr.in/Sao+Caetano+do+Sul?m&lang=pt&format=";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);
  Serial.println("\n\rIniciando conexão WiFi...\n\r");

  lcd.init();
  lcd.backlight();

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("#");
    delay(500);
  }

  Serial.print("\n\rConectado com sucesso!\n\r");
  Serial.printf("Endereço IPv4: %s\n\r", WiFi.localIP().toString().c_str() );
  Serial.printf("Endereço MAC: %s\n\r", WiFi.macAddress().c_str() );

  String clima;
  int temperatura;
  int sensacaoTermica;

  clima = pedirParaApiClima("%C");
  temperatura = pedirParaApiClima ("%t").toInt();
  sensacaoTermica = pedirParaApiClima ("%f").toInt();

  lcd.setCursor(0, 1);
  lcd.printf("Condição Climatica para hoje: %s\n\r", clima.c_str() );
  lcd.setCursor(0, 2);
  lcd.printf("Temperatura Atual %dC", temperatura);
  lcd.setCursor(0, 3);
  lcd.printf("sensação Termica %dC", sensacaoTermica);

  //Serial.printf("Condição Climatica para hoje: %s\n\r", clima.c_str() );
  //Serial.printf("Temperatura Atual %dC e sensação Termica %dC", temperatura, sensacaoTermica);
}

void loop() 
{
  
}

String pedirParaApiClima(String codigo)
{
  HTTPClient http;
  
  http.begin(enderecoApiClima + codigo);

  int codigoHTTP = http.GET(); //* Retorna o codigo HTTP ex. : 200 -> OK | 404 -> Pagina nao encontrada
  String resposta;

  if(codigoHTTP == 200) //* OK
  {
    resposta = http.getString(); //* Retorna o conteudo HTML da pagina
  }
  else
  {
    Serial.printf("Deu erro. Codigo: %d", codigoHTTP);
  }

  http.end();
  return resposta;
}