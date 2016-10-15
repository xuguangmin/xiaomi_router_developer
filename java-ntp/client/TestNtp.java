
import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.ConnectException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NoRouteToHostException;
import java.net.UnknownHostException;

public class TestNtp {

    public static void main(String[] args){
        int retry = 2;
        int port = 12345;
	String ip = "192.168.31.51";
        int timeout = 3000;

	String[] ipStr = ip.split("\\.");
	byte[] ipBuf = new byte[4];
	for(int i = 0; i < 4; i++) {
		ipBuf[i] = (byte)(Integer.parseInt(ipStr[i])&0xff);
	}

        // get the address and NTP address request
        //
        InetAddress ipv4Addr = null;
        try {
            ipv4Addr = InetAddress.getByAddress(ipBuf);
	    System.out.println(ipv4Addr);
        } catch (UnknownHostException e1) {
            e1.printStackTrace();
        }

        int serviceStatus = -1;
        DatagramSocket socket = null;
        long responseTime = -1;
        try {
            socket = new DatagramSocket();
            socket.setSoTimeout(timeout); // will force the
            // InterruptedIOException

            for (int attempts = 0; attempts <= retry && serviceStatus != 1; attempts++) {
                try {
                    // Send NTP request
                    //
                    byte[] data = new NtpMessage().toByteArray();
                    DatagramPacket outgoing = new DatagramPacket(data, data.length, ipv4Addr, port);
                    long sentTime = System.currentTimeMillis();
                    socket.send(outgoing);

                    // Get NTP Response
                    //
                    // byte[] buffer = new byte[512];
                    DatagramPacket incoming = new DatagramPacket(data, data.length);
                    socket.receive(incoming);
                    responseTime = System.currentTimeMillis() - sentTime;
                    double destinationTimestamp = (System.currentTimeMillis() / 1000.0) + 2208988800.0;

                    // Validate NTP Response
                    // IOException thrown if packet does not decode as expected.
                    NtpMessage msg = new NtpMessage(incoming.getData());
                    double localClockOffset = ((msg.receiveTimestamp - msg.originateTimestamp) + (msg.transmitTimestamp - destinationTimestamp)) / 2;

                    System.out.println("poll: valid NTP request received the local clock offset is " + localClockOffset + ", responseTime= " + responseTime + "ms");
                    System.out.println("poll: NTP message : " + msg.toString());
                    serviceStatus = 1;
                } catch (InterruptedIOException ex) {
                    // Ignore, no response received.
                }
            }
        } catch (NoRouteToHostException e) {
            System.out.println("No route to host exception for address: " + ipv4Addr);
        } catch (ConnectException e) {
            // Connection refused. Continue to retry.
            e.fillInStackTrace();
            System.out.println("Connection exception for address: " + ipv4Addr);
        } catch (IOException ex) {
            ex.fillInStackTrace();
            System.out.println("IOException while polling address: " + ipv4Addr);
        } finally {
            if (socket != null)
                socket.close();
        }

        // Store response time if available
        //
        if (serviceStatus == 1) {
            System.out.println("responsetime=="+responseTime);
        }

        
    }
}

