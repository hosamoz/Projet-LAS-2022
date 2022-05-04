#ifndef _VIREMENT_H_
#define _VIREMENT_H_

#define SERVER_PORT 	9501
#define SERVER_IP		"127.0.0.1"  /* localhost */
#define VIREMENT_OK 1
#define VIREMENT_KO 0


/* struct message used between server and client */
typedef struct {
  int compte_beneficiaire;
  int somme;
  int compte_emetteur;
  int code;
} Virement;
#endif
