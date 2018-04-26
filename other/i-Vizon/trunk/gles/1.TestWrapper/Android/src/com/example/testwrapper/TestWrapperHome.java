package com.example.testwrapper;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class TestWrapperHome extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.test_wrapper_home, menu);
        return true;
    }
    
}
