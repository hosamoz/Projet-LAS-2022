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

#define BACKLOG 5


volatile sig_atomic_t end = 0;

void endServerHandler(int sig)
{
  end = 1;
}

/*void terminate(Player *tabPlayers, int nbPlayers)
{
  printf("\nJoueurs inscrits : \n");
  for (int i = 0; i < nbPlayers; i++)
  {
    printf("  - %s inscrit\n", tabPlayers[i].pseudo);
  }
  exit(0);
}*/

// PRE:  ServerPort: a valid port number
// POST: on success bind a socket to 0.0.0.0:port and listen to it
//       return socket file descriptor
//       on failure, displays error cause and quits the program
int initSocketServer(int port)
{
  int sockfd = ssocket();

  /* no socket error */

  sbind(port, sockfd);

  /* no bind error */
  slisten(sockfd, BACKLOG);

  /* no listen error */
  return sockfd;
}

int main(int argc, char **argv)
{
  Virement virementTest;

  sigset_t set;
  ssigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigaddset(&set, SIGTERM);
  ssigprocmask(SIG_BLOCK, &set, NULL);

  ssigaction(SIGTERM, endServerHandler);
  ssigaction(SIGINT, endServerHandler);

  int sockfd = initSocketServer(SERVER_PORT);
  printf("Le serveur tourne sur le port : %i \n", SERVER_PORT);

  // setsockopt -> to avoid Address Already in Use
  int option = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

  ssigprocmask(SIG_UNBLOCK, &set, NULL);

  while (!end)
  {

    /* client trt */
    int newsockfd = accept(sockfd, NULL, NULL);
    if (end)
    {
      //terminate(tabPlayers, nbPlayers);
      printf("Je termine mon boulot");
    }
    checkNeg(newsockfd, "ERROR accept");

    ssize_t ret = read(newsockfd, &virementTest, sizeof(virementTest));
    if (end)
    {
      //terminate(tabPlayers, nbPlayers);
      printf("Je termine mon boulot!");
    }
    checkNeg(ret, "ERROR READ");

    printf("Virement envoyé : %d\n", virementTest.somme);
    /*if (nbPlayers < MAX_PLAYERS)
    {
      msg.code = INSCRIPTION_OK;
      strcpy(tabPlayers[nbPlayers].pseudo, msg.messageText);
      tabPlayers[nbPlayers].sockfd = newsockfd;
      nbPlayers++;
    }
    else
    {
      msg.code = INSCRIPTION_KO;
    }*/

    virementTest.code = 1;

    nwrite(newsockfd, &virementTest, sizeof(virementTest));
    
  }
  sclose(sockfd);
}
