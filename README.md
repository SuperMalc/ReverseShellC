## Multiclient reverse shell - winsock2
A simple multiclient reverse shell written in C with Winsocks2 library
[Compiled with Dev-C + + 5.11]

Server side:
```
        REVERSE SHELL TCP MULTICLIENT
        * Server: Rel.1.0.3.1 - 02/08/2023
        * Autore: Malcolm Mami (SuperMalc)

 # Server in fase di avvio:

 > Controllo su avvio modulo ws2 -->OK
 > Generazione socket di ascolto -->OK
 > Controllo binding su indirizzo e porta -->OK
 > Inizializzazione ascolto -->OK

 # Server avviato: in attesa di connessioni in arrivo, porta: 8888
```

Client side:

```
        CLIENT TCP
        * Client: Rel.1.0.3.1 - 02/08/2023


 > Inizializzazione winsock -->OK
        Nome indirizzo server: 127.0.0.1
        Porta utilizzata dal server: 8888
        Tipologia: AF_INET
        Lunghezza: 4
        Indirizzo IP #1: 127.0.0.1
 > Acquisizione nome computer -->OK
 > Controllo acquisizione nome -->OK [ DESKTOP-LIJUQMO ]
 + Creato un nuovo socket
 + Connessione al server riuscita (attesa risposta)
 + Autenticazione riuscitadesktop-lijuqmo\robot

```
