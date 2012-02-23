package cc.openframeworks.ofxGeneric;

import android.graphics.Rect;
import android.view.View.OnClickListener;

public class ButtonView extends View
{
	protected int mJNITouchID;
	
	protected android.view.View createView()
	{
		return new android.widget.Button( Activity.getInstance() );
	}
	
	public void Init( Rect frame )
	{
		super.Init( frame );
		if ( mView != null && mView instanceof android.widget.Button )
		{
			android.widget.Button button = ( android.widget.Button )mView;
			button.setOnClickListener( 
					new OnClickListener()
					{
						public void onClick( android.view.View view )
						{
							handleOnClick( view );
						}
					}
				);
		}
	}
	
	public void setText( String text )
	{
		if ( mView != null && mView instanceof android.widget.Button )
		{
			android.widget.Button button = ( android.widget.Button )mView;
			button.setText( text );
		}
	}
	
	public String getText()
	{
		if ( mView != null && mView instanceof android.widget.Button )
		{
			android.widget.Button button = ( android.widget.Button )mView;
			return button.getText().toString();
		}
		return "";
	}
	
	public void setBackgroundImage()//( String name )
	{
		if ( mView != null && mView instanceof android.widget.Button )
		{
			android.widget.Button asButtonView = ( android.widget.Button )mView;
			asButtonView.setBackgroundResource( cc.openframeworks.cc.R.drawable.launch );
		}
	}
	
	public void setJNITouchID( int id )
	{
		mJNITouchID = id;
	}
	
	public int getJNITouchID()
	{
		return mJNITouchID;
	}
	
	public void handleOnClick( android.view.View view )
	{
		handleOnClickNative( this, getJNITouchID() );
	}
	
	protected native void handleOnClickNative( ButtonView view, int ID );
}
