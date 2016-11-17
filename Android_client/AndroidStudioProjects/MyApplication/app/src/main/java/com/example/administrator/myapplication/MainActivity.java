package com.example.administrator.myapplication;

        import android.content.Intent;
        import android.support.v7.app.AppCompatActivity;
        import android.os.Bundle;


        import android.os.Bundle;
       // import android.support.design.widget.FloatingActionButton;
        //import android.support.design.widget.Snackbar;
        import android.support.v7.app.AppCompatActivity;
        import android.support.v7.widget.Toolbar;
        import android.view.View;
        import android.view.Menu;
        import android.view.MenuItem;
        import android.widget.TextView;

        import  com.example.administrator.myapplication.UdpServer;
        import com.example.administrator.myapplication.DaemonThread;
        import com.example.administrator.myapplication.TestNtp;
        import com.example.administrator.myapplication.FirstService;

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



        import android.app.Activity;

        import android.widget.Button;
        import android.widget.*;


public class MainActivity extends AppCompatActivity {
    Button button;
    Button start, stop;
    FirstService service;
    TextView textView;
    int count = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        /*Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });
        button = (Button)findViewById(R.id.button);
        textView = (TextView)findViewById(R.id.textView);
		*/

        /*
        DaemonThread t = new DaemonThread();
        t.setDaemon(true);
        t.start();
        System.out.println("app run success!");
*/

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
/*
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
*/

    public void BtnClick(View view)
    {
        String str = "---->" + count;
        textView.setText(str);


        count++;
      /*  try {
            new UdpServer().main_UdpServer();
        }catch (IOException e) {}*/
    }

}



/*
public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}
*/