package com.example.administrator.myapplication;

/**
 * Created by gm on 16-9-27.
 */
import java.net.*;
import java.io.*;

public class UdpServer
{
    public static void main_UdpServer()
            throws IOException
    {
        ServerSocket ss = new ServerSocket(2345);
        while (true)
        {
            Socket s = ss.accept();
            PrintStream ps = new PrintStream(s.getOutputStream());
            ps.println("xgm");
            ps.close();
            s.close();
        }
    }
}
