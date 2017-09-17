******************************README file*************************************************************

!!!Please Maximize the window to make the format clear!!!


a. Name: Gao, Xiaoyu


b. Student ID: 5069169720


c. What I have done in the assignment:

	1. I create four ".c" files with name "client.c", "edge.c", "server_and.c", and "server_or.c".

		1). For the "client.c", it required to enter the file name that you are going to open
 
		    in the command line. Firstly, it establishes a TCP connection with Edge-Server with
 
		    port number: 23720. Then it open the file and read all the data in the file. After 

		    that, it sends the data to Edge-Server through the TCP connection. Finally, it 

		    receives the calculation results from the Edge-Server through the TCP connection,
 
		    and print them out on the screen.

		2). For the "edge.c", it boots up the function as a TCP server firstly. Then, it heards
	
		    the connection requirement from "client.c", so it creates a child socket to receive 

		    the data from "client.c". After that, it starts processing these data by cutting the
 
		    data into several lines.
 
		    Then, for each line, it makes a decision of which Backend-Server should this line 

		    deliever to, and forward to the corresponding Backend-Server through UDP. Next, it 

		    receives the computation result from the corresponding Backend-Server through another 

		    UDP and save this result in to the final answer array. After dealing with the first 

		    line, it then starts dealing with the next line.

		    When completing dealing with all the lines, it now have the final result array. So it

		    then send this result array back to the "client.c" through the TCP connection.

		3). For the "server_and.c", it receives the corresponding line from Edge-Server through 

		    UDP. Then, it cuts this line into three parts: "and" part, first binary number part,
 
		    and second binary part. After that, it compares the length of the first binary number

		    and the second binary number, and pad "0"s in the shorter one, and calculate logical
 
		    "and" of first and second binary number, and save the result in to the answer array.

		    Then, it will send the result back to Edge-Server through another UDP connection, and 

		    keep receiving another line from Edge_server. "server_and.c" in my program is always 

		    non-stop.

		4). For the "server_and.c", it receives the corresponding line from Edge-Server through
 
		    UDP. Then, it cuts this line into three parts: "or" part, first binary number part,
 
		    and second binary part. After that, it compares the length of the first binary number

		    and the second binary number, and pad "0"s in the shorter one, and calculate logical
 
		    "or" of first and second binary number, and save the result in to the answer array.

		    Then, it will send the result back to Edge-Server through another UDP connection, and
 
		    keep receiving another line from Edge_server. "server_or.c" in my program is always
 
		    non-stop.

	2. I create the makefile that contains six functions:
		1). By entering "make all" in the command, it will compile all of the four ".c" files.
		2). By entering "make clean" in the command, it will clean all the ".out" and ".o" files.
		3). After entering "make all", you can boot up Server AND by entering "make server_and".
		4). After entering "make all", you can boot up Server OR by entering "make server_or".
		5). After entering "make all", you can boot up Edge-Server by entering "make edge".
		6). After entering "make all", you can start the project by entering "./client <input_file
		    _name>" (For example, enter "./client job.txt" will open the file "job.txt").
	3. Finally, I write this README.txt to explain my project in detail.

d. What your code files are and what each one of them does:
	My project include 12 files, which are "job.txt", "client", "client.c", "client.o", "edge.c", 
	"edge.out", "makefile", "server_and.c", "server_and.out", "server_or.c", "server_or.out", 
	and "README.txt".
	There are four code files, which are "client.c", "edge.c", "server_and.c", and "server_or.c".
	The function of those code files can be found in part c of "README.txt".

e. What the TA should do to run the program:
	1. Enter: "make clean" to clean all the corresponding output files in the same folder. If it works,
		move to the next step.
	2. Enter: "make all" to compile all the corresponding code file. If the 5 output files show up, 
		move to the next step.
	3. Open four different terminals at the same time.
	4. Enter: "make server_and" in the first terminal to boot up Backend-Server AND. If it shows 
		"The Backend-Server AND is ready for receving data.", move to the next step.
	5. Enter: "make server_or" in the second terminal to boot up Backend-Server OR. If it shows 
		"The Backend-Server OR is ready for receving data.", move to the next step.
	6. Enter: "make edge" in the third terminal to boot up Edge-Server. If it shows "The edge server is 
		up and running.", move to the next step.
	7. Enter: "./client <input_file_name>" in the last terminal, where <input_file_name> is the name of 
		the file you are goin to test.
	8. After doing all these, you can find the output of each code file showing on each terminal.

f. The format of all the messages exchanged.
	Since I'm doing another way by sending each line seperately from Edge-Server to Backend-Server rather
	than sending all the "And" lines to Server-AND and all the "Or" lines to Server-OR as a group, there
	are some messages exchanged in Edge-Server, Server_AND, and Server-OR terminals.
	1. In Backend-Server AND terminal:
		1). I dont't have "The Server AND has successfully received <#> lines from the edge server and
		    finished all AND computations". Instead of that, I put the "This is the result of No.<#> line
		    received from edge server" just behind the "<binary number 1> and <binary number 2> = <AND 
		    computation result>".
		2). I dont't have the sentence "The Server AND has successfully finished sending all computation 
		    results to the edge server" because in my program, the Backend-Server AND is always waiting for 
		    the next line to process.
	2. In Backend-Server OR terminal:
		1). I dont't have "The Server OR has successfully received <#> lines from the edge server and
		    finished all OR computations". Instead of that, I put the "This is the result of No.<#> line
		    received from edge server" just behind the "<binary number 1> or <binary number 2> = <OR 
		    computation result>".
		2). I dont't have the sentence "The Server OR has successfully finished sending all computation 
		    results to the edge server" because in my program, the Backend-Server OR is always waiting for 
		    the next line to process.
	3. In Edge-Server terminal:
		There will only be result shown on the screen rather in form of equation "<binary number 1> and/or 
		<binary number 2> = <AND/OR computation result>".

g. Idiosyncrasy of my project:
	My project will run well in most of conditions and return an error message if any error occurs. However, 
	there is a small chance that it will be blocked because the UDP connection between Backend-Server and 
	Edge-Server is not 100% reliable. Some frame may lost during the transmission, and the server will then 
	keep waiting for the result, which will block the whole program. For this condition, just repeat all the 
	steps in "part e" of the "README.txt" file.

h. Reused Code:
	I only use some functions from Beej's book and the allowed youtube video, and I have mentioned that in the inline 
	comment in the source code.


