#include "cinder/app/AppBasic.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"

#include "Resources.h"

using namespace ci;
using namespace ci::app;

class ImageFileTestApp : public AppBasic {
 public: 	
	void setup();
	void keyDown( KeyEvent event );
	
	void update();
	void draw();
	
	gl::Texture		mTexture;	
	gl::GlslProg	mShader;
	gl::Fbo			mFbo;
	float			mAngle;
};


void ImageFileTestApp::setup()
{
	try {
		mTexture = gl::Texture( loadImage( loadResource( RES_IMAGE_JPG ) ) );
	}
	catch( ... ) {
		std::cout << "unable to load the texture file!" << std::endl;
	}
	
	try {
		mShader = gl::GlslProg( loadResource( RES_PASSTHRU_VERT ), loadResource( RES_BLUR_FRAG ) );
	}
	catch( gl::GlslProgCompileExc &exc ) {
		std::cout << "Shader compile error: " << std::endl;
		std::cout << exc.what();
	}
	catch( ... ) {
		std::cout << "Unable to load shader" << std::endl;
	}
	
	mAngle = 0.0f;
	
	gl::Fbo::Format format;
	//	format.setSamples( 4 ); // uncomment this to enable 4x antialiasing
	mFbo = gl::Fbo( getWindowWidth(), getWindowHeight(), format );
	
}

void ImageFileTestApp::keyDown( KeyEvent event )
{
	if( event.getCode() == app::KeyEvent::KEY_f ) {
		setFullScreen( ! isFullScreen() );
	}
}

void ImageFileTestApp::update()
{
//	mAngle += 0.05f;
}

void ImageFileTestApp::draw()
{
	gl::clear();

	mFbo.bindFramebuffer();

	mTexture.enableAndBind();
//	setMatricesWindow(getWindowBounds()
	mShader.bind();
	mShader.uniform( "tex0", 0 );
	mShader.uniform( "sampleOffset", Vec2f( cos( mAngle ), sin( mAngle ) ) * ( 3.0f / getWindowWidth() ) );
	gl::drawSolidRect( getWindowBounds() );
	mShader.unbind();
	mTexture.unbind();
	mFbo.unbindFramebuffer();

	
	mFbo.bindFramebuffer();
	mFbo.bindTexture();
	mShader.bind();
	mShader.uniform( "sampleOffset", Vec2f( cos( mAngle + 3.1415926/2), sin( mAngle + 3.1415926/2 ) ) * ( 3.0f / getWindowWidth() ) );
	gl::drawSolidRect( getWindowBounds() );
	mShader.unbind();
	mFbo.unbindTexture();
	mFbo.unbindFramebuffer();
	
	
	mFbo.bindTexture();
	gl::drawSolidRect( getWindowBounds() );
	mFbo.unbindTexture();
	
	
}


CINDER_APP_BASIC( ImageFileTestApp, RendererGl )