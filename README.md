To run the code, first compile both client and server files

gcc -o client client.c
gcc -o server server.c

Then create the server directory and create all the files that you would like to keep in the server

Run the server from the server directory

<path>/server

Run the client and input the filenames that you would like to transfer from the server to the client

./client <filename1> <filename2> <filename3> ...

It will then transfer the files from the server directory to the client directory displaying the percentage of completion for each file one by one, creating files if they do not already existed and replacing them if they already do exist

Error messages are outputted for various cases:
1. If the file doesn't exist in the server directory
2. If the input is a directory and not a file
3. If the file does not have read permissions for files in the server directory
4. If the file does not have write permissions for files in the client directory
5. If the input is a directory that already exists in the client directory

The way the code works is that I send the filenames that inputted one by one from the client side to the server. The server then checks in its directory if those files do not have any issues with them (refer to the errors mentioned above). If they do, it lets the client know that there is any error and moves on to  the next file. If there isn't any issue, it then starts sending the data in file from the server to the client 1024 bytes at a time. This continues until the whole file has been sent. Then it moves onto the next file and this process repeats until all the files have been taken care of. After all the files have been taken care of, the server continues running while the client finishes. Press ctrl + c to stop the server.