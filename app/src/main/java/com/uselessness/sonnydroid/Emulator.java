package com.uselessness.sonnydroid;

import android.app.Activity;
import android.content.Context;
import android.net.Uri;

import androidx.appcompat.app.AlertDialog;

import java.io.ByteArrayOutputStream;
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
    public void start(){
        //callError("hi");
        if (emuThread == null){
            emuThread = new Thread(){
                public void run(){
                    rom = getRomBytes(0,0);
                    nativeInit(rom);
                }
            };
            emuThread.start();
        }
    }
    public native void nativeInit(byte[] rom);

    // Open ROM ////////////////////////////////////////////////////////////////////////////////////
    private byte[] rom;
    private Uri romUri;
    public void importRomUri(Uri u){romUri = u;}
    public byte[] getBytes(InputStream inputStream) throws IOException {
        ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();
        int bufferSize = 1024;
        byte[] buffer = new byte[bufferSize];

        int len = 0;
        while ((len = inputStream.read(buffer)) != -1) {
            byteBuffer.write(buffer, 0, len);
        }
        return byteBuffer.toByteArray();
    }
    //Called from native function
    public byte[] getRomBytes(int offset, int len_out) {
        try {
            InputStream inputStream = context.getContentResolver().openInputStream(romUri);

            //inputStream.skip(offset);
            ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();
            int bufferSize = 1024;
            byte[] buffer = new byte[bufferSize];

            int curr_len = 0;//curr_len <= len_out &&
            int len = 0;
            while ((len = inputStream.read(buffer)) != -1) {
                byteBuffer.write(buffer, 0, len);
                //curr_len += len;
            }
            inputStream.close();
            System.gc();
            return byteBuffer.toByteArray();
        }catch (FileNotFoundException ex) {callError("File Not Found");}
        catch (IOException ex) {callError("IOException");}
        return null;//new byte[len_out];
    }

    // ScreenView //////////////////////////////////////////////////////////////////////////////////
    public void updateScreenView(int screen[]){
        screenView.setScreen(screen);
    }
    public void updateScreenViewPixel(int index, int pixel){
        screenView.setScreenPixel(index, pixel);
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
