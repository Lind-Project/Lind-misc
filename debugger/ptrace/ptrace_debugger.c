/*                                                                                                                                                                      
 * Author: Ali Gholami                                                                                                                                                  
 * Date: 2014-02-27                                                                                                                                                     
 */

#include "ptrace_debugger.h"


/* initialize a process to be traced */
pid_t init_ptrace(int argc, char** argv);

/* intercept the system calls issued by the tracee process*/
int intercept_calls(pid_t tracee);


int main(int argc, char** argv){
   
  /* check the command line arguments to see if a process is defined for trace */
  if(argc <= 0){
    fprintf(stderr, "Usage %s <program> <options>\n", argv[0]);
    exit(-1);
  }
  
  pid_t child =  init_ptrace(argc,argv);

  intercept_calls(child);

  return 0;
}

/* initialize a process to be traced */
pid_t init_ptrace(int argc, char** argv) {
  
  pid_t tracee;
  tracee = fork();

  /* check if fork was successful */
  if(tracee < 0){
    perror("No process could be initialized.");
    exit(-1);
  }

  /* trace the child */
  if(tracee == 0) {
    /* to let the parent process to trace the child*/
    ptrace(PTRACE_TRACEME, 0, NULL, NULL); 
    
    /* stop the current process*/
    kill(getpid(), SIGSTOP);

    /* the binary or command to be executed */
    execlp(argv[1], argv[1], argv[2], NULL);
  }
  return tracee;
}

/* intercept the system calls issued by the tracee process*/
int intercept_calls(pid_t tracee){
  
  int status = 0, syscall_num = 0, ret_val =0;
  struct user_regs_struct user_regs;
  
  /* to store the logging info in a file */
  FILE * fd = fopen("syscalls.log", "w");  
  if (fd == NULL) {
      printf("Error while creating the log file. \n");
      exit(-1);
    }

  /* to distinguish between normal traps and syscall traps*/
  ptrace(PTRACE_SETOPTIONS, tracee, NULL, PTRACE_O_TRACESYSGOOD);
 
  ptrace(PTRACE_SYSCALL, tracee, NULL, NULL);
  
  fprintf(fd, "syscall(number): return value \n");
  fprintf(fd, "------------------------------------\n");  

  while(1) {
    
    /* wait until child is stopped */
    wait(&status);

    /* when child is terminated */
    if(WIFEXITED(status)){
      break;
    }
      
    if(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80){
      /* get the syscall name and number */
      ptrace(PTRACE_GETREGS, tracee, NULL, &user_regs);
      syscall_num = user_regs.ORIG_REG;
      ret_val = user_regs.REG;
      fprintf(fd, "%s(%d):  %d\n", syscall_names[syscall_num], syscall_num, ret_val);
    }
    
    /* resume the child */
    ptrace(PTRACE_SYSCALL, tracee, NULL, NULL);
  }

  fflush(fd);
  fclose(fd);
  
  return 0;
}

