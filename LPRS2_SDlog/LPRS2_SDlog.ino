#include <SD.h>
#include <SPI.h>

File myFile;
Sd2Card card;
SdVolume volume;
SdFile root;

int chipSelect = 10;

void setup() 
{
  Serial.begin(9600);
  pinMode(chipSelect, OUTPUT);

  // inicijalizacija
  while(!Serial);
    if(!card.init(SPI_QUARTER_SPEED, chipSelect))
    {
      Serial.println("Inicijalizacija 1 neuspesna.");
      while(1);
    } 
    else
    {
      Serial.println("Inicijalizacija 1 uspesna. Povezivanje je uspesno i kartica je prisutna.");
      if(SD.begin(chipSelect))
      {
        Serial.println("Iniciajlizacija 2 uspesna.");
      }
      else
      {
        Serial.println("Iniciajlizacija 2 neuspesna.");
      }
    }
}

// cita ulazni fajl, stampa ga liniju po liniju na Serial monitor
void readFile(String filename)
{
    myFile = SD.open(filename, FILE_READ);
    if(myFile)
    {
        while(myFile.available())
        {
            Serial.write(myFile.read()); // cita liniju po liniju
        }
        myFile.close();
    }
    else
    {
        Serial.println("Greska u otvaranju fajla. (readFile)");
    }
}

void ls()
{
  File rootDir = SD.open("/");
  while(true)
  {
    File f = rootDir.openNextFile();
    if (!f)
    {
      // nema vise fajlova
      f.close();
      break;
    }
    Serial.println(f.name());
    f.close();
  }
}

// pise iz Serial monitora u fajl
void writeFile(String filename)
{
    myFile = SD.open(filename, FILE_WRITE);
    if(myFile)
    {
        Serial.println("Unesi tekst.");
        while(!Serial.available()); // cekamo da korisnik nesto ukuca (i posalje) na Serial
        myFile.println(Serial.readString()); // upisujemo sve Strignove sa Serial ulaza u fajl "na kartici"
        
        int ok = 1;
        while(ok){
          Serial.println("Da li zelite jos da upisujete???\n->(1) da\n->(2) ne");
          while(!Serial.available());
          char choice = Serial.read();

          if(choice == '1'){
            Serial.println("Unesi tekst.");
            while(!Serial.available());
            myFile.println(Serial.readString());
          }
          else if(choice == '2')
            ok = 0;
          else
            Serial.println("Nevazeci ulaz. 1 za nastavak, 2 za kraj.");
        }
        
        myFile.close(); // a zapis se zapravo izvrsava tek kad zatvorimo ovde fajl
    }
    else
    {
        Serial.println("Greska u otvaranju fajla (writeFile)");
    }
}

void clearFile(String filename)
{
  if(SD.exists(filename))
  {
    SD.remove(filename);
    myFile = SD.open(filename, FILE_WRITE);
    myFile.close();
    Serial.print("Fajl ispraznjen.");
  }
  else
  {
    Serial.print("Fajl nije pronadjem.");
  }
}

void deleteFile(String filename)
{
  if(SD.exists(filename))
  {
    SD.remove(filename);
    Serial.print("Fajl obrisan.");
  }
  else
  {
    Serial.print("Fajl nije pronadjem.");
  }
}

void ispisiAutore()
{
  Serial.println("Milica Tadic\nMarija Bajic\nStefan Kurcan");
}

// stampa meni na Serial monitor, ceka izbor funkcije u obliku broja i naziv fajla sa .txt na kraju naziva ("a.txt")
void printMenu()
{
    Serial.println("Biraj:\n -> (1) Citam\n -> (2) Pisem\n -> (3) Brisem\n -> (4) Brisem fajl\n -> (5) Izlistavam fajlove\n -> (9) Ispisi autore");

    //biramo opciju
    while(!Serial.available());
    char choice = Serial.read();
    Serial.flush();

    //biramo fajl
    String filename;
    if(choice != '5' && choice != '9')
    {
      Serial.println("Naziv fajla? (filename.txt)");
      while(!Serial.available());
      filename = Serial.readString();
      Serial.println(filename);
    }
    
    if(choice == '1') 
    {
        readFile(filename);
    } 
    else if(choice == '2')
    {
        writeFile(filename);
    }
    else if(choice == '3')
    {
        clearFile(filename);
    }
    else if(choice == '4')
    {
        deleteFile(filename);
    }
    else if(choice == '5')
    {
        ls();
    }
    else if(choice == '9')
    {
        ispisiAutore();
    }
    else
    {
        Serial.println("Nevazeci ulaz.");
    }
}


void loop() 
{
    printMenu();
    delay(3000);  
}
