#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "virement.h"
#include "utils_v1.h"

#define MAX 50

// PRE: ServerIP : a valid IP address
//      ServerPort: a valid port number
// POST: on success connects a client socket to ServerIP:port
//       return socket file descriptor
//       on failure, displays error cause and quits the program
int initSocketClient(char *ServerIP, int Serverport)
{
  int sockfd = ssocket();
  sconnect(ServerIP, Serverport, sockfd);
  return sockfd;
}

int main(int argc, char **argv)
{ 
  char command[MAX];
  int nbCharRead = 0;
  if(argc != 5){
    /*
      TODO : Affichage Success à la fin non voulu
    */
    perror("Nombre d'arguments incorrect !\n");
    exit(EXIT_FAILURE);
  }

  char *server_ip = argv[1];
  int port = atoi(argv[2]);
  //int compte_client = atoi(argv[3]);
  //int delay = atoi(argv[4]);

  printf("Bienvenue dans la banque :\n");
  printf("Pour éffectué un virement normal tapper la commande : '+ [numero_compte_beneficiaire] [montant]'\n");
  printf("Pour éffectué un virement recurrent tapper la commande : '* [numero_compte_beneficiaire] [montant]'\n");
  printf("Pour quitter le programme tapper q \n");


  do{
    write(1,">", 1);
    nbCharRead = sread(STDIN_FILENO, command, MAX);
  }while(nbCharRead > 0);


  Virement virementTest;
  virementTest.compte_beneficiaire = 1;
  virementTest.compte_emetteur = 2;
  virementTest.somme = 30;

 // int ret = sread(0, vire, 4);

  int sockfd = initSocketClient(server_ip, port);
  swrite(sockfd, &virementTest, sizeof(virementTest));

  /* wait server response */
  sread(sockfd, &virementTest, sizeof(virementTest));

  if (virementTest.code == VIREMENT_OK)
  {
    printf("Réponse du serveur : Virement accepté\n");
  }
  else if (virementTest.code == VIREMENT_KO)
  {
    printf("Réponse du serveur : Virement refusé\n");
  }


  sclose(sockfd);
  return 0;
}
