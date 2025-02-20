1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: using fork/execvp allows the parent and the child processes to run concurrently. 

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**: the child process was not successfully created, and the parent must handle the failure. my
	 > implementation checks if fork() failed before proceeding and returns a -1 error in this scenario. 

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: execvp uses the environment variable PATH as the path to a directory where it looks for a given
	 > function to execute. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: The parent process needs to wait for the child process to continue before continuing, it also collects
	 > the childs PID. Not calling wait might lead to unpredictable behavior if the parent process advances before the
	 > child is completed.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: WEXITSTATUS is good for error handling and debugging by providing the exit status from a terminated
	 > child process. 

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: I manually itterate through each command, and if I encounter quotes they are "skipped" and I preserve
	 > the white space until I reach another quote, which I also skip, then return to normal parsing that removes white
	 > space. This is neccessary because quoted arguments must preserve white space.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: I had to add additional parsing to handle indiviudal arguments, as opposed to full commands seperated
	 > by pipes. I also had to write seperate parsing for quoted arguments and non-quoted arguments which proved
	 > challenging.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: signals enable control over process execution and can inform a process of useful information. other
	 > PIC methods carry data loads for processes, but signals do not, and therefore can be delivered asynchronously. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGKILL kills a process immediately. It is used when a processes is unresponsive or not responding to
	 > SIGTERM. SIGTERM Kills a process but allows it to do so "gracefully" by giving it the ability to do things like
	 > cleanup. SIGINT terminates a process, for example, pressing ctrl-c when running a program in unix.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP cannot be ignored like SIGINT as it is designed to reliably pause a process. Being caught or
	 > ignored prevents this from reliably happening
