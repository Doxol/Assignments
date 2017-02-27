/**
 * FastFTP Class
 * FastFtp implements a basic FTP application based on UDP data transmission and 
 * alternating-bit stop-and-wait concept
 * @author      XYZ
 * @version     1.0, 1 Feb 2017
 * David Sepulveda
 * 10161931
 */

import java.io.*;
import java.net.*;
import java.util.*;

public class FTPClient {

	public DatagramSocket serverUDPSocket;
	public Socket serverTCPSocket;
	
	public String server_name;
	public String file_name;

	public static int port;
	public static int timeout;

	/**
	 * Constructor to initialize the program 
	 * 
	 * @param serverName	server name
	 * @param server_port	server port
	 * @param file_name		name of file to transfer
	 * @param timeout		Time out value (in milli-seconds).
	 */
	public FTPClient(String server_name, int server_port, String file_name, int timeout) {

	/* Initialize values */
		this.server_name = server_name;
		this.file_name = file_name;
		this.timeout = timeout;
		try {
			// Listen on port 8888
			serverTCPSocket = new Socket(server_name, server_port);

			serverUDPSocket = new DatagramSocket();
		}
		catch (Exception e) {
			if (serverTCPSocket != null){
				try {
					serverTCPSocket.close();
				}
				catch (Exception ex) {
					System.out.println("TCP socket ran into problems.");
				}
			}
			if (serverUDPSocket != null){
				try {
					serverUDPSocket.close();
				}
				catch (Exception ex) {
					System.out.println("UDP socket ran into problems.");
				}
			}
		}
	}


	/**
	 * Start with TCP handshake
	 * If response is 0, start UDP sending
	 * Read the file into 1000 byte buffers, and create segments out of these buffers
	 * Send these buffers to the server and wait for a response. Schedule a timeout to send
	 * the segment again if 50 ms pass with no response. Once we receive a response, ensure
	 * that the server responed to the correct sequence code. If yes, send the next
	 * segment. Continue until the file has been sent. On the last segment, the buffer may be too 
	 * large for the information we want to send. If this is the case, create a new buffer that is
	 * the size of the remaining file contents and create a segment from this.
	 * Send file content as Segments
	 * 
	 */
	public void send() {

		// send logic goes here. You may introduce addtional methods and classes
		try{
			DataOutputStream serverTCPOutputStream = new DataOutputStream(serverTCPSocket.getOutputStream());
			DataInputStream serverTCPInputStream = new DataInputStream(serverTCPSocket.getInputStream());
		
			serverTCPOutputStream.writeUTF(file_name);

			byte b = serverTCPInputStream.readByte();

			// if server is ready
			if (b == 0){
				// open file
				FileInputStream fileInput = new FileInputStream(file_name);
				int chunk_size = Segment.MAX_PAYLOAD_SIZE;
				byte[] chunky = new byte[chunk_size];
				byte[] receiveChunky = new byte[Segment.MAX_SEGMENT_SIZE];


				Segment segment;
				// adress for UDP sending
				InetAddress inetAddress = InetAddress.getByName("localhost");
				// packet to send
				DatagramPacket packet;
				// server's response
				DatagramPacket receivePacket;

				int returned = 0;
				int seqNum = 0;
				int ackNum;

				Segment receiveSeg;
				
				// timer for scheduling timeouts in case server drops packets
				Timer timer = new Timer(true);

				while (true) {

					returned = fileInput.read(chunky);
					// if EOF, break
					if (returned == -1)
						break;

					// if the information to send is too small
					if (returned < chunk_size){
						chunky = Arrays.copyOf(chunky, returned);
					}
					// create the segment to send
					segment = new Segment(seqNum, chunky);
					// create the packet to send
					packet = new DatagramPacket(segment.getBytes(), segment.getBytes().length, inetAddress, port);

//hi :)				
					// send the packet using the socket we opened
					serverUDPSocket.send(packet);
					// schedule a timeout to resend packet if the server drops it
					timer.schedule(new TimeoutHandler(serverUDPSocket, packet, timer, timeout), timeout);

					// until server has returned an appropriate ack, keep waiting for response
					while (true){

						// server response
						receivePacket = new DatagramPacket(receiveChunky, receiveChunky.length);

						serverUDPSocket.receive(receivePacket);

						receiveSeg = new Segment(receivePacket);

						// segment the server is responding to
						ackNum = receiveSeg.getSeqNum();

						// if server is responding to correct packet
						if (seqNum == ackNum){
							try {
								// cancel any timers still scheduled
								// would cancel teh timeout handler, but it may have already finished
								// created a new one, so it is better to cancel everything
								timer.cancel();
								timer = new Timer(true);
							}
							catch (Exception e){
							}
							break;
						}

					}


					seqNum = (seqNum + 1)%2;
				}
				serverTCPOutputStream.writeByte(0);




//else statement
			}
			else {
				System.out.println("Error: server is not ready to receive file content.");
			}

		}
		catch (Exception e) {
			System.out.println(e.getMessage());
		}
		finally{
			// finally close everything if it has not been closed already
			if (serverTCPSocket != null){
				try {
					serverTCPSocket.close();
				}
				catch (Exception ex) {
					System.out.println("print3");
					// ignore
				}
			}
			if (serverUDPSocket != null){
				try {
					serverUDPSocket.close();
				}
				catch (Exception ex) {
					System.out.println("print4");
					// ignore
				}
			}
		}
	}



	/**
	 * A simple test driver
	 * 
	 */
	public static void main(String[] args) {


		String server = "localhost";
		String file_name = "";
		int server_port = 8888;
		port = server_port;
		timeout = 50; // milli-seconds (this value should not be changed)


		// check for command line arguments
		if (args.length == 3) {
			// either provide 3 parameters
			server = args[0];
			server_port = Integer.parseInt(args[1]);
			file_name = args[2];
		}
		else {
			System.out.println("Wrong number of arguments, try again.");
			System.out.println("usage: java FTPClient server port file");
			System.exit(0);
		}


		FTPClient ftp = new FTPClient(server, server_port, file_name, timeout);

		System.out.printf("sending file \'%s\' to server...\n", file_name);
		ftp.send();

		System.out.println("file transfer completed.");
	}
}