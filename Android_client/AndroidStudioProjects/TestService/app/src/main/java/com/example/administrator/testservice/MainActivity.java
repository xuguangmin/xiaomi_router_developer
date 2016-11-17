package com.example.administrator.testservice;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

/**
 * Created by Administrator on 2016/11/15.
 */
public class MainActivity extends Activity
{
    Button start, stop;
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        //获取程序界面中的start,stop两个按钮
        start = (Button) findViewById(R.id.start);
        stop = (Button) findViewById(R.id.stop);
        //创建启动service的Intent
        final Intent intent = new Intent(this, FirstService.class);
        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0)
            {
                //启动指定service
                startService(intent);
            }
        });
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //停止指定service
                stopService(intent);
            }
        });
    }
}
