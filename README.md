# Data Logger - Server Program
## 0 Wiring up the Arduino Nano IoT 33 and NEO-6M GPS Module
Connect the Arduino and Module using the following diagram:


<img src="https://github.com/edholmes2232/DataLoggerServer/blob/master/arduino-wiring.png" width="438" height="324" />




## 1 Host Computer Setup
This software was designed to work on a Linux operating system. The following section describes how to setup a Windows 10 computer to run this software using the Windows Subsystem for Linux. The WSL runs a version of Linux called "Ubuntu". 
More info about WSL and Ubuntu can be found [here](https://wiki.ubuntu.com/WSL)



Any commands listed as `$ echo "This is a command"`, type directly into the Ubuntu Windows (unless specified) and press "Enter"
Do not type the "$". For example; for the above prompt, the terminal window will show:
```
melanie@DESKTOP-QJGOFUC:~$ echo "This is a command"
This is a command
melanie@DESKTOP-QJGOFUC:~$
```



### 2 Linux Subsystem setup
1. [Click here to set up Ubuntu WSL](https://ubuntu.com/wsl)
2. Ensure a username and password is set
3. Run the following command in the black **Ubuntu** terminal window and press "Enter" to download and install gcc and make:
`$ sudo apt update && sudo apt install gcc make`
Type "Y" and press "Enter" when prompted
4. Run the following command to download the Server software onto the "Ubuntu" system:
`$ git clone https://github.com/edholmes2232/DataLoggerServer.git`
5. Change Directory to the data logger folder:
`$ cd DataLoggerServer`
6. Compile the program, ignoring any warnings:
`$ make` 
7. Leave this window open for later
8. Create a WiFi network for the Server and Clients to connect to (Must be an WPA network)
9. Connect the laptop/computer to this network

### 3 Finding the IP Address
1. Go to Windows Search in the bottom left of the screen
2. Search for "cmd" and run "Command Prompt" program
3. In this window type `> ipconfig` and press "Enter"
4. Note down the IP address listed as IPv4 Address

### 4 Arduino Setup
The Arduino boards only need the code uploading once, the program is stored even when turned off, so uploading the code only needs to be done once per board

1. [Click here to download the Arduino IDE from the Microsoft Store](https://www.microsoft.com/store/apps/9nblggh4rsd8) 
2. Go here and click the Green "Clone or Download" button in the top left, and select Download ZIP": https://github.com/edholmes2232/DataLoggerClient
This is the Arduino Client software
3. Extract the Zip file to a seperate folder
4. In the extracted file, enter the "Client" folder and open "Client.ino" file in the Arduino IDE
5. In the code, change the following lines to the WiFi network you're going to use. SSID is the network name 
```
const char *ssid     = "H1S2";
const char *password = "pw1234pw1234";
```

5. Change following line to your computer's IP Address (note the commas in place of decimal points)
`IPAddress server(192, 168, 43, 30);`
6. Plug in first Arduino board
7. If prompted to install Drivers:
  * Click "Install this package to use your Arduino NANO 33 IoT board" in bottom left of Arduino IDE
  * Accept driver install prompts 

9. Go to Tools > Manage Libraries
10. Search for "WiFiNINA", if not installed, click install
11. Select "Arduino NANO 33 IoT" in Tools > Board
12. Ensure the Arduino is selected in Tools > Port
13. Click Sketch > Upload
14. Wait until "Done Uploading" 
15. Repeat for all Nodes to use
16. Arduino units can now be unplugged from the computer and connected to external power (e.g. portable phone charger)
 
## 5 Running the Server
1. Run in the Ubuntu terminal window `explorer.exe . ` (note the period)
Read and Edit the Settings.cfg in Notepad (Select "Open With" and select "Notepad" from the list), Save when finished

2. Ensure all Arduino clients are connected to power and the Wifi Network is on
3. In the Ubuntu window Run the following command to start the data logger server:
`$ ./DataLoggerServer`
4. Wait for all Nodes to connect, press "Enter" when ready to begin data logging
  * Run the program for as long as required
5. Exit the program by pressing "Ctrl+C"
6. Data received is logged automatically, the program can be exited at any time and the Data will be saved

##  (OPTIONAL) Watching the Data as it is collected
The following will show the latest 10 lines from each Node.
1. Open a second Ubuntu window (click the icon again)
2. Change Directory into the DataLoggerServer folder (Step #1.5)
3. Type the following Command 
`watch -n 1 tail -n 20 "Node*"`
4. This only watches the files, it doesn't modify the data
5. Exit using 'Ctrl+C'

## 6 Extracting Data from Linux/Ubuntu to Windows
1. Each Node saves its data in a seperate file with it's Node ID, and the date/time the program was run e.g. "Node-3-0404-12:45.txt'
2. Run `explorer.exe . `
3. Copy the "Node-X-DDMM-HHMM.txt" files to your desired folder
4. It is recommended to remove any old Node files from the directory after use to allow the "watch" command to work
