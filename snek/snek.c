#include <sys/socket.h> // for socket()
#include <arpa/inet.h>  // for add6
#include <stdio.h>      // for printf()
#include <unistd.h>     // for read()
#include <stdlib.h>     // for malloc()
#include <string.h>     // for strlen()
#include <time.h>       // for time()
#include <fcntl.h>      // for fcntl()
#include <pthread.h>    

// sockets
#define DOMAIN AF_INET6 // ipv6
#define LOOPBACK "::1"  // "loopback" ipv6 - network locally

// debug
#define VERBOSE 1
#define BUFF 16

// booleans
#define TRUE 1
#define FALS 0
typedef int bool;

// gameplay
#define HIGH 23
#define WIDE 80


#define DOMAIN AF_INET6
#define PORT   htons(0xC271)
#define SIZE   1024

// shorter names for addresses and casts
typedef struct sockaddr_in6 *add6;
typedef struct sockaddr *add4;

// int r = rand();

struct Node { 
    int x;
	 int y;
    struct Node *next; 
}; 

struct Node *make_node(int x, int y, struct Node *next) {
	struct Node *new = malloc(sizeof(struct Node)) ;
	new->x = x ;
	new->y = y ;
	new->next = next ;
	return new ;
}

int move(struct Node *head, struct Node *apple, char input) {
	struct Node *node = head;
	int x = node->x, y = node->y;
	switch(input) {
		case 'w': 
			y = node->y - 1;
			break;
		case 'a': 
			x = node->x - 1;
			break;
		case 's': 
			y = node->y + 1;
			break;
		default: 
			x = node->x + 1;
			break;
	}
	int currentx = node->x, currenty = node->y;
	while (node->next != NULL) {
		currentx = node->x; currenty = node->y;
		node->x = x; node->y = y;
		node = node->next;
		x = currentx; y = currenty;
	}
	currentx = node->x; currenty = node->y;
	node->x = x; node->y = y;
	if (head->x == apple->x && head->y == apple->y) {
		struct Node *new = make_node(currentx, currenty, NULL);
		node->next = new;
		apple->x = (rand() % (WIDE - 2)) + 1;
		apple->y = (rand() % (HIGH - 2)) + 1;
	}
	return 0;
}

int render(struct Node *node, struct Node *apple) {
	char **rows = malloc(sizeof(char*) * HIGH);
	for (int y = 0; y < HIGH; y++) {
		char *col = malloc(sizeof(char) * WIDE);
		for (int x = 0; x < WIDE; x++) {
			char point = ' ';
			if (y == 0 || y == HIGH - 1 || x == 0 || x == WIDE - 1) {
				point = 'X';
			}
			col[x] = point;
		}
		rows[y] = col;
	}
	rows[apple->y][apple->x] = '@'; //print apple
	while (node != NULL) {
		printf("node: %d %d\n", node->x, node->y);
		rows[node->y][node->x] = 'O';
		node = node->next;
	}
	for (int i = 0; i < HIGH; i++) {
		printf("%s\n", rows[i]);
	}
	return 0;
}

void *server_help(void *buff){

	struct Node *head = make_node(40, 10, NULL);
	struct Node *apple = make_node(60, 5, NULL);
	apple->x = 60; apple->y = 5; apple->next = NULL;
	while (TRUE) {
		usleep(500000); // wait
		char *input = (char *) buff; //read input type
		if (input[0]=='q'){ // quit
			printf("quit");
			return 0;
		}
		printf("%c\n", input[0]); 
		move(head, apple, input[0]); // move the snake or whatever
		render(head, apple); // print game
		if (head->y >= HIGH - 1 || head->y <= 0 || head->x >= WIDE - 1 || head->x <= 0) { // does snake die
			printf("died x_x\n");
			exit(0);
			return 0;
		}
	}
	return 0;
}


int server(int argc, char const *argv[]) {
   printf("Starting server...\n");
   int sock = socket(DOMAIN, SOCK_STREAM, 0), opt = 1, conx ;
	socklen_t s = sizeof(struct sockaddr_in6) ;
	struct sockaddr_in6 addr ;
	char buff[SIZE] ;
	
	addr.sin6_family = DOMAIN ;
	addr.sin6_port   = PORT   ;
	addr.sin6_addr   = in6addr_any ;
	memset(buff, 0, SIZE) ;
	
	// make testing easier
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
		perror("Server - setsockopt addr failed.\n") ;
		exit(-1) ;
	}
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))){
		perror("Server - setsockopt port failed.\n") ;
		exit(-1) ;
	}
	// end testing ease
	if (bind(sock, (struct sockaddr *)&addr, s)){
		perror("Server - bindin failed.\n") ;
		exit(-1) ;
	}
	if (listen(sock, 1)){	
		perror("Server - listen failed.\n") ;
		exit(-1) ;
	}
	conx = accept(sock, (struct sockaddr *)&addr, &s) ;
	if (conx == -1){
		perror("Server - accept failed.\n") ;
		exit(-1) ;
	}
	printf("starting server thread...\n");
	pthread_t thread_id; 
	pthread_create(&thread_id, NULL, server_help, (void *) buff); 
	while (TRUE){
		printf("buff: %s\n", buff) ;
		read(conx, buff, SIZE);
		if (buff[0]=='q'){
			printf("quit\n");
			return 0;
		}
	}
	return 0 ;
}

int client(int argc, char const *argv[]) {
   printf("Starting client...\n");

   int sock;
   struct sockaddr_in6 address; 
	char buff[SIZE];
	memset(buff, 0, SIZE) ;

   sock = socket(DOMAIN, SOCK_STREAM, 0);

   address.sin6_family = DOMAIN; 
   // "...only the port field needs to be formatted with htons()"
   address.sin6_port = PORT; 
   // "::1" is IPv^ "loopback" address
   inet_pton(DOMAIN, "::1", &address.sin6_addr); 

	if (connect(sock, (struct sockaddr *)&address, sizeof(address))) {
		perror("Client - conect failed.") ;
		exit(-1) ;
	}else{
		printf("client connected\n");
	}
	while (TRUE){
		read(0, buff, SIZE);
		write(sock, buff, SIZE);
		if (buff[0]=='q'){
			printf("quit\n");
			return 0;
		}
		memset(buff, 0, SIZE) ;
	}
	return 0 ;
}

int main(int argc, char const *argv[]) {
   srand(time(NULL)); // Initialization, should only be called once
   printf("%s, expects (1) arg, %d provided", argv[0], argc-1);
   if (argc == 2) {
      printf(": \"%s\".\n", argv[1]);
   } else {
      printf(".\n");
      return 1;
   }
   switch(argv[1][1]) { // indi showed me how to do this
      case 's': // start server
         server(argc, argv);
         break;
      case 'c': // start client
         client(argc, argv);
         break;
      case 'h': // help
         printf("HELP: Usage:  -s for server, -c for client\n");
         break;
      default: 
         printf("Usage: -h for help, -s for server, -c for client\n");
         break;
   }
   return 0;
}