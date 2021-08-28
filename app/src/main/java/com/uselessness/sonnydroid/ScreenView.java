package com.uselessness.sonnydroid;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Region;
import android.os.Build;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ScreenView extends SurfaceView implements Runnable{

    private Thread thread;
    volatile boolean running;

    private int w = 320;
    private int h = 240;
    public int[] screen;

    public ScreenView(Context context) {
        this(context, null);
    }

    public ScreenView(Context context, AttributeSet attrs) {
        super(context, attrs);

        screen = new int[w*h];
        for (int i = 0; i < w*h; i++) {
            int pix = 0x80FFFF00;//255;
            if (i%w>200 || i/w> 60)pix=1022;
            screen[i] = (pix & 0xf800) << 8 |
                    (pix & 0x07e0) << 5 |
                    (pix & 0x1f) << 3;
        }
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        //getHolder().setFixedSize(320, 240);
    }


    @Override
    public void run() {
        while(running) {
            draw();
        }
    }

    private void draw(){

        if(getHolder().getSurface().isValid()) {
            Canvas canvas = getHolder().lockCanvas();

            canvas.drawColor(Color.argb(255, 155, 0, 0));

            getHolder().setFixedSize(w, h);
            canvas.drawBitmap(screen, 0, w, 0, 0, w, h, false, null);

            getHolder().unlockCanvasAndPost(canvas);
        }
    }


    public void resume() {
        running = true;
        thread = new Thread(this);
        thread.start();
    }

    public void pause() {
        running = false;

        while (thread.isAlive()) {
            try {
                thread.join(100);

                if(thread.isAlive()) {
                    thread.interrupt();
                    thread.join();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
