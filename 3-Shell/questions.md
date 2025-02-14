1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**: fgets stops reading at eof or a newline. We want the shell to process whatever is in standard in
	 > whenever we press "enter" and fgets allows us to do that 

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**: this makes it easier for us to find the end of cmd_buff when processing the command, if we allocated a
	 > fixed-size array we might itterate over unnecessary bytes.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: if we don't trim white space we might miss or missread arguments to our commands, causing problems 

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: our shell should have input and output redirection as well as piping. Piping creates the challenge of
	 > having to use fork to create child processes. Output redirection requires that we use open() with the appropriate
	 > flags depending on what is being redirected. Input redirection has the challenge of replacing standard in with the
	 > file descriptor.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**: pipes are used to pass the output to another program, while redirections passes the output to a
	 > seperate file or stream. 

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: They are seperate to avoid confusion between regular output and errors 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: By default STDOUT and STDERR should be kept seperate, but we should support merging them using output
	 > redirection. Errors should return exit status codes to help
	 > with debugging.  
