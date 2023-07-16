#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>

#define SUCCESS 0
#define ERROR 1

// region global vars

char *server_ip;
int server_port;

// endregion global vars

// region utils

int flush_stdin() {
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
  return SUCCESS;
}

void print_ip_info() {
  printf("IP = %s", server_ip);
  printf("Port = %d", server_port);
}

// endregion utils

// region game playing

// endregion game playing

// region game creation

int poll_and_exec_lobby_command() {
  char cmd = getchar();
  switch (cmd) {
    case 's':
      return 2;
    case 'p':
      // TODO: list players
      break;
    case 'i':
      print_ip_info();
      break;
    case 'c':
      // TODO: choose config file
      break;
    case 'l':
      // TODO: list topics
      break;
    case 'a':
      // TODO: add topic
      break;
    case 'r':
      // TODO: remove topic
      break;
    case 'x':
      return 3;
    default:
      break;
  }

  return SUCCESS;
}

void show_lobby() {
  printf(
"Menu:\n\
  - [s]tart game\n\
  - show connected [p]layers\n\
  - show server [i]nfo\n\
  - choose [c]onfig file\n\
  - [l]ist topics\n\
  - [a]dd topic\n\
  - [r]emove topic\n\
  - e[x]it\n\
");
}

int lobby() {
  print_ip_info();

  int brk = SUCCESS;
  while (brk == SUCCESS) {
    show_lobby();
    brk = poll_and_exec_lobby_command();
    printf("\033[2J");
  }

  // change brk to SUCCESS if the exit command was inputted
  if (brk == 3) {
    brk = SUCCESS;
  }

  return brk;
}

int get_ip_info(int port) {
  struct ifaddrs *ifaddr;
  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    return ERROR;
  }

  freeifaddrs(ifaddr);
  return SUCCESS;
}

int create_server(int port) {

  return SUCCESS;
}

int create_game() {
  printf("\nCreating a new game ...\n");
  printf("Specify port for game hosting (default: 80): ");
  flush_stdin();

  // get port
  char strport[5];
  fgets(strport, 5, stdin);
  int port = strtol(strport, NULL, 10);

  if (create_server(port) == ERROR) {
    return ERROR;
  }

  printf("\033[2J");
  printf("Server creation successful!");

  if (get_ip_info(port) == ERROR) {
    return ERROR;
  }

  return SUCCESS;
}

// endregion game creation

// region game joining

int join_server() {

  return SUCCESS;
}

// endregion game joining

int main(int argc, char *argv[])
{
  printf("City, Country, River Game\n");
  printf(
"Do you want to [c]reate a new game \
or [j]oin an existing one? "
  );

  char mode = getchar();
  if (mode == 'c') {
    if (create_game() == ERROR) {
      return ERROR;
    }
    if (lobby() == ERROR) {
      return ERROR;
    }
  }
  else if (mode == 'j') {
    join_server();
  }
  else {
    printf("Unknown command. Aborting.");
    return ERROR;
  }

  return SUCCESS;
}
