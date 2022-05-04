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
  if(argc != 5){
    perror("Nombre d'arguments incorrect !\n");
    exit(EXIT_FAILURE);
  }

  char *server_ip = argv[1];
  int port = atoi(argv[2]);
  //int compte_client = atoi(argv[3]);
  //int delay = atoi(argv[4]);




  /* retrieve player name */
  printf("Bienvenue dans la banque, veuillez introduire votre virement :\n");

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
    printf("Réponse du serveur : Inscription acceptée\n");
  }
  else if (virementTest.code == VIREMENT_KO)
  {
    printf("Réponse du serveur : Inscription refusée\n");
  }


  sclose(sockfd);
  return 0;
}
