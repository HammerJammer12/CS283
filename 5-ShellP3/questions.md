1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

	The child processes will become zombies and will continue to consume resources until their parent explicitly collects
	them. My implementation avoids this by calling waitpid() in a loop for each child process.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

	Closing unused pipes prevents resource leaks, if the pipes were to remain open processes may be waiting indefinetly.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

	If cd were external, the shell would create a child process that changes its directory, that would then exit and
	leave the parent where it started, rendering it ineffective.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

 I would have to modify command_list_t to use dynamically allocated memory using realloc().
