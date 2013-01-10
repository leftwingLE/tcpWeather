#include <stdio.h>          //Ein-/Ausgabesystem von ANSI-C
#include <sys/types.h>      //Deklariert Typ time_t für Zeitfunktionen
#include <arpa/inet.h>      //Definiert Internetfunktionen, z.B. in_addr
#include <sys/socket.h>     //Einbinden der Socket API
#include <netdb.h>          //Definiert Netzwerkdatenbankfunktionen, z.B: in_port
#include <netinet/in.h>     //Stellt Internet-Protokollfamilie bereit
#include <unistd.h>         //Standard für symbolische Konstanten und Typen (Textersetzung beim Compalieren)
#include <iostream>         //Ein-/Ausgabesystem auf objektorientierter Klassenhierarchie (Erweitert stdio)

// ohne ".h" ist C++ Standard, mit ".h" sind veraltete Bibliotheken

using namespace std;        //Im Gültigkeitsbereich der Datei wird der Namensbereich "std" durch using zur
                            //Verfügung gestellt. Damit wird z.B. aus std::cout nur cout



//Funktion: "find_line_end", Rückgabewert: String
// Zum Bestimmen des String-Endes. Da Eingebene Städtenamen mit ":" versehen werden, um das Eingabeende zu ermitteln.
string find_line_end(string line)
{
    //Deklaration: Variable: "position", Datentyp: unsigned long [bzgl. find Rückgabetyp von find()]
    unsigned long position;
    //Zuweisung: Variable "position" wird Position des ":"-Zeichens aus der Eingabe Zeile zugewiesen.
    position = line.find(":",0);
    //String-Funktion erase: Löscht alle Zeichen des Strings "line" ab Position.
    line.erase(position);
    //Rückgabe: Gibt String "line" zurück. Enthält alle Zeichen bis ausschließlich ":".
    return line;
}

// Funktion: "weather", Rückgabewert: String.
// Zur Abwicklung der Wetterabfrage, gibt zur Eingabe der Stadt das jeweilige Wetter zurück.
string weather(string city)
{
    //Deklaration: Variable "output", Datentyp: "string", Rückgabevariable für das Wetter
    string output;
    
    //Bedingungprüfung: Ordnet der Stadt das Wetter zu
    if(city=="Leipzig") output ="Sonnig, 20 Grad:";
    else if(city=="Stuttgart") output="Sonnig, 23 Grad:";
    else if(city=="Hamburg") output="Windig, 18 Grad:";
    else if(city=="Erlangen") output="Regen, 19 Grad:";
    else if(city=="Konstanz") output="Neblig, 24 Grad:";
    else output="Bitte ueberpruefen Sie die Eingabe!:";
    
    //Rückgae: Gibt String "output" mit Wetterdaten zurück.
    return output;
}

int main(int argc, char *argv[]){       //Hauptfunktion :: Dem Programm können Parameter übergeben werden
                                        //argc ist die Anzahl und argv entspricht den Werten der Parameter
    cout << "Wetter-Server" << endl;    //c-out ist Std-Ausgabestrom, "<<" bedeutet auf Strom schieben
                                        //end-l gibt Zeilenumbruch und sorgt für Anzeige
    
    //Deklaration: Variablen "server_socket" und "client_socket", Datentyp: integer, werden für die Socket-Deskriptoren genutzt.
    int server_socket, client_socket;
    //Deklaration: Variable "client_size", Datentyp: socklen_t (socket.h), werdeb für die IP-Adresslänge der Clientadresse genutzt.
    socklen_t client_size;
    //Deklaration: Variablen "server_address" und "client_address", Datentyp: (strukturierten Datentyp) sockaddr_in (netinet/in.h), werden für die IP-Adressen genutzt.
    struct sockaddr_in server_address, client_address;
    //Deklaration: "buffer", Datentyp: char, Speicher für "recv"-Empfang als Zeichenkette 255 Zeichen.
    char buffer[255];
    //Deklaration: Variablen "input", "output", Datentyp: string, werden für Ein-und Ausgaben genutzt.
    string input,output;
    
    //server_socket ist Deskriptor eines Sockets
    //Protokolfamilie: AF_INET(netinet/in.h) stellt TCP bei verbindungsorieten Stream-Transfers, UDP bei verbindungslosen Stream-Transfers bereit
    //Type: SOCK_STREAM legt verbindungsorieten Stream-Fest
    //Protocol: 0 bedeutet Standardprotokoll benutzen
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    
    //Überprüfung ob Socket geöffnet werden kann, sonst Fehlerausgabe
    if(server_socket<0){
        printf("Fehler: Socket konnte nicht geöffnet werden!");
    }
    
    //Variable server_address vom strukturierten Typ sockaddr_in (netinet/in.h) wird nun initialisiert
    server_address.sin_family = AF_INET;            //Protokolfamilie TCP/UDP
    //Serveradresse sin_addr von strukturierten Datentyp in_addr (netinet/in.h) :: Konstante INADDR_ANY bewirkt, dass jede IP-Adresse des Rechners genutzt werden kann. Somit ist keine Konfiguration der IP-Adresse notwendig --> Multihomehost
    server_address.sin_addr.s_addr = INADDR_ANY;
    //Serverport: 50000
    //htons (Host To Network Short) gibt Nummer in Network Byte Order zurück
    server_address.sin_port = htons(50000);
    
    //bind :: Bindet Socket, Portnummer und IP-Adresse für Kommunikation
    //IF-Konstrukt zur Fehlerausgabe, falls bind()==-1 ist und somit fehlschlägt
    //sockfd ist Deskriptor des Sockets
    if(bind(server_socket,(sockaddr*)&server_address, sizeof(server_address))==-1){
        printf("Fehler: bind() fehlgeschlagen!");
        return EXIT_FAILURE;
    }
    
    //listen :: Veretzt Server-Socket in einen passiven Betriebszustand
    //Socket-Socket wartet auf Anfragen vom Client mit der Warteschlangengröße von 3 Anfragen.
    //Ausgabe einer Fehlermeldung, falls "listen()" fehlschlägt
    if(listen(server_socket,3)==-1){
        printf("Fehler: listen() fehlgeschlagen!");
        return EXIT_FAILURE;
    }
    
    //Endlosschleifen, damit Server dauerhaft in Betrieb ist.
    while(1){
        //Liest Größe vom der Client-Adresse aus (durch sturkturierte Datentyp vorgegeben)
        client_size = sizeof(client_address);
        //Meldung in der Konsole, dass Server auf Client wartet.
        cout << "Warte auf Anfrage vom Client..." << endl;
        
        //IF-Bedinung mit Versuch der Verbindungsannahme
        //Server-Socket akzeptiert Clientanfrage und stellt für die Client-Server-Verbindung
        //neuen Socket, den Client-Socket, bereit
        //Dabei werden die mit Zeigerversehenen Variablen "client_address" und "client_size"
        //automatisch mit den Daten com Client ausgefüllt.
        //Ausgabe einer Fehlermeldung, falls Annahme fehlschlägt.
        if((client_socket = accept(server_socket,(sockaddr*)&client_address,&client_size))==-1)
        {
            cout<<("Fehler bei accept")<<endl;
            return EXIT_FAILURE;
        }
        
        //Anzeige von Client-IP in der Serverkonsole 
        printf("Client: %s\n",inet_ntoa(client_address.sin_addr));
        
        //Emfpang der Nachricht der Client-Server-Verbindung (bzgl "client_socket")
        //Nachricht wird in der Speicher-Variable "buffer" geschrieben.
        recv(client_socket, buffer, sizeof(buffer),0);
        
        //In die Ausgabe wird Rückgabe der "weather"-Funktion geschrieben, welche die "find_line_end"-Funktion als Parameter enthält, die wiederum den "buffer" als Paramter übergeben bekommt.
        output = weather(find_line_end(buffer));
        
        //Server sendet anschließend über die Verbindung ("client_socket" )
        //die string Ausgabe wird mit "c.str()" in ein Feld geschrieben und liefert einen Zeiger auf das Feld zurück. Hängt eine 0 am Ende an (als C-String-Endzeichen)
        //Es werden mit 0-Bits für Optionen angehangen, also werden keine Flags o.ä. übertragen
        send(client_socket, output.c_str(),output.size(),0);
        
        
        //Verbindung zw. Client und Server wird geschlossen
        close(client_socket);
    }
    
}
