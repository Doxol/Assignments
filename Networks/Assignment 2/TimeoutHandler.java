import java.io.*;
import java.net.*;
import java.util.*;

public class TimeoutHandler extends TimerTask {
	
	private DatagramSocket socket;
	private DatagramPacket packet;
	private Timer timer;
	private int timeout;

	public TimeoutHandler(DatagramSocket socket, DatagramPacket packet, Timer timer, int timeout){
		this.socket = socket;
		this.packet = packet;
		this.timer = timer;
		this.timeout = timeout;
	}

	public void run(){

		try {
			socket.send(packet);
			timer.schedule(new TimeoutHandler(socket, packet, timer, timeout), timeout);
		}
		catch (Exception e){

		}
	}
}