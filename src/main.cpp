#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

String pedirParaApiClima(String codigo);
void dashboardHorario();
void telaInicial();

const char* nomeRedeWiFi = "EMERSONYM_2G";
const char* senhaRedeWiFi = "Eym25121979@";
const char* enderecoApiClima = "https://wttr.in/Sao+Paulo?m&lang=pt&format=";

//*=============SERVIDOR NTP=======================================
const char* SERVIDOR_NTP_1 = "a.stl.ntp.br";
const char* SERVIDOR_NTP_2 = "pool.ntp.org";
const char* FUSO_HORARIO = "<-03>3";
const char* DIAS_SEMANA_PTBR[] = {"Domingo", "Segunda-Feira", "Terca-Feira", "Quarta-feira", "Quinta-Feira", "Sexta-Feira", "Sabado"};

void setup()
{
  Serial.begin(115200);
  WiFi.begin(nomeRedeWiFi, senhaRedeWiFi);
  Serial.println("\n\rIniciando conexão WiFi...\n\r");

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("#");
    delay(500);
  }

  Serial.print("\n\rConectado com sucesso!\n\r");
  Serial.printf("Endereço IPv4: %s\n\r", WiFi.localIP().toString().c_str() );
  Serial.printf("Endereço MAC: %s\n\r", WiFi.macAddress().c_str() );

  //* Configuração de fuso horario, servidores NTP e inicia a sincronização
  configTzTime(FUSO_HORARIO, SERVIDOR_NTP_1, SERVIDOR_NTP_2);

  struct tm agora;
  
  while(!getLocalTime(&agora) ) //* parametro é o endereço (&) da estrutura que ira receber dados
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\n\rHora sincronizada com sucesso");

  telaInicial();
}

void loop() 
{
  
  dashboardHorario();
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

void dashboardHorario()
{
 struct tm agora;

  if(getLocalTime(&agora))
  {

    Serial.printf("Hoje é %s, %02d/%02d/%d - %02d:%02d:%02d\n\r",
                    DIAS_SEMANA_PTBR[agora.tm_wday],
                    agora.tm_mday,
                    1 + agora.tm_mon,
                    1900 + agora.tm_year,
                    agora.tm_hour,
                    agora.tm_min,
                    agora.tm_sec);

                    time_t epoch_time = mktime(&agora);
                    Serial.printf("Timestamp: %d\n\r", epoch_time);
    delay(1000);

  }
}

void telaInicial()
{
  String clima;
  int temperatura;
  int sensacaoTermica;

  clima = pedirParaApiClima("%C");
  temperatura = pedirParaApiClima ("%t").toInt();
  sensacaoTermica = pedirParaApiClima ("%f").toInt();

  Serial.printf("Condição Climatica para hoje: %s\n\r", clima.c_str() );
  Serial.printf("Temperatura Atual %dC e sensação Termica %dC", temperatura, sensacaoTermica);
}

/* struct  tm agora;

  if(getLocalTime(&agora) )
  {
    Serial.printf("Hoje é %s, %02d/%02d/%d - %02d:%02d:%02d\n\r",
                    DIAS_SEMANA_PTBR[agora.tm_wday],
                    agora.tm_mday,
                    1 + agora.tm_mon,
                    1900 + agora.tm_year,
                    agora.tm_hour,
                    agora.tm_min,
                    agora.tm_sec);

                    time_t epoch_time = mktime(&agora);
                    Serial.printf("Timestamp: %d\n\r", epoch_time);
    delay(1000); */