#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
	if (!cmd_line || !clist){
		return ERR_CMD_OR_ARGS_TOO_BIG;
	}

	if (*cmd_line == 0){
		return WARN_NO_CMDS; 
	}

	memset(clist, 0, sizeof(command_list_t));

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
      	cmdToken = strtok(NULL, PIPE_STRING);
		//	printf("DEBUF: testing cmdToken [%s}\n", cmdToken);
      	continue;
		}

		//printf("DEBUG: Processing command: [%s]\n", cmdToken);

		if (cmdCount >= CMD_MAX){
			free(cmdCopy);
			return ERR_TOO_MANY_COMMANDS;
		}

		command_t *cmd = &clist->commands[cmdCount];
		memset(cmd, 0, sizeof(command_t));

		char *argToken = strtok(cmdToken, " ");
		if (!argToken){
			free(cmdCopy);
			return ERR_CMD_OR_ARGS_TOO_BIG;
		}
		
		strncpy(cmd->exe, argToken, EXE_MAX);
		cmd->exe[EXE_MAX -1] = '\0';

		cmd->args[0] = '\0';
		char *args = cmd->args;
		int argLen = 0;

		while ((argToken = strtok(NULL, " ")) != NULL){
			int tokenLen = strlen(argToken);
			if (argLen + tokenLen + 1 >= ARG_MAX) {
				break;
			}
			if (argLen > 0) {
				args[argLen++] = ' ';
			}
			strncpy(args + argLen, argToken, tokenLen);
			argLen += tokenLen;
		}
		args[argLen] = '\0';

		cmdCount++;
		cmdToken = strtok_r(cmdCopySave, PIPE_STRING, &cmdCopySave);
	   //printf("DEBUG: END OF LOOP CMDTOKEN: [%s]\n", cmdToken);
	}

	clist->num = cmdCount;
	free(cmdCopy);

	return OK;
}
