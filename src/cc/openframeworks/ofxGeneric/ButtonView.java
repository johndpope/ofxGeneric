package cc.openframeworks.ofxGeneric;

public class ButtonView extends View
{
	protected android.view.View createView()
	{
		return new android.widget.Button( Activity.getInstance() );
	}
	
	public void setText( String text )
	{
		if ( mView != null ) // TODO: typecheck
		{
			android.widget.Button button = ( android.widget.Button )mView;
			button.setText( text );
		}
	}
	
	public String getText()
	{
		if ( mView != null ) // TODO: typecheck
		{
			android.widget.Button button = ( android.widget.Button )mView;
			return button.getText().toString();
		}
		return "";
	}
	
	public void setBackgroundImage()//( String name )
	{
		if ( mView != null ) // TODO: typecheck
		{
			android.widget.Button asButtonView = ( android.widget.Button )mView;
			asButtonView.setBackgroundResource( cc.openframeworks.cc.R.drawable.launch );
		}
	}
}
