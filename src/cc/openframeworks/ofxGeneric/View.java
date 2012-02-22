package cc.openframeworks.ofxGeneric;

import android.graphics.Color;
import android.graphics.Rect;
import android.opengl.Visibility;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.*;
import android.widget.RelativeLayout.LayoutParams;

public class View 
{
	protected android.view.View mView;
	protected android.view.ViewGroup mViewGroup;
	
	protected int mBackgroundColor;	
	protected Rect mFrame;
	
	public View()
	{		
	}
	
	public void Init( Rect frame )
	{
		mView = createView();
		mViewGroup = createViewGroup(); // TODO: only create view group if we try to add a child?
		mViewGroup.addView( mView, createSelfLayoutParams() );
		if ( frame != null )
		{
			RelativeLayout.LayoutParams layout = new RelativeLayout.LayoutParams( frame.width(), frame.height() );
			layout.leftMargin = frame.top;
			layout.topMargin = frame.left;
			mViewGroup.setLayoutParams( layout );
		}
		mView.setBackgroundColor( Color.WHITE );
		mView.setVisibility( android.view.View.VISIBLE );
		
		mFrame = frame;
	}
	
	static public View createAndInit( Rect frame )
	{
		View view = new View();
		view.Init( frame );
		return view;
	}
	
	protected android.view.View createView()
	{
		return new android.view.View( Activity.getInstance() );
	}
	
	protected android.view.ViewGroup.LayoutParams createSelfLayoutParams()
	{		
		RelativeLayout.LayoutParams result = new RelativeLayout.LayoutParams( RelativeLayout.LayoutParams.FILL_PARENT, RelativeLayout.LayoutParams.FILL_PARENT );
		result.addRule( RelativeLayout.ALIGN_PARENT_LEFT, RelativeLayout.TRUE );
		return result;
	}
	
	protected android.view.ViewGroup createViewGroup()
	{
		return new android.widget.RelativeLayout( Activity.getInstance() );
	}
	
	protected android.view.View getAndroidView()
	{
		return mViewGroup;
	}
	
	public Rect getFrame()
	{
		return mFrame;
	}
	
	public void setFrame( Rect frame )
	{
		mFrame = frame;
		RelativeLayout.LayoutParams layout = new RelativeLayout.LayoutParams( mFrame.width(), mFrame.height() );
		layout.leftMargin = mFrame.top;
		layout.topMargin = mFrame.left;
		mViewGroup.setLayoutParams(  layout );
	}
	
	public void setBackgroundColor( int red, int green, int blue, int alpha )
	{
		mBackgroundColor = Color.argb( alpha, red, green, blue );
		mView.setBackgroundColor( mBackgroundColor );
	}
	
	public int getBackgroundColor()
	{
		return mBackgroundColor;
	}
	
	public void addChildView( View child )
	{
		RelativeLayout.LayoutParams layout = new RelativeLayout.LayoutParams( mFrame.width(), mFrame.height() );
		layout.leftMargin = mFrame.top;
		layout.topMargin = mFrame.left;
		mViewGroup.addView( child.getAndroidView(), layout );
	}
	
	public void removeChildView( View child )
	{
		mViewGroup.removeView( child.getAndroidView() );
	}
}
