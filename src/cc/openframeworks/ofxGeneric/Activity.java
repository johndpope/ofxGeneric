package cc.openframeworks.ofxGeneric;

import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import cc.openframeworks.OFAndroid;

public class Activity extends android.app.Activity
{
//	static protected OFAndroid ofApp;
	static protected Activity mActivity; // TODO: can we have multiple activities of the same type at once??

	@Override
    public void onCreate(Bundle savedInstanceState)
    { 
		mActivity = this;
		
        super.onCreate(savedInstanceState);

        Activity.SetupJNI();
        Activity.Main();
/*        if ( ofApp == null )
        {
            String packageName = getPackageName();
        	ofApp = new OFAndroid( packageName, this );
        }*/
    }
	
	public static native void SetupJNI();
	
    public static native void Main(); // TODO: App requires it built, should be part of ofxGeneric

    static 
    {
    	try
    	{
    		System.loadLibrary("stlport_shared");
    	} catch( Throwable e )
    	{
    		Log.i( "OF::Generic", "Exception loading library stlport_shared: ", e );
    	}
/*    	try
    	{
    		System.loadLibrary("neondetection"); 
	    	if(hasNeon()){
	    		Log.i("OF","loading neon optimized library");
	    		System.loadLibrary("OFAndroidApp_neon");
	    	}else{
	    		Log.i("OF","loading not-neon optimized library");
	    	}
    	} catch( Throwable e ) 
    	{
    		Log.i("OF","failed neon detection, loading not-neon library",e);
    	}
*/
    	try
    	{
    		System.loadLibrary("OFAndroidApp");
    	} catch( Throwable e )
    	{
    		Log.i( "OF::Generic", "Exception loading library OFAndroidApp: ", e );
    	}
    }

    static public Activity getInstance() 
    {
    	return mActivity;
    }
	
	@Override
	public void onDetachedFromWindow() {
	}
	
    @Override
    protected void onPause() {
        super.onPause();
//        ofApp.pause();
    }

    @Override
    protected void onResume() {
        super.onResume();
//        ofApp.resume();
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
 //       OFAndroid.onKeyDown(keyCode);
        return super.onKeyDown(keyCode, event);
    }
    
    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if ((keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)) {
 //           if( OFAndroid.onBackPressed() ) return true;
 //           else return super.onKeyUp(keyCode, event);
        }
        
 //       OFAndroid.onKeyUp(keyCode);
        return super.onKeyUp(keyCode, event);
    }


    // Menus
    // http://developer.android.com/guide/topics/ui/menus.html
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	// Create settings menu options from here, one by one or infalting an xml
        return super.onCreateOptionsMenu(menu);
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
    	// This passes the menu option string to OF
    	// you can add additional behavior from java modifying this method
    	// but keep the call to OFAndroid so OF is notified of menu events
//    	if(OFAndroid.menuItemSelected(item.getItemId())){
    		
    	//	return true;
  //  	}
    	return super.onOptionsItemSelected(item);
    }
    

    @Override
    public boolean onPrepareOptionsMenu (Menu menu){
    	// This method is called every time the menu is opened
    	//  you can add or remove menu options from here
    	return  super.onPrepareOptionsMenu(menu);
    }
}



