#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "uds_helper.h"

int UNIX_PATH_MAX = 40;
int
connection_handler (int connection_fd)
{
  int nbytes;
  char buffer[MSG_SIZE];
	message *andi;
  nbytes = read (connection_fd, buffer, MSG_SIZE);
  buffer[nbytes] = 0;
	andi = (message *) buffer;
  printf ("MESSAGE FROM CLIENT: %d\n", atoi((buffer)));
  printf ("MESSAGE FROM ANDI: %s\n", andi->msg_size);
 // printf("debug: %s\n", buffer);
  int tmp;
  sprintf(andi->call_num, "%d", atoi(buffer)*3);
  printf("debug: %d\n", atoi(buffer)*2);
  printf("debug: %d\n", tmp);
  
  //nbytes = snprintf (buffer, 256, "hello from server");
  
  //sprintf(buffer, "%d", tmp);
  //write (connection_fd, buffer, nbytes);
	write(connection_fd, andi, 30);
  close (connection_fd);

  return 0;
}

int
main (void)
{
  struct sockaddr_un address;
  int socket_fd, connection_fd;
  socklen_t address_length;
  pid_t child;

  socket_fd = socket (PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0)
    {
      printf ("socket() failed\n");
      return 1;
    }

  unlink ("./../output/demo_socket");

  /* start with a clean address structure */
  memset (&address, 0, sizeof (struct sockaddr_un));

  address.sun_family = AF_UNIX;
  snprintf (address.sun_path, UNIX_PATH_MAX, "./../output/demo_socket");

  if (bind (socket_fd,
	    (struct sockaddr *) &address, sizeof (struct sockaddr_un)) != 0)
    {
      printf ("bind() failed\n");
      return 1;
    }

  if (listen (socket_fd, 5) != 0)
    {
      printf ("listen() failed\n");
      return 1;
    }

  while ((connection_fd = accept (socket_fd,
				  (struct sockaddr *) &address,
				  &address_length)) > -1)
    {
      child = fork ();
      if (child == 0)
	{
	  /* now inside newly created connection handling process */
	  return connection_handler (connection_fd);
	}

      /* still inside server process */
      close (connection_fd);
    }

  close (socket_fd);
  unlink ("./../output/demo_socket");
  return 0;
}
