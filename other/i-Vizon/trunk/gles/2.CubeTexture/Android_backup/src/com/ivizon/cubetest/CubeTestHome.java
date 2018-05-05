package com.ivizon.cubetest;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class CubeTestHome extends Activity {

	JNIView mView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mView = new JNIView(getApplication());
		setContentView(mView);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.cube_test_home, menu);
		return true;
	}
	@Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

}
