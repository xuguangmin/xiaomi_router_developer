package com.example.administrator.myapplication;
import com.example.administrator.myapplication.NtpMessage;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.net.ConnectException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NoRouteToHostException;
import java.net.UnknownHostException;
import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Date;


public class TestNtp {

    public static void main_back(String[] args) throws IOException {
        try {
            int retry = 1;
            int port = 12345;
            double SecondsFrom1900To1970 = 2208988800.0;

            DatagramSocket socket = null;

            socket = new DatagramSocket(port);

            while(true) {
                byte[] ipbyte = new byte[32];
                byte[] data0 = new NtpMessage().toByteArray();
                // receive NTP request
                DatagramPacket incoming = new DatagramPacket(data0, data0.length);
                System.out.println("wait for request...");
                socket.receive(incoming);
                System.out.println("receive request end");

                //NTP Response
                byte[] data = new NtpMessage((byte)0, (byte)3, (byte)4, (short)5, (byte)3, (byte)0, 0.0, 0.0, ipbyte, System.currentTimeMillis() / 1000.0 + SecondsFrom1900To1970, System.currentTimeMillis() / 1000.0 + SecondsFrom1900To1970, System.currentTimeMillis() / 1000.0 + SecondsFrom1900To1970, System.currentTimeMillis() / 1000.0 + SecondsFrom1900To1970).toByteArray();

                DatagramPacket outgoing = new DatagramPacket(data, data.length, incoming.getSocketAddress());
                long sentTime = System.currentTimeMillis();
                socket.send(outgoing);

                //double localClockOffset = ((msg.receiveTimestamp - msg.originateTimestamp) + (msg.transmitTimestamp - destinationTimestamp)) / 2;

                //System.out.println("poll: valid NTP request received the local clock offset is " + localClockOffset + ", responseTime= " + responseTime + "ms");
                //System.out.println("poll: NTP message : " + msg.toString());
            }
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
}//end class

