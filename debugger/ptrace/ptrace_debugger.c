/*                                                                                                                                                                      
 * Author: Ali Gholami                                                                                                                                                  
 * Date: 2014-02-27                                                                                                                                                     
 */

#include "ptrace_debugger.h"

int main(int argc, char** argv){
  
  struct user_regs_struct uregs;
  int status = 0, syscall_num = 0;

  /* to store the logging info in a file */
  FILE * fd = fopen("logs.txt", "w");  
  
  pid_t child = fork();
  
  /* check the command line arguments to see if a process is defined for trace */
  if(argc <= 0){
    fprintf(stderr, "Usage %s <program>\n", argv[0]);
    exit(-1);
  }
  
  /* check if fork was successful */
  if(child < 0){
    perror("No process could be initialized.");
    exit(-1);
  }

  if (fd == NULL) {
      printf("Error while creating the log file. \n");
      exit(-1);
    }


  /* trace the child */
  if(child == 0) {
    ptrace(PTRACE_TRACEME);
    /* The actual program to be traced */
    execl(argv[1], argv[1], NULL);
    exit(1);
  }
  
  /* trace the parent */
  ptrace(PTRACE_SYSCALL, child, NULL, NULL);
  
  fprintf(fd, "List of the syscalls invoked by: %s \n", argv[1]);
  fprintf(fd, "------------------------------------\n");
  
  /* trace the syscalls */  
  while(1) {
  
    wait(&status);

    /* when child is terminated */
    if(WIFEXITED(status)){
      exit (1);
    }
      
    if(WSTOPSIG(status)){
      /* get the syscall name and number */
      ptrace(PTRACE_GETREGS, child, NULL, &uregs);
      syscall_num = uregs.ORIG_REG;
      fprintf(fd,"%s  %d\n",  syscall_names[syscall_num], syscall_num);
      ptrace(PTRACE_SYSCALL, child, NULL, NULL);
      wait(&status);
      
    }
     ptrace(PTRACE_SYSCALL, child, NULL, NULL);
  }
  fclose(fd);
  return 0;
}
