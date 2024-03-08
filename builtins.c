#include "builtins.h"
#include "csapp.h"
#include "jobs.h"
#include "get_path.c"

int command_jobs(int argc, char **argv);
int command_bg(int argc, char **argv);
int command_fg(int argc, char **argv);


char *which(char *command, struct pathelement *pathlist )
{
   
  //char cmd[64];
  //struct pathelement *p;

  //p = get_path();
  while (pathlist) {         // WHICH
    sprintf(command, "%s/gcc", pathlist->element);
    if (access(command, X_OK) == 0) {
      printf("[%s]\n", command);
      break;
    }
    pathlist = pathlist->next;
  }
	

	/* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

} /* which() */
/* If first arg is a builtin command, run it and return true */
int builtin_command(int argc, char **argv)
{
	    if (!strcmp(argv[0], "exit")){ /* quit command */
		            exit(0);
	    }
	    if (!strcmp(argv[0], "&")){    /* Ignore singleton & */
	       return 1;
	    }
	    if (!strcmp(argv[0], "which")){    /* Ignore singleton & */
	       //printf("test");
	       which("fs", get_path());
	       return 1;
	    }
	    if (!strcmp(argv[0], "pid")){
		printf("%d\n", getpid());
		return 1;

	    }
	    if (!strcmp(argv[0], "kill")){
		    //replace getpid() with the pid that should be killed
		    kill(getpid(), 1);
		    return 1;
	    }
	    if (!strcmp(argv[0], "prompt")){
		    //ask for a new promt
		    //take in
		    return 1;
	    }
	    if (!strcmp(argv[0], "where")){
		    //d
		    return 1;
	    }
	    if (!strcmp(argv[0], "jobs")){
		return command_jobs(argc, argv);
	    }

	    if (!strcmp(argv[0], "bg")){
		return command_bg(argc, argv);
	    }
	    if (!strcmp(argv[0], "fg")){
		return command_fg(argc, argv);
	    }
	    return 0;                     /* Not a builtin command */
}

int command_jobs(int argc, char **argv) {
	pid_t pid;
	int status;
	Job *j;
	for (j = first_job; j; j = j->next)
		printf("[%d] %d %s   %s", 
				j->jid, 
				j->pid, 
				j->stopped ? "Stopped" : "Running", 
				j->cmdline);
	return 1;
}

int command_bg(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: bg <job>\n");
		printf("    job can be either a process id or job id prefixed with %%\n");
		printf("Examples:\n");
		printf("    bg 12345  -  Runs job with process id 12345 in the background\n");
		printf("    bg %%5     -  Runs job with job id 5 in the background\n");
		return 1;
	}

	Job *j;
	if (argv[1][0] == '%') {
		unsigned jid = atoi(argv[1]+1);
		j = job_with_jid(jid);
	} else {
		pid_t pid = atoi(argv[1]);
		j = job_with_pid(pid);
	}
	if (!j) {

		fprintf(stderr, "No job with specified jid or pid\n");
		return 1;
	}

	kill(-j->pid, SIGCONT);
	j->stopped = 0;

	return 1;
}

int command_fg(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: fg <job>\n");
		printf("    job can be either a process id or job id prefixed with %%\n");
		printf("Examples:\n");
		printf("    fg 69105  -  Runs job with process id 69105 in the foreground\n");
		printf("    fg %%3     -  Runs job with job id 3 in the foreground\n");
		return 1;
	}

	Job *j;
	if (argv[1][0] == '%') {
		unsigned jid = atoi(argv[1]+1);
		j = job_with_jid(jid);
	} else {
		pid_t pid = atoi(argv[1]);
		j = job_with_pid(pid);
	}

	if (!j) {
		fprintf(stderr, "No job with specified jid or pid\n");
		return 1;
	}

	kill(-j->pid, SIGCONT);
	j->stopped = 0;

	// mark job as needing to continue in foreground
	wait_for_job(j);
	return 1;
}
				       
