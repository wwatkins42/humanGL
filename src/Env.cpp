#include "Env.hpp"

Env::Env( void ) : character() {
    try {
        this->initGlfwEnvironment("4.0");
        this->initGlfwWindow(1280, 960);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw Exception::InitError("glad initialization failed");
        this->controller = new Controller(this->window.ptr);
        this->character = new Skeleton(this->createCharacterSkeleton(), "torso");
        this->animator = new Animator(this->character, {
            {new tAnimationFrames({{}}), 100, false},
            {this->createIdleAnimation(), 1800, false},
            {this->createWalkingAnimation(), 750, false},
            {this->createBackflipAnimation(), 1500, true},
            {this->createBackflipConnectedAnimation(), 2000, true},
            {this->createJumpingAnimation(), 1100, false},
        });
        this->setupController();
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }
}

Env::~Env( void ) {
    delete this->character;
    delete this->animator;
    delete this->controller;
    glfwDestroyWindow(this->window.ptr);
    glfwTerminate();
}

void	Env::initGlfwEnvironment( const std::string& glVersion ) {
	if (!glfwInit())
		throw Exception::InitError("glfw initialization failed");
    if (!std::regex_match(glVersion, static_cast<std::regex>("^[0-9]{1}.[0-9]{1}$")))
        throw Exception::InitError("invalid openGL version specified");
    size_t  p = glVersion.find('.');
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::stoi(glVersion.substr(0,p)));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::stoi(glVersion.substr(p+1)));
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void	Env::initGlfwWindow( size_t width, size_t height ) {
    glfwWindowHint(GLFW_SAMPLES, 8);
	if (!(this->window.ptr = glfwCreateWindow(width, height, "humanGL", NULL, NULL)))
        throw Exception::InitError("glfwCreateWindow failed");
	glfwMakeContextCurrent(this->window.ptr);
	glfwSetFramebufferSizeCallback(this->window.ptr, this->framebufferSizeCallback);
	glfwSetInputMode(this->window.ptr, GLFW_STICKY_KEYS, 1);
    this->window.width = width;
    this->window.height = height;
}

void    Env::setupController( void ) {
    this->controller->setKeyProperties(GLFW_KEY_C, eKeyMode::toggle, 0, 1000);
    this->controller->setKeyProperties(GLFW_KEY_M, eKeyMode::cycle, 1, 300, 3);
}

void    Env::framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
    glViewport(0, 0, width, height);
}

tAnimationFrames*   Env::createIdleAnimation( void ) {
    tAnimationFrames*   idleAnimation = new tAnimationFrames({{
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({-0.1, 0, 0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({   0, 0, 0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({   0,0.05, 0}), vec3({    0,-0.06, 0.08 }), vec3({   0,    0,   0 })},
            {"lowerArmLeft",  vec3({   0,   0, 0}), vec3({ 0.03,    0,    0 }), vec3({   0,    0,   0 })},
            {"upperArmRight", vec3({   0,0.05, 0}), vec3({    0, 0.06,-0.08 }), vec3({   0,    0,   0 })},
            {"lowerArmRight", vec3({   0,   0, 0}), vec3({ 0.03,    0,    0 }), vec3({   0,    0,   0 })},
            {"torso",         vec3({   0,   0, 0}), vec3({    0,    0,    0 }), vec3({ 0.2, 0.15, 0.1 })},
            {"head",          vec3({   0,   0, 0}), vec3({    0,    0,    0 }), vec3({   0,    0,   0 })},
            {"upperLegLeft",  vec3({ 0.01,  0, 0}), vec3({    0,    0,    0 }), vec3({   0,    0,   0 })},
            {"lowerLegLeft",  vec3({   0,   0, 0}), vec3({    0,    0,    0 }), vec3({   0,    0,   0 })},
            {"upperLegRight", vec3({-0.01,  0, 0}), vec3({    0,    0,    0 }), vec3({   0,    0,   0 })},
            {"lowerLegRight", vec3({   0,   0, 0}), vec3({    0,    0,    0 }), vec3({   0,    0,   0 })},
        }}),
    }});
    return (idleAnimation);
}

tAnimationFrames*   Env::createWalkingAnimation( void ) {
    tAnimationFrames*   walkingAnimation = new tAnimationFrames({{
        // contact
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.7, 0.9,-0.1}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, 0.1, 0.025}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, -0.025}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, -0.05}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({-0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // going down
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.6, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.8, 0.5,-0.05}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, 0.2, 0}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({-0.6, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // average pose
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.03, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.25, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0.2, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, 0.1, -0.03}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0.07}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0.07}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ 0, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-1.8, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // going up
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, 0, -0.07}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0.13}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0.13}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.9, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-1.2, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // contact, DO IT AGAIN
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.7,-0.9, 0.1}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, -0.1, -0.025}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0.025}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0.05}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // going down
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.8,-0.5, 0.05}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.6, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, -0.2, 0}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.6, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // average pose
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.3,-0.2, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.03, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.25, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, -0.1, 0.03}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, -0.07}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, -0.07}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-1.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({   0, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        // going up
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({-0.2, 0, 0.07}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, -0.13}), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, -0.13}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.9, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-1.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
        }}),

    }});
    return (walkingAnimation);
}

tAnimationFrames*   Env::createBackflipAnimation( void ) {
    tAnimationFrames*   backflipAnimation = new tAnimationFrames({{
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.95, 0.0, 0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.17, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.95, 0.0, -0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.17, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.7, 0}), vec3({ -0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.1, 0.0, 0.15 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.1, 0.0, -0.15 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 3.14, 0.4, 0.3 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  0.0, 0.0, -0.2 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 3.14, -0.4, -0.3 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.2 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0.3, -0.1}), vec3({  0.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ -0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.7, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ -0.4, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.35, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ -0.4, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.35, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.8, 0.0, 0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.97, 0.0, -0.6 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.6, 0.0, -0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.97, 0.0, 0.6 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 1.3, -0.5}), vec3({  1.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.5, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.4, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -2.3, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.6, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -2.3, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.7, 0.0, 0.08 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.97, 0.0, -0.56 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.6, 0.0, -0.08 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.97, 0.0, 0.56 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 1.5, -1}), vec3({  2.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.5, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.5, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -2.3, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.6, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -2.3, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.9, 0.0, 0.08 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  1.1, 0.0, -0.4 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.9, 0.0, -0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  1.1, 0.0, 0.3 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0.9, -1.5}), vec3({  3.7, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.4, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ -0.7, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.9, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.9, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  1.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  1.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, -0.9, -2.2}), vec3({  4.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 1.48, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 1.48, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  1.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  1.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.6, -2.5}), vec3({  5.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.4, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -0.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.4, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -0.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, -3.1}), vec3({ M_PI*2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
    }});
    return (backflipAnimation);
}


tAnimationFrames*   Env::createBackflipConnectedAnimation( void ) {
    tAnimationFrames*   backflipConnectedAnimation = new tAnimationFrames({{
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.95, 0.0, 0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.17, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.95, 0.0, -0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.17, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.7, 0}), vec3({ -0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.1, 0.0, 0.15 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.1, 0.0, -0.15 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 3.14, 0.4, 0.3 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  0.0, 0.0, -0.2 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 3.14, -0.4, -0.3 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.2 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0.3, -0.1}), vec3({  0.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ -0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.7, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ -0.4, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.35, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ -0.4, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.35, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.8, 0.0, 0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.97, 0.0, -0.6 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.6, 0.0, -0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.97, 0.0, 0.6 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 1.3, -0.5}), vec3({  1.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.5, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.4, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -2.3, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.6, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -2.3, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.7, 0.0, 0.08 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.97, 0.0, -0.56 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.6, 0.0, -0.08 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.97, 0.0, 0.56 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 1.5, -1}), vec3({  2.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.5, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.5, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -2.3, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.6, 0.0, -0.07 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -2.3, 0.0, 0.07 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.9, 0.0, 0.08 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  1.1, 0.0, -0.4 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.9, 0.0, -0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  1.1, 0.0, 0.3 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0.9, -1.5}), vec3({  3.7, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.4, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ -0.7, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.9, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.9, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  1.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  1.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, -0.9, -2.2}), vec3({  4.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 1.48, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 1.48, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -0.2, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({  1.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({  1.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.6, -2.5}), vec3({  5.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.1, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.4, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -0.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.4, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -0.8, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
        // Walking connection
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.7, 0.9,-0.1}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, -2.5}), vec3({ M_PI*2-0.2, 0.1, 0.025}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, -0.025}), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({ 0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, -0.05}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({-0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.6, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.8, 0.5,-0.05}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, -2}), vec3({ M_PI*2-0.2, 0.2, 0}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({ 0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({-0.6, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.03, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.25, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0.2, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, -1.5}), vec3({ M_PI*2-0.2, 0.1, -0.03}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0.07}), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0.07}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ 0, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-1.8, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.3, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, -1}), vec3({ M_PI*2-0.2, 0, -0.07}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0.13}), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0.13}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.9, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-1.2, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({ 0.7,-0.9, 0.1}), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.5, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0, -0.5}), vec3({ M_PI*2-0.2, -0.1, -0.025}), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({ 0.2, 0, 0.025}), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({ 0, 0, 0.05}), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({-0.4, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({ 0.7, 0, 0}), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({-0.1, 0, 0}), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0, 0, 0}), vec3({-0.95, 0.0, 0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0, 0, 0}), vec3({ 0.17, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0, 0, 0}), vec3({-0.95, 0.0, -0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0, 0, 0}), vec3({ 0.17, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.7, 0}), vec3({ (M_PI*2.0)-0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"pelvis",        vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"stomach",       vec3({0, 0, 0}), vec3({  0.0, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0, 0, 0}), vec3({  0.6, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0, 0, 0}), vec3({  1.5, 0.0, 0.15 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0, 0, 0}), vec3({  1.5, 0.0, -0.15 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0, 0, 0}), vec3({ -1.3, 0.0, 0.0 }), vec3({ 0, 0, 0 })},
        }}),
    }});
    return (backflipConnectedAnimation);
}

tAnimationFrames*   Env::createJumpingAnimation( void ) {
    tAnimationFrames*   jumpingAnimation = new tAnimationFrames({{
        // frame 1: start
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,    0, 0}), vec3({  0.2,   0, 0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,    0, 0}), vec3({  0.3,   0,-0.3 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,    0, 0}), vec3({  0.2,   0,-0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,    0, 0}), vec3({  0.3,   0, 0.3 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.25, 0}), vec3({ -0.4,   0,   0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,    0, 0}), vec3({ -0.1,   0,   0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,    0, 0}), vec3({  0.8,   0,   0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,    0, 0}), vec3({ -0.4,   0,   0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,    0, 0}), vec3({  0.8,   0,   0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,    0, 0}), vec3({ -0.4,   0,   0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({ -0.4, 0.3,   0.3 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.1,   0,  -0.2 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({ -0.4,-0.3,  -0.3 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.1,   0,   0.2 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.4, 0}), vec3({ -0.6,   0,     0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({ -0.1,   0,     0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({  1.2,   0,     0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({ -0.6,   0,     0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({  1.2,   0,     0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({ -0.6,   0,     0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({ -0.8,  0.3,  0.4 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.3,    0, -0.2 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({ -0.8, -0.3, -0.4 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.3,    0,  0.2 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.5, 0}), vec3({ -0.7,    0,    0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({ -0.5,    0,    0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({  1.3,    0,    0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({ -0.6,    0,    0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({  1.3,    0,    0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({ -0.6,    0,    0 }), vec3({ 0, 0, 0 })},
        }}),
        // frame 4 : jumping
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({    1,-0.3,-0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.5,   0,-0.1 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({    1, 0.3, 0.2 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.5,   0, 0.1 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 0.5, 0}), vec3({    0,   0,   0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({ -0.2,   0,   0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({  0.1,   0,   0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({ -0.4,   0,   0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({  0.1,   0,   0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({ -0.4,   0,   0 }), vec3({ 0, 0, 0 })},
        }}),
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({  0.1,   0, 0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.3,   0,-0.1 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({  0.1,   0,-0.1 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.3,   0, 0.1 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 2.1, 0}), vec3({    0,   0,   0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({ -0.1,   0,   0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({  0.6,   0,   0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({ -1.2,   0,   0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({  0.6,   0,   0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({ -1.2,   0,   0 }), vec3({ 0, 0, 0 })},
        }}),
        // frame 6: reaching jump peak
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({ -0.2,-0.3,   0.4 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.1,   0,  -0.1 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({ -0.2, 0.3,  -0.4 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.1,   0,   0.1 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 2.8, 0}), vec3({    0,   0,     0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({    0,   0,     0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({  0.4,   0,     0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({ -0.8,   0,     0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({  0.4,   0,     0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({ -0.8,   0,     0 }), vec3({ 0, 0, 0 })},
        }}),
        // frame 7: falling
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({ -0.4,-0.35,  0.6 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.3,    0, -0.2 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({ -0.4, 0.35, -0.6 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.3,    0,  0.2 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0, 1.5, 0}), vec3({    0,    0,    0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({    0,    0,    0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({    0,    0,    0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({    0,    0,    0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({    0,    0,    0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({    0,    0,    0 }), vec3({ 0, 0, 0 })},
        }}),
        // frame 8: landing
        new std::vector<tBoneTransform>({{
            {"upperArmLeft",  vec3({0,   0, 0}), vec3({  0.1, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerArmLeft",  vec3({0,   0, 0}), vec3({  0.1, 0, 0 }), vec3({ 0, 0, 0 })},
            {"upperArmRight", vec3({0,   0, 0}), vec3({  0.1, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerArmRight", vec3({0,   0, 0}), vec3({  0.1, 0, 0 }), vec3({ 0, 0, 0 })},
            {"torso",         vec3({0,-0.2, 0}), vec3({ -0.2, 0, 0 }), vec3({ 0, 0, 0 })},
            {"head",          vec3({0,   0, 0}), vec3({ -0.4, 0, 0 }), vec3({ 0, 0, 0 })},
            {"upperLegLeft",  vec3({0,   0, 0}), vec3({  0.6, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerLegLeft",  vec3({0,   0, 0}), vec3({ -0.6, 0, 0 }), vec3({ 0, 0, 0 })},
            {"upperLegRight", vec3({0,   0, 0}), vec3({  0.6, 0, 0 }), vec3({ 0, 0, 0 })},
            {"lowerLegRight", vec3({0,   0, 0}), vec3({ -0.6, 0, 0 }), vec3({ 0, 0, 0 })},
        }}),
    }});
    return (jumpingAnimation);
}

std::unordered_map<std::string, Bone*>  Env::createCharacterSkeleton( void ) {
    std::unordered_map<std::string, Bone*>  bones;
    bones["head"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "head",
        vec3({0, 0.425, 0}),
        vec3({0, 0, 0}),
        vec3({0.8, 0.85, 0.8}),
        vec3({0, -0.225, 0}),
        0xEEAD7E
    );
    bones["neck"] = new Bone(
        (std::forward_list<Bone*>){ bones["head"] },
        "neck",
        vec3({0, 0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xEEAD7E
    );
    bones["lowerLegLeft"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerLegLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.4, 1, 0.4}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["lowerLegRight"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerLegRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.4, 1, 0.4}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["kneeLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerLegLeft"] },
        "kneeLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["kneeRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerLegRight"] },
        "kneeRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["upperLegLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["kneeLeft"] },
        "upperLegLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.5, 1, 0.5}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["upperLegRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["kneeRight"] },
        "upperLegRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.5, 1, 0.5}),
        vec3({0, 0.5, 0}),
        0x3F5D6A
    );
    bones["hipLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["upperLegLeft"] },
        "hipLeft",
        vec3({-0.3, -0.4, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["hipRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["upperLegRight"] },
        "hipRight",
        vec3({0.3, -0.4, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["lowerArmLeft"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerArmLeft",
        vec3({0, -0.4, 0}),
        vec3({0, 0, 0}),
        vec3({0.35, 0.8, 0.35}),
        vec3({0, 0.4, 0}),
        0xEEAD7E
    );
    bones["lowerArmRight"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerArmRight",
        vec3({0, -0.4, 0}),
        vec3({0, 0, 0}),
        vec3({0.35, 0.8, 0.35}),
        vec3({0, 0.4, 0}),
        0xEEAD7E
    );
    bones["elbowLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerArmLeft"] },
        "elbowLeft",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["elbowRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerArmRight"] },
        "elbowRight",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["upperArmLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["elbowLeft"] },
        "upperArmLeft",
        vec3({0, -0.45, 0}),
        vec3({0, 0, 0}),
        vec3({0.4, 0.9, 0.4}),
        vec3({0, 0.45, 0}),
        0x408467
    );
    bones["upperArmRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["elbowRight"] },
        "upperArmRight",
        vec3({0, -0.45, 0}),
        vec3({0, 0, 0}),
        vec3({0.4, 0.9, 0.4}),
        vec3({0, 0.45, 0}),
        0x408467
    );
    bones["shoulderLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["upperArmLeft"] },
        "shoulderLeft",
        vec3({-0.85, 0, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["shoulderRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["upperArmRight"] },
        "shoulderRight",
        vec3({0.85, 0, 0}),
        vec3({0, 0, 0}),
        vec3({0.3, 0.3, 0.3}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["pelvis"] = new Bone(
        (std::forward_list<Bone*>){{ bones["hipLeft"], bones["hipRight"] }},
        "pelvis",
        vec3({0, -0.425, 0}),
        vec3({0, 0, 0}),
        vec3({0.95, 0.95, 0.8}),
        vec3({0, 0, 0}),
        0x43876A
    );
    bones["stomach"] = new Bone(
        (std::forward_list<Bone*>){ bones["pelvis"] },
        "stomach",
        vec3({0, -0.5, 0}),
        vec3({0, 0, 0}),
        vec3({0.8, 0.8, 0.9}),
        vec3({0, 0, 0}),
        0xA4B37F
    );
    bones["torso"] = new Bone(
        (std::forward_list<Bone*>){{ bones["neck"], bones["stomach"], bones["shoulderLeft"], bones["shoulderRight"] }},//, bones["shoulderLeft"], bones["shoulderRight"], bones["hipLeft"], bones["hipRight"] }},
        "torso",
        vec3({0, 0.25, 0}),
        vec3({0, 0, 0}),
        vec3({1.5, 1, 1}),
        vec3({0, 0, 0}),
        0x43876A
    );
    return (bones);
}
