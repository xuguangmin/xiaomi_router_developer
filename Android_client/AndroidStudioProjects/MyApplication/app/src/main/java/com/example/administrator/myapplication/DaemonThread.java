package com.example.administrator.myapplication;

import java.io.IOException;

/**
 * Created by gm on 16-9-27.
 */


public class DaemonThread extends Thread
{
    @Override
    public void run()
    {
        try{
            new TestNtp().main_back(null);
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
}