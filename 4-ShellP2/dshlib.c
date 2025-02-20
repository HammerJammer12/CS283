#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char cmd_buff[ARG_MAX];
    int rc = 0;
    cmd_buff_t cmd;

	 while(1){
		printf("%s", SH_PROMPT);

		if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
			printf("\n");
			break;
		}

		cmd_buff[strcspn(cmd_buff, "\n")] = '\0';
		
		//exit command
		if (strcmp(cmd_buff, EXIT_CMD) == 0){
			exit(0);
		}

		//build command list
		rc = build_cmd_buff(cmd_buff, &cmd);

		if (rc == WARN_NO_CMDS){
			printf(CMD_WARN_NO_CMD);
			continue;
		} else if (rc == ERR_TOO_MANY_COMMANDS){
			printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
			continue;
		}

		//split commands into arguments
		char *args[CMD_ARGV_MAX];
		int argc = 0;
		char *ptr = cmd.argv[0];
		
		while (*ptr) {
			while (*ptr == ' ') ptr ++;
			if (*ptr == '\0') break;

			if (*ptr == '"'){
				ptr++;
				args[argc++] = ptr;

				while (*ptr && *ptr != '"'){
					ptr++;
				}

				if (*ptr == '"'){
					*ptr = '\0';
					ptr++;
				}

				
				if (*ptr == '"') ptr++;
			
			} else {
				args[argc++] = ptr;
				while (*ptr && *ptr != ' ') ptr++;
				if (*ptr) {
					*ptr = '\0';
					ptr++;
				}
			}

				if (argc >= CMD_ARGV_MAX - 1) break;
			}

		args[argc] = NULL;

		if (argc == 0){
			continue;
		}
		

		//cd command
		if (strcmp(args[0], "cd") == 0){
			if (argc == 1){
				//missing argument do nothing
			} else {
				if (chdir(args[1]) != 0) {
					perror("cd");
				}
			}

			continue;
		}

		//fork exec
		pid_t pid = fork();
		if(pid == -1){
			perror("fork");
			continue;
		} else if (pid == 0) {
			execvp(args[0], args);
			perror("CMD_ERR_EXECUTE");
			exit(1);
		} else {
			int status;
			waitpid(pid, &status, 0);
		}

	 }

    return OK;
}


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff)
{
	if (!cmd_line || !cmd_buff){
		return ERR_CMD_OR_ARGS_TOO_BIG;
	}

	if (*cmd_line == 0){
		return WARN_NO_CMDS;
	}

	memset(cmd_buff, 0, sizeof(cmd_buff_t));

	char *cmdCopy = strdup(cmd_line);
	if(!cmdCopy){
		return ERR_CMD_OR_ARGS_TOO_BIG;
	}

	char *cmdCopySave;
	char *cmdToken = strtok_r(cmdCopy, PIPE_STRING, &cmdCopySave);
	int cmdCount = 0;

	while (cmdToken){
		while (*cmdToken == SPACE_CHAR) cmdToken++;

		char *end = cmdToken + strlen(cmdToken) - 1;
		while (end > cmdToken && *end == SPACE_CHAR){
			*end = '\0';
			end--;
		}

		if (*cmdToken == '\0'){
      	cmdToken = strtok_r(NULL, PIPE_STRING, &cmdCopySave);
      	continue;
		}


		if (cmdCount >= CMD_MAX){
			free(cmdCopy);
			return ERR_TOO_MANY_COMMANDS;
		}
		
		cmd_buff->argv[cmdCount] = strdup(cmdToken);
		if (!cmd_buff->argv[cmdCount]){
			free(cmdCopy);
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}

		cmdCount++;
		cmdToken = strtok_r(cmdCopySave, PIPE_STRING, &cmdCopySave);
	}

	cmd_buff->argc = cmdCount;

	int totalLen = 0;
	for (int i = 0; i < cmdCount; i++) {
		totalLen += strlen(cmd_buff->argv[i]) + 1;
	}

	cmd_buff->_cmd_buffer = malloc(totalLen + 1);
	if (!cmd_buff->_cmd_buffer){
		free(cmdCopy);
		return ERR_CMD_OR_ARGS_TOO_BIG;
	}

	char *buffPtr = cmd_buff->_cmd_buffer;
	for (int i = 0; i < cmdCount; i++){
		strcpy(buffPtr, cmd_buff->argv[i]);
      buffPtr += strlen(cmd_buff->argv[i]);

		if (i < cmdCount - 1){
			*buffPtr = ' ';
			buffPtr++;
		}
	}

	free(cmdCopy);
	return OK;
}

