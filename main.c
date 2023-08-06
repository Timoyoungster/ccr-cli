#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <ifaddrs.h>

#define SUCCESS 0
#define ERROR 1
#define START 2
#define QUIT 3
#define MAX_TOPICS 50
#define CLEAR() printf("\033[2J"); printf("\033[;H")

// region global vars

char *server_ip;
int server_port;
char *config_path;

char **topics;

// endregion global vars

// region utils

int flush_stdin() {
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
  return SUCCESS;
}

void print_ip_info() {
  printf("IP = %s ", server_ip);
  printf("Port = %d\n", server_port);
}

// endregion utils

// region game playing

// endregion game playing

// region game creation

int choose_config() {
  char path[255];
  char topic[255];
  int topicc = MAX_TOPICS;
  char **temp_topics = malloc(sizeof(char *) * topicc);
  int topic_inx = 0;
  FILE *fconfig;

  printf("Config path: ");
  scanf("%s", path);
  config_path = path;

  fconfig = fopen(path, "r");
  char c;
  int inx = 0;
  while ((c = fgetc(fconfig) != EOF)) {
    if (topic_inx >= topicc) {
      topicc += 10;
      temp_topics = realloc(temp_topics, topicc);
    }

    if (c == '\n' || (c == ' ' && inx == 0)) {
      continue;
    }

    if (c == ',') {
      topic[inx++] = '\0';
      temp_topics[topic_inx] = malloc(
          sizeof(char) * inx
        );
      strcpy(temp_topics[topic_inx++], topic);
      inx = 0;
      continue;
    }

    topic[inx++] = c;
  }

  // save last topic as well
  topic[inx++] = '\0';
  temp_topics[topic_inx] = malloc(
      sizeof(char) * inx
      );
  strcpy(temp_topics[topic_inx++], topic);

  topics = malloc(sizeof(char *) * topic_inx);
  for (int i = 0; i < topic_inx; i++) {
    topics[i] = temp_topics[i];
  }
  free(temp_topics);

  return SUCCESS;
}

int poll_and_exec_lobby_command() {
  char cmd = getchar();
  switch (cmd) {
    case 's':
      return START;
    case 'p':
      // TODO: list players
      break;
    case 'i':
      print_ip_info();
      break;
    case 'c':
      choose_config();
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
      return QUIT;
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
> ");
}

int lobby() {
  print_ip_info();

  int brk = SUCCESS;
  while (brk == SUCCESS) {
    show_lobby();
    brk = poll_and_exec_lobby_command();
    CLEAR();
  }

  // change brk to SUCCESS if the exit command was inputted
  if (brk == QUIT) {
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
  server_port = port;

  if (create_server(port) == ERROR) {
    return ERROR;
  }

  CLEAR();
  printf("Server creation successful!\n");

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
    int action = lobby();
    if (action == START) {
      // start game
    }
    return action; // is either SUCCESS or ERROR
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
