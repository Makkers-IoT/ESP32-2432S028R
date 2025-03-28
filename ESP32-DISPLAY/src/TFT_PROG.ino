#include <Arduino.h> 
#include <SPI.h>    
#include <TFT_eSPI.h>   
#include <PNGdec.h>  

#include <logo.h>

/*--------------------------------------
                Display
--------------------------------------- */
#define BLACK   0x0000
int xpos = 0;  // Posição X onde a imagem será desenhada
int ypos = 0;  // Posição Y onde a imagem será desenhada
int widhtImg = 0;
int heightImg = 0;

TFT_eSPI tft = TFT_eSPI();

PNG png;  // Criando a instância do PNGdec


void setup() {
    Serial.begin(115200);

    // Inicializa a tela
    tft.init();
    tft.setRotation(3);

    intro(); // Exibe a logo
}

void loop() {
}

void intro() {
  tft.fillScreen(BLACK);  // Limpa a tela

  widhtImg = 320;
  heightImg = 240;
  xpos = (tft.width() - widhtImg) / 2;  // Centraliza horizontalmente
  ypos = (tft.height() - heightImg) / 2;  // Centraliza verticalmente

  displayImage((uint8_t *)logo, sizeof(logo));  // Exibe a primeira logo
}



// Função genérica para exibir imagens
void displayImage(uint8_t* imgData, size_t imgSize) {
  int16_t rc = png.openFLASH(imgData, imgSize, pngDraw);
  if (rc == PNG_SUCCESS) {
    Serial.println("Imagem PNG carregada com sucesso");
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);  // O png.decode() não precisa de parâmetros de posição aqui
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
  } else {
    Serial.println("Falha ao carregar imagem");
  }
}

// Função de callback para desenhar cada linha da imagem
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[320];  // Largura máxima da imagem, ajustando conforme necessário

  // Obtém a linha da imagem no formato RGB565
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xFFFFFF);
  
  // Exibe a linha no display usando as coordenadas passadas
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
