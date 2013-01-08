//
//  main.cpp
//  socketpractice1
//
//  Created by Johannes Schmidt on 08.01.13.
//
//

#include "main.h"
#include <stdio.h>          //Ein-/Ausgabesystem von ANSI-C
#include <sys/types.h>      //Deklariert Typ time_t für Zeitfunktionen
#include <arpa/inet.h>      //Definiert Internetfunktionen, z.B. in_addr
#include <sys/socket.h>     //Einbinden der Socket API
#include <netdb.h>          //Definiert Netzwerkdatenbankfunktionen, z.B: in_port
#include <netinet/in.h>     //Stellt Internet-Protokollfamilie bereit
#include <unistd.h>         //Standard für symbolische Konstanten und Typen (Textersetzung beim Compalieren)
#include <iostream>         //Ein-/Ausgabesystem auf objektorientierter Klassenhierarchie (Erweitert stdio)
#include <fstream>          //Datein auf i/o Streams lesen und schreiben

// ohne ".h" ist C++ Standard, mit ".h" sind veraltete Bibliotheken

using namespace std;        //Im Gültigkeitsbereich der Datei wird der Namensbereich "std" durch using zur
                            //Verüfung gestellt. Damit wird z.B. aus std::cout nur cout

int main(int argc, char *argv[]){       //Hauptfunktion :: Dem Programm können Parameter übergeben werden
                                        //argc ist die Anzahl und argv entspricht den Werten der Parameter
    cout << "Wetter-Server" << endl;    //c-out ist Std-Ausgabestrom, "<<" bedeutet auf Strom schieben
                                        //end-l gibt Zeilenumbruch und sorgt für Anzeige
    
    //Integer-Variablen deklarieren
    int server_socket, client_socket, length;
    //Variable client_size vom Typ socklen_t (socket.h) für die Adresslänge
    socklen_t client_size;
    //Variablen server_address, client vom (strukturierten Datentyp) sockaddr_in (netinet/in.h)
    struct sockaddr_in server_address, client_address;
    //char-Variablen deklarieren, Zeichenkette buffer mit 255, file_buffer mit 1024 und string filename (*)
    char buffer[255], *filename;
    char file_buffer[1024];
    
    //sockfd ist Deskriptor eines Sockets
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
    cout << typeid(server_socket).name()<<endl;
    cout << typeid((struct sockaddr_in*)&server_address).name()<<endl;
    cout << typeid(sizeof(server_address)).name() << endl;
    bind(server_socket,(struct sockaddr_in*)&server_address, sizeof(server_address));
    if(bind(server_socket,(sockaddr*) &server_address, sizeof(server_address))==-1){
        printf("Fehler: bind() fehlgeschlagen!");
    }
    
    if(listen(server_socket,3)==-1){
        printf("Fehler: listen() fehlgeschlagen!");
    }
    
    while(1){
        client_size = sizeof(client_address);
        if((client_socket = accept(server_socket, (sockaddr*) &client_address, &client_size)) == -1) {
            printf("Fehler: accept() fehlgeschlagen!"); }
    }
    
}



