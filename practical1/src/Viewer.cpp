#include "./../include/Viewer.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <iomanip>

static const Viewer::Duration g_modeInformationTextTimeout = std::chrono::seconds( 3 );

static const std::string screenshot_basename = "screenshot";

static void initializeGL()
{
  //Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if( GLEW_OK != err )
      LOG( error, "[GLEW] " << glewGetErrorString(err) );
  LOG( info, "[GLEW] using version " << glewGetString( GLEW_VERSION ) );

  //Initialize OpenGL context
  glcheck(glClearColor(0.8f,0.8f,0.8f,1.0f));
  glcheck(glEnable(GL_DEPTH_TEST));
  glcheck(glDepthFunc(GL_LESS));
  glcheck(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
  glcheck(glEnable(GL_TEXTURE_2D));
}

Viewer::KeyboardState::KeyboardState()
  : forward( false ), backward( false ), left( false ), right( false ),
    slow( false ), fast( false ), speed( 0.5 )
{}

Viewer::~Viewer()
{}

Viewer::Viewer(float width, float height) :
    m_window{
      sf::VideoMode(width, height),
      "Computer Graphics Practicals",
      sf::Style::Default,
      sf::ContextSettings{ 24 /* depth*/, 8 /*stencil*/, 4 /*anti aliasing level*/, 4 /*GL major version*/, 0 /*GL minor version*/}
    },
    m_startTime{},
    m_modeInformationTextDisappearanceTime{ clock::now() + g_modeInformationTextTimeout },
    m_modeInformationText{ "Arcball Camera Activated" },
    m_applicationRunning{ true }, m_animationLoop{ false }, m_animationIsStarted{ false },
    m_loopDuration{0}, m_screenshotCounter{0}, m_helpDisplayed{false},
    m_lastEventHandleTime{ clock::now() }
{
  sf::ContextSettings settings = m_window.getSettings();
  LOG( info, "Settings of OPENGL Context created by SFML");
  LOG( info, "\tdepth bits:         " << settings.depthBits );
  LOG( info, "\tstencil bits:       " << settings.stencilBits );
  LOG( info, "\tantialiasing level: " << settings.antialiasingLevel);
  LOG( info, "\tGL version:         " << glGetString( GL_VERSION ) );
  LOG( info, "\tGL renderer:        " << glGetString( GL_RENDERER ) );

  //Initialize the camera
  float ratio = width/height;
  m_camera.setRatio(ratio);

  //Set up GLEW
  initializeGL();

  //Initialize the text engine (this SHOULD be done after initializeGL, as the text
  //engine store some data on the graphic card)
  m_tengine.init();
  m_tengine.setWindowDimensions( m_window.getSize().x, m_window.getSize().y );
}

static const std::string g_help_message =
    "VIEWER SHORTCUTS:\n"
    "      [F1]  Display/Hide this help message\n"
    "      [F2]  Take a screen shot of the frame currently in the frame buffer\n"
    "      [F3]  Reload all managed shader program from their sources\n"
    "       [c]  Switch the camera mode between Arcball / Space ship\n"
    "[ctrl]+[w]  Quit the application\n"
    "\n"
    "CAMERA CONTROL:\n"
    "  Right click to change the camera orientation\n"
    "  * Arcball mode\n"
    "    Use the wheel to change the zoom\n"
    "\n"
    "  * Spaceship mode\n"
    "    [q] to go left         [d] to go right\n"
    "    [z] to go forward      [z] to go backward\n"
    "    [LShift] to go slower  [Space] to go faster\n"
    ;

void Viewer::draw()
{
  glcheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  for(RenderablePtr r : m_renderables)
  {
    r->bindShaderProgram();
    glcheck(glUniformMatrix4fv(r->projectionLocation(), 1, GL_FALSE, glm::value_ptr(m_camera.projectionMatrix())));
    glcheck(glUniformMatrix4fv(r->viewLocation(), 1, GL_FALSE, glm::value_ptr(m_camera.viewMatrix())));
    r->draw();
    r->unbindShaderProgram();
  }

  //Refresh the viewer.m_window
  if( clock::now() < m_modeInformationTextDisappearanceTime )
    {
      m_tengine.render( m_modeInformationText, glm::vec2(10, m_window.getSize().y - 30), glm::vec3(0.1, 0.1, 0.1) );
    }
  {
    std::ostringstream ss;
    ss << "FPS: " << std::setprecision( 2 ) << std::fixed << m_fpsCounter.getFPS();
    m_tengine.render( ss.str(), glm::vec2(m_window.getSize().x - 200, m_window.getSize().y - 30), glm::vec3(0.1,0.1,0.1) );
  }
  if( m_helpDisplayed )
    m_tengine.render( g_help_message, glm::vec2(100, 650), glm::vec3{.0, .1, .2});
}

float Viewer::getTime()
{
    Duration duration = clock::now()-m_startTime;
    return duration.count();
}

void Viewer::animate()
{
    if(m_animationIsStarted)
    {
        if(m_animationLoop && getTime()>=m_loopDuration)
            resetAnimation();

        for(RenderablePtr r : m_renderables)
            r->animate( getTime() );

        m_camera.animate( getTime() );
    }
}

void Viewer::setAnimationLoop(bool animationLoop, float loopDuration)
{
    m_animationLoop = animationLoop;
    m_loopDuration = loopDuration;
}

void Viewer::startAnimation()
{
    m_startTime = clock::now();
    m_animationIsStarted=true;
}

void Viewer::stopAnimation()
{
    m_animationIsStarted=false;
}

void Viewer::resetAnimation()
{
    m_startTime = clock::now();
}

void Viewer::addRenderable(RenderablePtr r)
{
    m_renderables.insert(r);
    r->m_viewer = this;
}

void Viewer::keyPressedEvent(sf::Event& e)
{
    switch (e.key.code)
    {
    case sf::Keyboard::C:
        changeCameraMode();
        break;
    case sf::Keyboard::F1:
        m_helpDisplayed = !m_helpDisplayed;
        break;
    case sf::Keyboard::F2:
        takeScreenshot();
        break;
    case sf::Keyboard::F3:
        reloadShaderPrograms();
        break;

    case sf::Keyboard::W:
      if( e.key.control )
        m_applicationRunning = false;
      break;

    case sf::Keyboard::Q:
        keyboard.left = true;
        keyboard.direction.x = -1;
        break;

    case sf::Keyboard::S:
        keyboard.backward = true;
        keyboard.direction.z = -1;
        break;

    case sf::Keyboard::D:
        keyboard.right = true;
        keyboard.direction.x = 1;
        break;

    case sf::Keyboard::Z:
        keyboard.forward = true;
        keyboard.direction.z = 1;
        break;

    case sf::Keyboard::Space:
        keyboard.fast = true;
        break;

    case sf::Keyboard::LShift:
        keyboard.slow = true;
        break;
    default:
        break;
    }
    for(RenderablePtr r : m_renderables)
        r->keyPressedEvent(e);
}

void Viewer::keyReleasedEvent(sf::Event& e)
{
  switch (e.key.code)
  {
  case sf::Keyboard::Q:
      keyboard.left = false;
      keyboard.direction.x = keyboard.right ? 1 : 0;
      break;

  case sf::Keyboard::S:
      keyboard.backward = false;
      keyboard.direction.z = keyboard.forward ? 1 : 0;
      break;

  case sf::Keyboard::D:
      keyboard.right = false;
      keyboard.direction.x = keyboard.left ? -1 : 0;
      break;

  case sf::Keyboard::Z:
      keyboard.forward = false;
      keyboard.direction.z = keyboard.backward ? -1 : 0;
      break;

  case sf::Keyboard::Space:
      keyboard.fast = false;
      break;

  case sf::Keyboard::LShift:
      keyboard.slow = false;
      break;
  default:
      break;
  }


    for(RenderablePtr r : m_renderables)
        r->keyReleasedEvent(e);
}

void Viewer::mousePressEvent(sf::Event& e)
{
    for(RenderablePtr r : m_renderables)
        r->mousePressEvent(e);
}

void Viewer::mouseReleaseEvent(sf::Event& e)
{
    for(RenderablePtr r : m_renderables)
        r->mouseReleaseEvent(e);
}

void Viewer::mouseWheelEvent(sf::Event& e)
{
    // a wheel event modify the distance of the camera to the scene origin
    // when the wheel moved of a distance dz, the camera will be translated of dz * Z_camera
    // note: if we perform this operation inside the camera class, we avoid useless computations.
    // however, it's easier to understand that way.
  if( m_camera.getMouseBehavior() == Camera::ARCBALL_BEHAVIOR )
    m_camera.setPosition( m_camera.getPosition() + float(e.mouseWheel.delta) * m_camera.getForward() );

    //Solve mouse wheel event for the renderables of the viewer
    for(RenderablePtr r : m_renderables)
        r->mouseWheelEvent(e);
}

void Viewer::mouseMoveEvent(sf::Event& e)
{
    //Compute normalized mouse position between [-1,1]
    glm::vec3 normalizedMousePosition;
    normalizedMousePosition.x = 2.0f*e.mouseMove.x/(float)m_window.getSize().x -1.0f;
    normalizedMousePosition.y = 1.0f - (2.0f*e.mouseMove.y)/(float)m_window.getSize().y;
    normalizedMousePosition.z = 1.0f;

    //Set value for current mouse position and displacement from the last move.
    m_currentMousePosition = normalizedMousePosition;
    m_deltaMousePosition = m_currentMousePosition - m_lastMousePosition;

    if( sf::Mouse::isButtonPressed(sf::Mouse::Right) )
    {
        m_camera.update( m_deltaMousePosition.x, -m_deltaMousePosition.y );
    }

    //Set last mouse position.
    m_lastMousePosition = m_currentMousePosition;

    for(RenderablePtr r : m_renderables)
        r->mouseMoveEvent(e);
}


void Viewer::handleEvent()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            m_applicationRunning=false;
            break;
        case sf::Event::Resized:
            m_window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            m_camera.setRatio( (float)(m_window.getSize().x)/(float)(m_window.getSize().y) );
            m_tengine.setWindowDimensions( m_window.getSize().x, m_window.getSize().y );
            glcheck(glViewport(0, 0, event.size.width, event.size.height));
            break;
        case sf::Event::KeyPressed:
            keyPressedEvent(event);
            break;

        case sf::Event::KeyReleased:
            keyReleasedEvent(event);
            break;
        case sf::Event::MouseWheelMoved:
            mouseWheelEvent(event);
            break;
        case sf::Event::MouseButtonPressed:
            mousePressEvent(event);
            break;
        case sf::Event::MouseButtonReleased:
            mouseReleaseEvent(event);
            break;
        case sf::Event::MouseMoved:
            mouseMoveEvent(event);
            break;
        default:
            break;
        }
    }

    if( m_camera.getMouseBehavior() == Camera::SPACESHIP_BEHAVIOR
        && (keyboard.forward || keyboard.backward || keyboard.left || keyboard.right ) )
      {
        float speed = keyboard.speed * Duration(clock::now() - m_lastEventHandleTime).count();
        if( keyboard.fast )
          speed *= float(15.0);
        else if ( keyboard.slow )
          speed *= float(0.5);
        glm::vec3 shift = speed * normalize( keyboard.direction );
        m_camera.setPosition( m_camera.getPosition()
            + shift.x * m_camera.getRight()
            + shift.y * m_camera.getUp()
            + shift.z * m_camera.getForward() );
      }
    m_lastEventHandleTime = clock::now();
}

void Viewer::takeScreenshot()
{
    int padding = 5;
    sf::Image screen = m_window.capture();
    std::ostringstream filename_sstr;
    filename_sstr << screenshot_basename << std::setw(padding) << std::setfill('0') << m_screenshotCounter << ".png";
    if( !screen.saveToFile(filename_sstr.str()) )
      {
        LOG( error, "Error while saving the screenshot")
      }
    else
      {
        LOG( info, "Screenshot successfully taken : " << filename_sstr.str())
      }
    m_screenshotCounter++;
}

void Viewer::changeCameraMode()
{
    if( m_camera.getMouseBehavior() == Camera::ARCBALL_BEHAVIOR )
    {
        m_camera.setMouseBehavior( Camera::SPACESHIP_BEHAVIOR );
        m_modeInformationText = "Spaceship Camera Activated";
    }
    else
    {
        m_camera.setMouseBehavior( Camera::ARCBALL_BEHAVIOR );
        m_modeInformationText = "Arcball Camera Activated";
    }
    m_modeInformationTextDisappearanceTime = clock::now() + g_modeInformationTextTimeout;
}

bool Viewer::isRunning() const
{
  return m_applicationRunning;
}

void Viewer::display()
{
  m_window.display();
}

void Viewer::addShaderProgram( ShaderProgramPtr program )
{
  m_programs.insert( program );
}

void Viewer::reloadShaderPrograms()
{
  for( ShaderProgramPtr program : m_programs )
    program->reload();
}

Camera& Viewer::getCamera()
{
  return m_camera;
}
