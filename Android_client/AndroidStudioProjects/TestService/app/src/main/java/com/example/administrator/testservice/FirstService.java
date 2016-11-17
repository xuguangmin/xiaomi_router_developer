package com.example.administrator.testservice;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

/**
 * Created by Administrator on 2016/11/15.
 */
public class FirstService extends Service
{
    //必须实现的方法
    @Override
    public IBinder onBind(Intent arg0)
    {
          return  null;
    }
    //service 被创建时回调该方法
    @Override
    public void onCreate()
    {
        super.onCreate();
        System.out.println("Service is Created");
    }
    //ervice 被启动时回调该方法
    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        System.out.println("Service is Started");
        return START_STICKY;
    }
    @Override
    public void onDestroy()
    {
        super.onDestroy();
        System.out.println("Service is Destroyed");
    }
}
