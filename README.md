# Data Logger - Server Program
## 0 Wiring up the Arduino Nano IoT 33 and NEO-6M GPS Module
Connect the Arduino and Module using the following diagram:
<img src="https://raw.githubusercontent.com/edholmes2232/DataLoggerServer/master/arduino-wiring.png" width="438" height="324" />




## 1 Host Computer Setup
This software was designed to work on a Linux operating system. The following section describes how to setup a Windows 10 computer to run this software using the Windows Subsystem for Linux. The WSL runs a version of Linux called "Ubuntu". 
More info about WSL and Ubuntu can be found [here](https://wiki.ubuntu.com/WSL)

Any commands listed as `$ test command`, type directly into the Ubuntu Windows (unless specified)
Do not type the "$"

### 2 Linux SubSystem setup
1. [Follow this guide to set up Ubuntu WSL](https://ubuntu.com/wsl)
2. Ensure a username and password is set
3. Download and install gcc and make:
`$ sudo apt update && sudo apt install gcc make`
4. Download the software into your Linux system:
`$ git clone https://github.com/edholmes2232/DataLoggerServer.git`
5. Change Directory to the data logger folder:
`$ cd DataLoggerServer`
6. Compile the program, ignoring any warnings:
`$ make` 

**__CREATE WIFI NETWORK (e.g. MOBILE HOTSPOT) AND CONNECT THE COMPUTER TO IT__**

### 3 Finding the IP Address
Open Command prompt (Go to Start, and type "cmd"), and type `> ipconfig`
Note the IP address listed as IPv4 Address

### 4 Arduino Setup
The Arduino boards only need the code uploading once, the program is stored even when turned off, so uploading the code only needs to be done once per board

1. [Download the Arduino IDE](https://www.arduino.cc/en/main/software) 
3. Download the client repository to Windows: https://github.com/edholmes2232/DataLoggerClient
3. Open DataLoggerClient/Client/Client.ino in Arduino
4. Change the following lines to the WiFi network you're going to use. SSID is the network name 
```
const char *ssid     = "H1S2";
const char *password = "pw1234pw1234";
```

5. Change following line to your computer's IP Address (note the commas in place of decimal points)
`IPAddress server(192, 168, 43, 30);`
6. Plug in first Arduino board
7. Click "Install this package to use your Arduino NANO 33 IoT board" in bottom left of Arduino IDE
8. Accept driver install prompts 
9. Go to Tools > Manage Libraries
10. Search for "WiFiNINA" and install library
11. Select "Arduino NANO 33 IoT" in Tools > Board
12. Ensure the Arduino is selected in Tools > Port
13. Upload the code to the Arduino Board using the "->" arrow in top left
14. Wait until "Upload Complete" 
15. Repeat for all Nodes to use
16. Arduino units can now be unplugged from the computer and connected to external power (e.g. portable phone charger)
 
## 5 Running the Server
1. In "Ubuntu", ensure the current directory is the data logger (Step #1.5)
2. Edit the "Settings.cfg" file to list the number of Nodes to connect and enable/disable debug messages.
2.1 Using Windows Explorer:
Run in the Ubuntu terminal window `explorer.exe . ` (note the period)
Edit the Settings.cfg in Notepad (Select "Open With" and select "Notepad" from the list), Save when finished
2.2 Using the 'nano' editor:
`nano Settings.cfg`
3. To Save/Exit, pressy 'CTRL+X', 'Y' (Yes to saving the file), and 'Enter'
4. Ensure all Arduino clients are Plugged in and the Wifi Network is on
5. Run the data logger server:
`./DataLoggerServer`
6. Wait for all Nodes to connect, press "Enter" when ready to begin data logging
7. Data received is logged automatically, the program can be exited at any time and the Data will be saved
8. Exit the program by pressing "Ctrl+C"

##  (OPTIONAL) Watching the Data as it is collected
The following will show the latest 10 lines from each Node.
1. Open a second Ubuntu window (click the icon again)
2. Change Directory into the DataLoggerServer folder (Step #1.5)
3. Type the following Command 
`watch???`
4. This only watches the files, it doesn't modify the data
5. Exit using 'Ctrl+C'

## 6 Extracting Data from Linux/Ubuntu to Windows
1. Each Node saves its data in a seperate file with it's Node ID, and the date/time the program was run e.g. "Node-3-0404-12:45.txt'
2. Run `explorer.exe . `
3. Copy the "Slave-X-DDMM-HHMM.txt" files to your desired folder
4. It is recommended to remove any old Slave files from the directory after use to allow the "watch" command to work






