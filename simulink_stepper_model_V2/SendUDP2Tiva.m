%Title: Clemson University ECE 4960 UDP-MatlabToTiva_Demo
%Author: Reinaldo Gift
%Date: 04/06/2016
%Description: Demo showing how to communicate between MATLAB and Tiva via
%UDP and the Tiva's UART

instrreset; %runs stopasync, fclose('all') and delete to close all connections
% edit comm server config so that Virtual Port vport
% is used to send/receive, set the packet size to msg_size below.
%Set up incoming UDP port number. Ports taking data coming from Tiva to PC 
%are on the default port: port number 12400 + VirtualPortNumber
vport_in = 2; %choose port number 12402

%Set up outgoing UDP port. Ports sending data to Tiva from PC are on the 
%default port: port number 12300 + VirtualPortNumber
vport_out = 3; %choose port number 12303

%Set up two UDP ports: One port to send, one port to receive.
%The remote address is localhost, which is also known by the IP address
%'127.0.0.1'. The UDP port number is the default Tiva Port as described
%above
%Opening a UDP port is similar to opening a file. Note the Timeout parameter.
%The usend port will drop packets after 10 seconds of trying
usend = udp('127.0.0.1', 12300+vport_out, 'Timeout', 10); %Port sending from PC to Tiva
%Note below the LocalPort parameter. If you don't set this up also, it won't work. 
urec = udp('127.0.0.1', 12400+vport_in, 'LocalPort', 12400+vport_in, 'Timeout', 10); %Port receiving from Tiva to PC
%Open the two UDP ports for reading/writing
fopen(usend);
fopen(urec);
%initialize the data to send
A = [uint8(1), uint8(255)]; %The data to be sent to Tiva
%Set up the size of the message send. Remember to edit your setting.ini
%file as described below. Also change your MPComRecv and MPComSend block as need
%to match the size of the data being send and received
msg_size = [1, 2]; %The size of message being sent from Tiva, arriving at PC: 1 row, 2 columns large

%Send message forever
while(1)
 fwrite(usend, A,'uint8'); %send data, of type unint8
 pause(0.1) %wait a bit before reading
 A=fread(urec, msg_size, 'uint8'); %read data comming from Tiva to PC: a 1x2 matrix of uint8s
 A %display A: The data
 
 %Have the first data byte count up
 if(A(1,1) == uint8(255))
     A(1,1) = 0;
 else
    A(1,1) = A(1,1) + uint8(1);
 end
 
  %Have the second data byte count down
  if(A(1,2) == uint8(0))
     A(1,2) = 255;
 else
    A(1,2) = A(1,2) - uint8(1);
 end

end

%Close sockets when done or you will have to restart matlab to close the
%sockets
fclose(usend)
fclose(urec)

%Notes: 

%***Use putty and the Tiva COM server to see what's being receive on the
%Tiva side.
%***Use MATLAB command window to see what's being received on the PC side
%***Remember to set up your setting.ini file correctly as described below.
%I have my example settings below also
%Change your serial port number to the appropriate COM port of the Tiva
%Comment out any unused UDP ports or they may stall your program
%Remember that when you set up a UDP port, your size = number of bytes
%being sent or received on that port.
%***Run your Tiva in Normal mode
%"***My solver options are fixed step-size of 0.01. I have seen faster 
%sampling times cause people's Tivas to freeze. i.e. Stop displaying stuff 
%on putty after the first iteration.
%***If you system is still not working, try disabling your firewall for the
%first run of the program. Sometimes the firewall can block UDP
%connections. You can also try disabling your antivirus as well since some
%antivirus's include firewalls.
%***If Tiva lights are not blinking something is wrong
%***On the Tiva side Block Diagram, it is the blink demo with UArt Blocks.
%MPComRecv receives data from what is being writting by MATLAB over UDP.
%The data is then demuxed into two uint8's and then converted to unit32s 
%using 'Data Type Conversion'for display using %u in the Advanced Debug 
%Print Block. Meanwhile the raw data from MPComRecv is fed back to 
%MPComSend as 2 bytes so that MATLAB can read the data, manipulate it, and 
%send it out again. 
%***If you are experiencing delays on your transfers, you may be sending or
%receiving too quickly and the information is maintained in the
%communication queue. You would want to set resonable sample times on your
%MPComSend block especially. Also examing the MPComRecv sample time as well
%as the pauses in between fwrite and fread.

%Put Matlab's directory to: 'C:\MATLAB\SupportPackages\Tiva_TM4C123G\utils'
% Run setting.ini file set using command:
% runStellarisLPCmd('editcommsvrcfg')
% Use: runStellarisLPCmd('runcommsvr')
% to run
% 
% 
% [SERIAL]
% PORT=7
% BAUD=1000000
% 
% [PORT0] #default port for external mode
% TYPE=TCP
% AUTORESET=1
% 
% [PORT1] #default port for debug print
% TYPE=TCP
% 
% # Example tcp connection
% # incomming data from serial route to tcp port 12200+n
% # outgoing data to serial routed from tcp port 12200+n
% #[PORTn]
% #TYPE=TCP
% #AUTORESET=1
% 
% # Example udp connection
% # incomming data from serial route to udp port 12400+n
% # outgoing data to serial routed from udp port 12300+n
% #[PORTn]
% #TYPE=UDP
% #SIZE=6
% 
% # the following port setting is for LightRemoterDemo
% #[PORT2]
% #TYPE=TCP
% 
% # the following port setting is for LightRemoterDemoUDP
% #[PORT2]
% #TYPE=UDP
% #SIZE=4
% 
% #########################################################
% # the following port setting is for the UDPMatlab demo
% #########################################################
% #=================#
% # From Tiva to PC #
% #=================#
% [PORT2] 
% TYPE=UDP
% SIZE=2
% #=================#
% # From PC to Tiva #
% #=================#
% [PORT3]
% TYPE=UDP
% SIZE=2
% ##########################################################
% 


