/*
 * Copyright 2008 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can
 * be found in the LICENSE file.
 */

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

#define FILE_NAME  "3f23f9a0c6771d4e79fe0ea7"
#define FILE_NAME2 "SbdcO1HjzjFxn7qhtrhjyfvv"
#define FILE_NAME3 "asdfpomasaetgedkkfelesfm"
#define FILE_NAME_FSTAT "nghjuuiblkbjgzfasdasdnjk"

void check_fstat(void) {
  int file=0;
  if((file=open(FILE_NAME_FSTAT,O_RDONLY)) < -1)
    return;
 
  struct stat fileStat;
  if(fstat(file,&fileStat) < 0)    
    return;
 
  assert((int)fileStat.st_size==512);
  assert((int)fileStat.st_nlink==1);
  assert (! S_ISLNK(fileStat.st_mode)); 

}

void check_getpid(void) {
  int i;
  const int MAX_ITTER_CHECK = 10; /* Run a bunch of times, make sure the code is robust for many runs. */
  const int EXPECT_PID = 1000; 		/* 1000 is a hardcoded PID right now. */
  for (i = 0; i < MAX_ITTER_CHECK; i++) {
    int tmp = getpid();
    assert(tmp==EXPECT_PID);
  }
}

void check_file_ops(void) {
  printf("[output][glibc_test] About to open\n");
  fflush(stdout);
 
  
  FILE* f = fopen(FILE_NAME, "w");
  assert(f != NULL);
  
  char a[512] = "this is a test string. zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
  char b[512];
 
  memset(b, 0, sizeof(b));

  int size = fwrite(a, 1, strlen(a)+1, f);
  int rc = fclose(f);
  assert(rc == 0);
  f = NULL;

  /* check we can do a read. */
  f = fopen(FILE_NAME, "r");
  assert(f != NULL);
  size = fread(b, 1, 512, f);
  int cmp = strcmp(a,b);
  assert(cmp==0);
 
  fclose(f);
  f = NULL;
  f = fopen(FILE_NAME, "w");
  assert(f != NULL);

  /* check we can do a simple seek from the start of the file one forward */
  int seeked = fseek(f, 1, SEEK_SET);
  printf("[output][glibc_test] LSeek got %d \n", seeked);

  printf("[output][glibc_test] ferror = %d\n",ferror(f));
  assert(seeked==0);


  fclose(f);

}

void check_ioctl(void) {

  FILE* f = fopen(FILE_NAME, "w");
  assert(f != NULL);

  int ioctl_rc = ioctl(fileno(f), 0);
  printf("[output][glibc_test] ioctl got %d \n", ioctl_rc);
  assert(ioctl_rc==0);
  fclose(f);

}

void check_file_write(void) {
  printf("[output][glibc_test] About to write\n");
  fflush(stdout);

  FILE* f = fopen("foo2.txt", "w");
  assert(f != NULL);
  
  char * a = "This is a test string.";
  int size = fwrite(a, 1, strlen(a)+1, f);
  printf("[output][glibc_test] ferror = %d\n",ferror(f));
  

  printf("[output][glibc_test]Fwrite wrote %d\n", size);
  fflush(stdout);

  fclose(f);

}



void check_access(void) {
   printf("[output][glibc_test] Access\n");

   /* a simple file which should not exist */
   int access_test = access("afilethatdoesnotexist.txt", W_OK);
   assert(access_test == -1);

   /* check that longer path names work as well*/
   access_test = access("somevvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvverylongname.txt", R_OK);
   assert(access_test == -1);

   /* now make a file, and check it is found */
   FILE* f = fopen(FILE_NAME, "w");
   assert(f != NULL);
   fclose(f);

   access_test = access(FILE_NAME, R_OK);
   assert(access_test == 0);

}

#define LINK_FILENAME "foo3.txt"
#define LINK_DEST_1 "foo4.txt"
#define LINK_DEST_2 "foo5.txt"


void check_link(void) {
   printf("[output][glibc_test] Link\n");
   int link_test;
   link_test = link(FILE_NAME, LINK_FILENAME);
   assert(link_test == 0);
   link_test = link(FILE_NAME, "foo4.txt");
   assert(link_test == 0);
   link_test = link(LINK_FILENAME, "foo5.txt");
   assert(link_test == 0);
   
}


void check_unlink(void) {
   printf("[output][glibc_test] Unlink\n");
   int unlink_test;
   unlink_test = unlink(LINK_DEST_2);
   assert(unlink_test == 0);
   unlink_test = unlink(LINK_DEST_1);
   assert(unlink_test == 0);
   unlink_test = unlink(LINK_FILENAME);
   assert(unlink_test == 0);
   
}


void check_chdir(void) {
   printf("[output][glibc_test] Chdir\n");
   int chdir_test;
   /* Simple check */
   chdir_test = chdir("/usr/local/");
   assert(chdir_test == 0);
   chdir_test = chdir("/");
   assert(chdir_test == 0);
   chdir_test = chdir("/foo");
   assert(chdir_test == 0);
   /* Check you can go back to the same place twice */
   chdir_test = chdir("/usr/local");
   assert(chdir_test == 0);
   chdir_test = chdir("/usr/local");
   assert(chdir_test == 0);
   /* Test null string */
   /* chdir_test = chdir(""); */
   /* assert(chdir_test == -1); */
   
}

void check_mkdir(void) {
   printf("[output][glibc_test] Mkdir\n");
   int mkdir_test = mkdir("/usr/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/usr/local", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/foo", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/bar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/foobar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);
   mkdir_test = mkdir("/foobar/bar", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   assert(mkdir_test == 0);

}

void check_rmdir(void) {
   printf("[output][glibc_test] Rmdir\n");
   int rmdir_test;
   rmdir_test = rmdir("/usr/local/");
   assert(rmdir_test == 0);
   rmdir_test = rmdir("/usr/");
   assert(rmdir_test == 0);
   /* rmdir_test = rmdir("/"); */
   /* assert(rmdir_test == -1); */
}


#define DOUBLE_OPEN_FILE_NAME "double_open_file"


/* One way in which things can break is if we open the same file twice.
 *  This test case makes sure that works okay.
 */
void check_double_open(void) {
  char * a = "hello\n";
  int f = open(DOUBLE_OPEN_FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0666);
  write(f, a, strlen(a)+1);
  

  /* check double opens work */
  int f2 = open(DOUBLE_OPEN_FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0666);

  close(f2);
 
  close(f);
}


void check_two_open(void) {
  printf("Check Two Open\n");
  char * a = "hello\n";
  int f = open(DOUBLE_OPEN_FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0666);
  write(f, a, strlen(a)+1);
  printf("close1\n");
  fflush(stdout);

  close(f);

  /* check double opens work */
  int f2 = open(DOUBLE_OPEN_FILE_NAME, O_WRONLY,0);
  printf("Close 2\n");
  fflush(stdout);

  close(f2);
 
  
}


/* Make sure the getdents syscall works */
void check_readdir(void) {
  fprintf(stderr,"starting stderr\n");
  /* FILE *f = fopen("afile","w"); */
  /* fclose(f); */
  printf("Doing chdir\n");
  assert (chdir("/") == 0);
  printf("Doing Opendir\n");
  DIR* dirp = opendir(".");
  /*  perror("opening dir:"); */
  assert(dirp != NULL);
  struct dirent * dp = NULL;
  printf("Doing Readdir\n");
  while ((dp = readdir(dirp)) != NULL ) {
	printf("dp=%p d_ino = %llu, d_off= %llu d_name=%s\n", (void*)dp, dp->d_ino,dp->d_off, dp->d_name);
  }
  printf("Closedir\n");
  closedir(dirp);
}

/* make sure the statfs syscall works. Most of the values it returns are 
 hard coded though.  */
void check_statfs(void) {
    FILE* f = fopen(".", "r");
    struct statfs buf;
    int rc = fstatfs(fileno(f), &buf);

    /* printf("fstatfs: type: %x, bsize %d, blocks %ld, bfree %ld rc=%d\n", buf.f_type, buf.f_bsize, */
    /* 	   (long int)buf.f_blocks, (long int)buf.f_bfree, rc); */
    assert(rc==0);

    rc = statfs(".", &buf);

    /* printf("statfs: type: %x, bsize %d, blocks %ld, bfree %ld rc=%d\n", buf.f_type, buf.f_bsize, */
    /* 	   (long int)buf.f_blocks, (long int)buf.f_bfree, rc); */
    assert(rc==0);
    
}



/* check the dup and dup2 syscalls work 
   Right now I can't think how to test these besides making sure they return.
*/
void check_dups(void) {
  dup(12);
  dup2(15, 16);
}


/* CREAT is a special case of open, but check it here anyways.*/
void check_creat(void) {
  int rc = creat(FILE_NAME2, O_WRONLY);
  assert(rc != 0);
  close(rc);
}

/* function control currently returns zeros, but check it works anyhow.*/
void check_fcntl(void) {

  int fd = creat(FILE_NAME3, O_WRONLY);
  assert(fd != -1);
  int rc = fcntl(fd, F_GETFD, 0);
  assert(rc == 0); 		/* we don't have any flags yet. */
  
}
/* function control currently returns zeros, but check it works anyhow.*/
void check_socket(void) {

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(fd != -1);

  struct sockaddr_in ip4addr;


  ip4addr.sin_family = AF_INET;
  ip4addr.sin_port = htons(3490);
  inet_pton(AF_INET, "10.0.0.1", &ip4addr.sin_addr);

  int rc = bind(fd, (struct sockaddr*)&ip4addr, sizeof ip4addr);

  assert(rc == 0);
  shutdown(fd, SHUT_RD);
}

#define INT_FN "ints"
void screaming_files_test(void) {
  int limit = 2072576; 

  printf("Writing a file of %d bytes.\n", (limit/sizeof(int)));
  FILE* f = fopen(INT_FN, "w");
  assert(f != NULL);
  int size = 0;
  int i = 0;
  for (i = 0; i < limit; i++) {
	size = fwrite(&i, sizeof(int), 1, f);
	/* assert(size == sizeof(int)); */
  }
 
  int rc = fclose(f);
  assert(rc == 0);
  f = NULL;

  printf("Checking a file of %d bytes.\n", (limit/sizeof(int)));

  /* check we can do a read. */
  f = fopen(INT_FN, "r");
  assert(f != NULL);
  int num = 0;
  for (i=0; i<limit;i++) {
	size = fread(&num, sizeof(int), 1, f);
	assert(num == i);
  }
 
  fclose(f);
  f = NULL;
  printf("Done checking ints\n");

}


/* void test_inet_ops(void) { */
  
/*   unsigned long addr = 0; */
/*   int rc = inet_pton(AF_INET, "255.0.255.255", &addr); */
/*   assert(rc == 1); */

  
/*   printf(">> %lX\n", addr); */
/*   rc = inet_pton(AF_INET, "255.0.0.0", &addr); */
/*   assert(rc == 1); */
/*   printf(">> %lx\n", addr); */
/*   rc = inet_pton(AF_INET, "0.255.0.0", &addr); */
/*   assert(rc == 1); */
/*   printf(">> %lx\n", addr); */
/*   rc = inet_pton(AF_INET, "0.0.255.0", &addr); */
/*   assert(rc == 1); */
/*   printf(">> %lx\n", addr);  */
/*   rc = inet_pton(AF_INET, "0.0.0.255", &addr); */
/*   assert(rc == 1); */
/*   printf(">> %lx\n", addr); */

/*   printf("sockaddr = %d\n", sizeof(struct sockaddr)); */

/* } */


#define BYTE_SIZE 8
void print_sizes(void)
{

  int sizeof_int = sizeof(int) * BYTE_SIZE;
  int sizeof_long = sizeof(long) * BYTE_SIZE;
  int sizeof_ptr = sizeof(&sizeof_int) * BYTE_SIZE;

  printf("Size of int: %d, long: %d, pointer: %d\n",
         sizeof_int, sizeof_long, sizeof_ptr );
  int sizeof_uint = sizeof(unsigned int) * BYTE_SIZE;
  int sizeof_ulong = sizeof(unsigned long) * BYTE_SIZE;

  printf("Size of unsigned int: %d, unsigned long %d\n",
         sizeof_uint, sizeof_ulong);

  int sizeof_longint = sizeof(long int) * BYTE_SIZE;
  int sizeof_longlong = sizeof(long long) * BYTE_SIZE;

  printf("Size of long int: %d, long long: %d\n",
         sizeof_longint, sizeof_longlong );
}



int main() {
  print_sizes();
  /* test_inet_ops(); */
  check_two_open();

  check_double_open();
  check_getpid();
  
  check_file_ops();
  check_statfs();
  check_dups();
  check_creat();
  check_fcntl();
  check_access();

  check_two_open();

  check_fstat();
  check_file_write();
  check_mkdir();
  check_link();
  check_unlink();
  check_chdir();
  check_rmdir();
  /* check_socket(); */
  /* check_ioctl(); */
  check_readdir();

  check_two_open();
  screaming_files_test();
  printf("All tests ran sucessfully!\n");
  return EXIT_SUCCESS;

}
