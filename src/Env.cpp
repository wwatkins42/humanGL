#include "Env.hpp"

Env::Env( void ) : character() {
    try {
        this->initGlfwEnvironment("4.0");
        this->initGlfwWindow(1280, 960);
        // glad : load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw Exception::InitError("glad initialization failed");
        this->character = new Skeleton(this->createCharacterSkeleton(), "torso");
        this->animator = new Animator(this->character, this->createWalkingAnimation(), 500);
    } catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }
}

Env::Env( const Env& rhs ) {
    *this = rhs;
}

Env & Env::operator=( const Env& rhs ) {
    (void)rhs;
    return (*this);
}

Env::~Env( void ) {
    delete this->character;
    delete this->animator;
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
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);//GL_FALSE);
}

void	Env::initGlfwWindow( size_t width, size_t height ) {
	if (!(this->window.ptr = glfwCreateWindow(width, height, "humanGL", NULL, NULL)))
        throw Exception::InitError("glfwCreateWindow failed");
	glfwMakeContextCurrent(this->window.ptr);
	glfwSetFramebufferSizeCallback(this->window.ptr, this->framebufferSizeCallback);
	glfwSetInputMode(this->window.ptr, GLFW_STICKY_KEYS, 1);
    this->window.width = width;
    this->window.height = height;
}

void    Env::framebufferSizeCallback( GLFWwindow* window, int width, int height ) {
    glViewport(0, 0, width, height);
}

std::unordered_map<std::string, Bone*>  Env::createCharacterSkeleton( void ) {
    std::unordered_map<std::string, Bone*>  bones;
    bones["head"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "head",
        vec3({0, 2, 0}),
        vec3({1, 1, 1}),
        vec3({0, 0, 0}), vec3({0, -0.5, 0}),
        0xEEAD7E
    );
    bones["lowerLegLeft"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerLegLeft",
        vec3({0, -1.5, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({0, 0.75, 0}),
        0x3F5D6A
    );
    bones["lowerLegRight"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerLegRight",
        vec3({0, -1.5, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({0, 0.75, 0}),
        0x3F5D6A
    );
    bones["upperLegLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerLegLeft"] },
        "upperLegLeft",
        vec3({-0.625, -2.25, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({-0.375, 0.75, 0}),
        0x3F5D6A
    );
    bones["upperLegRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerLegRight"] },
        "upperLegRight",
        vec3({0.625, -2.25, 0}),
        vec3({0.75, 1.5, 0.75}),
        vec3({0, 0, 0}), vec3({0.375, 0.75, 0}),
        0x3F5D6A
    );
    bones["lowerArmLeft"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerArmLeft",
        vec3({0, -1.5, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({1.9, 0, 0}), vec3({0, 0.75, 0}),
        0xEEAD7E
    );
    bones["lowerArmRight"] = new Bone(
        (std::forward_list<Bone*>){{}},
        "lowerArmRight",
        vec3({0, -1.5, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({1.9, 0, 0}), vec3({0, 0.75, 0}),
        0xEEAD7E
    );
    bones["upperArmLeft"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerArmLeft"] },
        "upperArmLeft",
        vec3({-1.3, 0.75, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({0, 0, 0}), vec3({0.3, 0.75, 0}),
        0x408467
    );
    bones["upperArmRight"] = new Bone(
        (std::forward_list<Bone*>){ bones["lowerArmRight"] },
        "upperArmRight",
        vec3({1.3, 0.75, 0}),
        vec3({0.6, 1.5, 0.6}),
        vec3({0, 0, 0}), vec3({-0.3, 0.75, 0}),
        0x408467
    );
    bones["torso"] = new Bone(
        (std::forward_list<Bone*>){{ bones["head"], bones["upperArmLeft"], bones["upperArmRight"], bones["upperLegLeft"], bones["upperLegRight"] }},
        "torso",
        vec3({0, 0, 0}),
        vec3({2, 3, 0.9}),
        vec3({0, 0, 0}), vec3({0, 0, 0}),
        0x43876A
    );
    return (bones);
}

tAnimationFrames*   Env::createWalkingAnimation( void ) {
    // tAnimationFrames*   walkingAnimation = new tAnimationFrames({{
    //     // 1st frame
    //     new std::vector<tBoneTransform>({{
    //         {"upperArmRight", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
    //         {"upperArmLeft",  vec3({0, 0, 0}), vec3({0, 1, 0}), vec3({0, 0, 0})},
    //         {"lowerArmRight", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
    //         {"lowerArmLeft",  vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
    //     }}),
    //     // 2nd frame
    //     new std::vector<tBoneTransform>({{
    //         {"upperArmRight", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
    //         {"upperArmLeft",  vec3({0, 0, 0}), vec3({1, 0, 0}), vec3({0, 0, 0})},
    //         {"lowerArmRight", vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
    //         {"lowerArmLeft",  vec3({0, 0, 0}), vec3({0, 0, 0}), vec3({0, 0, 0})},
    //     }}),
    //     // ...
    // }});
    tAnimationFrames*   walkingAnimation = new tAnimationFrames({{
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({   1, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.8, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.6, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.4, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({   0, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.4, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.6, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({  -1, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.8, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.6, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.4, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({-0.2, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({   0, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.2, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.4, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.6, 0, 0}), vec3({0, 0, 0})} }}),
        new std::vector<tBoneTransform>({{{"upperArmLeft", vec3({0, 0, 0}), vec3({ 0.8, 0, 0}), vec3({0, 0, 0})} }}),
    }});

    return (walkingAnimation);
}
