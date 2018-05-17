package com.yglx.testjni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("native-lib");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final TextView textView = findViewById(R.id.result);
        Button notCertifyBtn = findViewById(R.id.not_certify_btn);
        Button certifyBtn = findViewById(R.id.certify_btn);
        notCertifyBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                textView.setText(getKey());
            }
        });

        certifyBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                textView.setText(getSafeKey(MainActivity.this));
            }
        });
    }
    public native String getKey();

    public native String getSafeKey(Object o);
}
