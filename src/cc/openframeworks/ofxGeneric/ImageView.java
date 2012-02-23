package cc.openframeworks.ofxGeneric;

public class ImageView extends View
{
	protected android.view.View createView()
	{
		return new android.widget.ImageView( Activity.getInstance() );
	}
	
	public void setImage()//( String name )
	{
		if ( mView != null && mView instanceof android.widget.ImageView )
		{
			android.widget.ImageView asImageView = ( android.widget.ImageView )mView;
			asImageView.setImageResource( cc.openframeworks.cc.R.drawable.launch );
		}
	}
}
