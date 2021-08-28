package com.uselessness.sonnydroid;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;

import androidx.appcompat.app.AlertDialog;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;

public class Emulator {
    static { System.loadLibrary("native-lib");}

    private Context context;
    private ScreenView screenView;
    boolean inputs[]; // up, down, left, right, A, B, C, Menu
    public Emulator(Context c, ScreenView sv){
        context = c;
        screenView = sv;
        inputs = new boolean[]{false, false, false, false, false, false, false, false};
    }

    private Thread emuThread;
    public void start(byte[] rom){
        //callError("hi");
        if (emuThread == null){
            emuThread = new Thread(){
                public void run(){
                    nativeInit(rom);
                }
            };
            emuThread.start();
        }
    }
    public native void nativeInit(byte[] rom);

    // Open ROM
    private byte[] rom;
    public void importRom(byte[] input){
        rom = input;
        //openROM(rom);
    }

    public void updateScreenView(int screen[]){
        screenView.screen = screen;//getScreen();
    }
    public native int[] getScreen();

    // Error Messages
    public void callError(String message){

        AlertDialog.Builder s = new AlertDialog.Builder(context);
        s.setMessage(message);
        final AlertDialog dialog = s.create();
        dialog.show();
    }

    // Inputs
    public void setInput(int index, boolean value){
        inputs[index] = value;
        //inputs[index] = true;
    }
    public boolean[] getInputs(){
        //boolean[] ii = {true, true, true, true, true, true, true, true}; //inputs;
        //return ii;
        return inputs;
    }

}
