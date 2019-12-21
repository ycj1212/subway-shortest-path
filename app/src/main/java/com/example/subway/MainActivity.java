package com.example.subway;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    View view;
    EditText start, end;
    Button dijkstra;
    int pointX, pointY;

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        view = findViewById(R.id.layout1);
        pointX = view.getWidth();
        pointY = view.getHeight();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        start = (EditText)findViewById(R.id.start);
        end = (EditText)findViewById(R.id.end);
        dijkstra = (Button)findViewById(R.id.dijkstra);

        dijkstra.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String s = start.getText().toString();
                String e = end.getText().toString();
                Intent intent = new Intent(MainActivity.this, Subway.class);
                intent.putExtra("출발지", s);
                intent.putExtra("도착지", e);
                intent.putExtra("가로", pointX);
                intent.putExtra("세로", pointY);
                startActivity(intent);
            }
        });

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    //public native void shortest_path(String start, String end);
}
