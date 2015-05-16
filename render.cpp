#include "headers.h"
GLuint m_texStar;
GLFWwindow* window;

using namespace std;

std::default_random_engine generator;
std::normal_distribution<float> maxwell(0.0,1.0);

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
}

void initGLFW()
{
	glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(2560, 1440, "Colliding Galaxy Simulation", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // make current window OPENGL context or else OPENGL will not work
    glfwMakeContextCurrent(window);

    // time interval at which the render buffer(back buffer) is brought to the front buffer
    glfwSwapInterval(1);

    //terminate when escape-key is pressed
    glfwSetKeyCallback(window, key_callback);
}

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  // glEnable(GL_BLEND);
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void initPointSpriteExt()
{

	SDL_Surface *tex;

	   // texture loading taken from
	// http://gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL
	  tex = SDL_LoadBMP("particle.bmp");

	  // get the number of channels in the SDL surface
	  GLint  nOfColors = tex->format->BytesPerPixel;
	  GLenum texture_format;
    if (nOfColors == 4)     // contains an alpha channel
	  {
	    if ( tex->format->Rmask == 0x000000ff)
	      texture_format = GL_RGBA;
	    else
	      texture_format = GL_BGRA;
	  }
	  else if (nOfColors == 3)     // no alpha channel
	  {
	    if ( tex->format->Rmask == 0x000000ff)
	      texture_format = GL_RGB;
	    else
	      texture_format = GL_BGR;
	   }

	  // Have OpenGL generate a texture object handle for us
		glGenTextures(1, &m_texStar);

	  // Bind the texture object
		glBindTexture( GL_TEXTURE_2D, m_texStar );

		// Set the texture's stretching properties
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D(GL_TEXTURE_2D,
      0,
      nOfColors,
      tex->w,
      tex->h,
      0,
      texture_format,
      GL_UNSIGNED_BYTE,
      tex->pixels );

}

extern "C" void Pre_Render()
{
	initGLFW();
	initGL();
	initPointSpriteExt();
}

extern "C" void Render(float* coord, int num, int frame)
{
	col color;
	int width, height;

  // set colors
  color.r = 0.5; 
  color.g = 0.5; 
  color.b = 0.8; 

  // start rendering

  float maxSize = 0.0f;

  glfwGetFramebufferSize(window, &width, &height); // not important

  glViewport(0, 0, width, height);  // view settings

  glLoadIdentity();

  glMatrixMode( GL_MODELVIEW );

   // gluPerspective(2, width/height,100, 1000);

  glScalef(0.05, 0.05, 0.05);
   //glEnable(GL_NORMALIZE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, m_texStar);

  glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
  // glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, maxSize);
   //glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, 1.0f);
  glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

  glEnable(GL_POINT_SPRITE_ARB);
  glEnable(GL_TEXTURE_2D);       // point sprite texture support
  glEnable(GL_BLEND);            // soft blending of point sprites
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Render a color-cube consisting of 6 quads with different colors

   // glMatrixMode(GL_MODELVIEW);
  glBindTexture(GL_TEXTURE_2D, m_texStar);

  glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
  //glPointParameterfARB(GL_POINT_SIZE_MAX_ARB, maxSize);
  //glPointParameterfARB(GL_POINT_SIZE_MIN_ARB, 1.0f);
  glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

  glEnable(GL_POINT_SPRITE_ARB);
  glEnable(GL_TEXTURE_2D);       // point sprite texture support
  glEnable(GL_BLEND);            // soft blending of point sprites
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);


  //glLoadIdentity();

  glPointSize(5); //pStars[i].m_mag*10);
    glBegin(GL_POINTS);


  for (int i=0; i<3*num; i+=3){
    glPushMatrix();
    glTranslatef(coord[i], coord[i+1], coord[i+2]);
    glColor3f(color.r,color.g,color.b);
    glVertex3f(coord[i], coord[i+1], coord[i+2]);
    glPopMatrix();
  }
  
  glEnd();
  
  // uncomment next line to output the frames
  //screenshot(width,height,frame);

  glDisable(GL_POINT_SPRITE_ARB);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  glfwSwapBuffers(window);
  glfwPollEvents();

}

extern "C" void Post_Render()
{
	glDeleteTextures(1,&m_texStar);
	glfwDestroyWindow(window);
    glfwTerminate();
}

// get image data
void screenshot (int x, int y, int frame){
  std::stringstream ss;
  ss << "plot" << frame << ".tga";
  std::string filename = ss.str();

  long imageSize = x * y * 3;
  unsigned char *data = new unsigned char[imageSize];
  glReadPixels(0,0,x,y, GL_BGR,GL_UNSIGNED_BYTE,data);// split x and y sizes into bytes
  int xa= x % 256;
  int xb= (x-xa)/256;int ya= y % 256;
  int yb= (y-ya)/256;//assemble the header
  unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(unsigned char)xa,
    (unsigned char)xb,(unsigned char)ya,(unsigned char)yb,24,0};

  // write header and data to file
  std::fstream File(filename, ios::out | ios::binary);
  File.write (reinterpret_cast<char *>(header), sizeof (char)*18);
  File.write (reinterpret_cast<char *>(data), sizeof (char)*imageSize);
  File.close();
  delete[] data;
  data=NULL;
}
