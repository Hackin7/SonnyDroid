package com.uselessness.sonnydroid;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Debug;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.uselessness.sonnydroid.databinding.ActivityMainBinding;

import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {
    private ScreenView screenView;
    // Used to load the 'native-lib' library on application startup.
    static { System.loadLibrary("native-lib");}

    private ActivityMainBinding binding;

    private Emulator emulator;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        screenView = (ScreenView) findViewById(R.id.screen_view) ;

        // Define Variables
        emulator = new Emulator(MainActivity.this, screenView);

        setInputs();
        openFile();

    }

    @Override
    protected void onResume() {
        super.onResume();
        screenView.resume();
    }
    @Override
    protected void onPause() {
        super.onPause();
        screenView.pause();
    }


    //// Importing ROM /////////////////////////////////////////////////////////////////////////////////
    // https://stackoverflow.com/questions/30789116/implementing-a-file-picker-in-android-and-copying-the-selected-file-to-another-l
    // Request code for selecting a PDF document.
    private static final int REQUEST_CODE = 2;
    private void openFile(){//Uri pickerInitialUri) {
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
        intent.addCategory(Intent.CATEGORY_OPENABLE);
        intent.setType("*/*");//application/pdf");
        startActivityForResult(intent, REQUEST_CODE);
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode != REQUEST_CODE || resultCode != RESULT_OK) { return; }
        emulator.importRomUri(data.getData());
        emulator.start();
    }
    //// Others ////////////////////////////////////////////////////////////////////////////////////
    public void alert(String message){
        AlertDialog.Builder s = new AlertDialog.Builder(MainActivity.this);
        s.setMessage(message);
        final AlertDialog dialog = s.create();
        dialog.show();
    }

    public void setInputs(){
        int[] buttonIds = {
                R.id.arrowUp, R.id.arrowDown, R.id.arrowLeft, R.id.arrowRight,
                R.id.btnA, R.id.btnB, R.id.btnC, R.id.btnStart
        };
        for (int i=0;i<8;i++){
            Button button = (Button) findViewById(buttonIds[i]);
            final int position = i;
            button.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch(event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            // PRESSED
                            emulator.setInput(position,true);
                            return true; // if you want to handle the touch event
                        case MotionEvent.ACTION_UP:
                            // RELEASED
                            emulator.setInput(position,false);
                            return true; // if you want to handle the touch event
                    }
                    emulator.setInput(position,false);
                    return false;
                }
            });
        }
    }
}