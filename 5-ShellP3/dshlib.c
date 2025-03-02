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
int exec_local_cmd_loop() {
	char cmd_buff[ARG_MAX];
	int rc;
	command_list_t clist;

	while (1) {
		printf("%s", SH_PROMPT);
		if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
			printf("\n");
			break;
		}
		cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

		if (strcmp(cmd_buff, EXIT_CMD) == 0) {
			exit(0);
		}

		rc = build_cmd_list(cmd_buff, &clist);
		if (rc == WARN_NO_CMDS) {
			printf(CMD_WARN_NO_CMD);
			continue;
		} else if (rc == ERR_TOO_MANY_COMMANDS) {
			printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
			continue;
		}

		execute_pipeline(&clist);

	}
	return OK;
}

int execute_pipeline(command_list_t *clist) {
	int num_cmds = clist->num;
	int pipes[num_cmds - 1][2];
	pid_t pids[num_cmds];

	for (int i = 0; i < num_cmds; i++) {
		if (i < num_cmds - 1) {
			if (pipe(pipes[i]) == -1) {
				perror("pipe");
				return ERR_MEMORY;
			}
		}

		pids[i] = fork();
		if (pids[i] == -1) {
			perror("fork");
			return ERR_MEMORY;
		}

		if (pids[i] == 0) { 
			if (i > 0) { 
				dup2(pipes[i - 1][0], STDIN_FILENO);
			}
			if (i < num_cmds - 1) { 
				dup2(pipes[i][1], STDOUT_FILENO);
			}

			for (int j = 0; j < num_cmds - 1; j++) {
				close(pipes[j][0]);
				close(pipes[j][1]);
			}

			execvp(clist->commands[i].argv[0], clist->commands[i].argv);
			perror("execvp");
			exit(1);
		}
	}

	for (int i = 0; i < num_cmds - 1; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
	}

	for (int i = 0; i < num_cmds; i++) { 
		int status;
		waitpid(pids[i], &status, 0);
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

int build_cmd_list(char *cmd_line, command_list_t *clist) {
	if (!cmd_line || !clist) {
		return ERR_CMD_OR_ARGS_TOO_BIG;
	}

	if (*cmd_line == 0) {
		return WARN_NO_CMDS;
	}

	memset(clist, 0, sizeof(command_list_t));

	char *cmdCopy = strdup(cmd_line);
	if (!cmdCopy) {
		return ERR_CMD_OR_ARGS_TOO_BIG;
	}

	char *cmdCopySave;
	char *cmdToken = strtok_r(cmdCopy, PIPE_STRING, &cmdCopySave);
	int cmdCount = 0;

	while (cmdToken) {
		while (*cmdToken == SPACE_CHAR) cmdToken++;

		char *end = cmdToken + strlen(cmdToken) - 1;
		while (end > cmdToken && *end == SPACE_CHAR) {
			*end = '\0';
			end--;
		}

		if (*cmdToken == '\0') {
			cmdToken = strtok_r(NULL, PIPE_STRING, &cmdCopySave);
			continue;
		}

		if (cmdCount >= CMD_MAX) {
			free(cmdCopy);
			return ERR_TOO_MANY_COMMANDS;
		}

		cmd_buff_t *cmd = &clist->commands[cmdCount];
		memset(cmd, 0, sizeof(cmd_buff_t));

		char *argToken = strtok(cmdToken, " ");
		if (!argToken) {
			free(cmdCopy);
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}

		cmd->argv[0] = strdup(argToken);
		cmd->argc = 1;

		while ((argToken = strtok(NULL, " ")) != NULL) {
			if (cmd->argc >= CMD_ARGV_MAX - 1) {
				break;
			}
			cmd->argv[cmd->argc++] = strdup(argToken);
		}
		cmd->argv[cmd->argc] = NULL;

		cmdCount++;
		cmdToken = strtok_r(NULL, PIPE_STRING, &cmdCopySave);
	}

	clist->num = cmdCount;
	free(cmdCopy);

	return OK;
}
