// Rodrigo García
// Carné: 202178
// Lab6_Digital2

#include <SD.h>
#include <SPI.h>

File archivo;
char option = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //iniciamos comunicacion serial
  SPI.setModule(0);
  while (!Serial) {
    ;
  }

  Serial.print("Iniciando tarjeta SD...");
  pinMode(A3, OUTPUT);

  if (!SD.begin(A3)) {
    Serial.println("inicialización falló!");
    return;
  }
  Serial.println("inicialización terminada");

  archivo = SD.open("/");
  printDirectory(archivo, 0);
  Serial.println("terminado!");
  Serial.println(" ");
  Serial.println("----------------------------------");
  Serial.println("               MENU               ");
  Serial.println("----------------------------------");
  Serial.println("  1. Desplegar imagen de SIVA     ");
  Serial.println("  2. Desplegar logo del Imperio   ");
  Serial.println("  3. Desplegar imagen de Destiny  ");
  Serial.println("  4. Crear nuevo archivo          ");
  Serial.println("  5. Desplegar imagen del archivo nuevo  ");

}

void loop() {
  if (Serial.available()) {
    char datos = Serial.read();
    if (datos >= '1' && datos <= '5') {
      option = datos; // Colocamos la opcion deseada
      if (option == '1') {
        archivo = SD.open("siva.txt");
        Serial.println("siva.txt:");

        // read from the file until there's nothing else in it:
        while (archivo.available())
        {
          Serial.write(archivo.read());
        }
        // close the file:
        archivo.close();
      }
      else if (option == '2') {
        archivo = SD.open("imperio.txt");
        Serial.println("imperio.txt:");

        // read from the file until there's nothing else in it:
        while (archivo.available())
        {
          Serial.write(archivo.read());
        }
        // close the file:
        archivo.close();

      }
      else if (option == '3') {
        archivo = SD.open("destiny.txt");
        Serial.println("destiny.txt:");

        // read from the file until there's nothing else in it:
        while (archivo.available())
        {
          Serial.write(archivo.read());
          // close the file:
        }
        archivo.close();
      }
      else if (option == '4') {
        Serial.println("Escribe el nombre del archivo que deseas crear (ej. nuevo.txt)");
      }
      else if (option == '5') {
        Serial.println("Escribe el nombre del nuevo archivo");
      }
    }
    else if (option != '0') { // Ya se selecciono una opcion
      if (option == '4') {
        // Que se cree un archivo nuevo y guardarlo
        String nuevo = datos + Serial.readStringUntil('\n');
        writeAndSaveFile(nuevo.c_str());
        option = '0';
      }
      else if (option == '5'){
        // Escribir el nombre del archivo creado
        String file = datos + Serial.readStringUntil('\n');
        readAndPrintFile(file.c_str());
        option = '0'; // Reseteamos opcion
      }

      else {
        Serial.println("Error");
      }
    }

    else {
      Serial.println("Error");
    }
  }
}



/********************************************/
void readAndPrintFile(const char* filename)
{
  archivo = SD.open(filename);
  if (archivo) {
    Serial.print(filename);
    Serial.println(":");
    while (archivo.available()) {
      Serial.write(archivo.read());
    }
    archivo.close();
  } else {
    Serial.print("Error al iniciar ");
    Serial.println(filename);
  }
}

void writeAndSaveFile(const char* filename)
{
  String data;
  Serial.println("Ingresa el texto del ASCII, asegura que sea de 80 caracteres de largo para que funcione correctamente");
  while (!Serial.available()) {
    //Esperamos input del usuario
  }
  data = Serial.readStringUntil('\n');

  archivo = SD.open(filename, FILE_WRITE);
  if (archivo) {
    Serial.print("Escribiendo archivo:");
    Serial.println(filename);

    int charContador = 0;
    for (int i = 0; i < data.length(); i++)
    {
      archivo.print(data[i]);
      charContador++;
      if (charContador == 81)
      {
        archivo.println();
        charContador = 0;
      }
    }
    archivo.close();
    Serial.println("Hecho.");
  }
  else {
    Serial.print("Error creando archivo: ");
    Serial.println(filename);
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
