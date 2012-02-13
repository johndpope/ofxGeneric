package cc.openframeworks.ofxGeneric;

import android.graphics.Color;
import android.graphics.Rect;
import android.opengl.Visibility;
import android.util.Log;
import android.widget.*;
import android.widget.RelativeLayout.LayoutParams;

public class View 
{
	protected android.view.View mView;
	protected android.view.ViewGroup mViewGroup;
	
	public View()
	{		
	}
	
	public void Init( Rect frame )
	{
		mView = createView();
		mViewGroup = createViewGroup();
		mViewGroup.addView( mView, createSelfLayoutParams() );
		if ( frame != null )
		{
			RelativeLayout.LayoutParams layout = new RelativeLayout.LayoutParams( frame.width(), frame.height() );
			layout.leftMargin = frame.top;
			layout.topMargin = frame.left;
			mView.setLayoutParams(  layout );
		}
		mView.setBackgroundColor( Color.WHITE );
		mView.setVisibility( android.view.View.VISIBLE );
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
	
	protected android.view.View getView()
	{
		return mViewGroup;
	}
	
//	public void setFrame()
//	public void getFrame()
	
	public void setBackgroundColor( int red, int green, int blue, int alpha )
	{
		mView.setBackgroundColor( Color.argb( alpha, red, green, blue ) );
	}
	
//	public int getBackgroundColor();
	
	public void addChildView( View child, LayoutParams params )
	{
		mViewGroup.addView( child.getView(), params );
	}
}
