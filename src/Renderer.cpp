#include "Renderer.hpp"

Renderer::Renderer( Env* env ) :
    env(env),
    camera(70, (float)env->getWindow().width / (float)env->getWindow().height),
    shader("./shader/vertex.glsl", "./shader/fragment.glsl") {
        this->env->getCharacter()->setShader(&this->shader);
}

Renderer::~Renderer( void ) {
}

float rayEllipsoidIntersect( const vec3& ray_origin, const vec3& ray_dir, const vec3& ellipsoid_origin, const vec3& ellipsoid_radius ) {
    vec3 origin = ray_origin - ellipsoid_origin;
    // float a = mtls::dot(ray_dir, ray_dir);
    // float b = 2.0f * mtls::dot(ray_dir, origin);
    // float c = mtls::dot(origin, origin) - 1.0f;
    float a = ((ray_dir[0]*ray_dir[0])/(ellipsoid_radius[0]*ellipsoid_radius[0]))
            + ((ray_dir[1]*ray_dir[1])/(ellipsoid_radius[1]*ellipsoid_radius[1]))
            + ((ray_dir[2]*ray_dir[2])/(ellipsoid_radius[2]*ellipsoid_radius[2]));
    float b = ((2.0f*origin[0]*ray_dir[0])/(ellipsoid_radius[0]*ellipsoid_radius[0]))
            + ((2.0f*origin[1]*ray_dir[1])/(ellipsoid_radius[1]*ellipsoid_radius[1]))
            + ((2.0f*origin[2]*ray_dir[2])/(ellipsoid_radius[2]*ellipsoid_radius[2]));
    float c = ((origin[0]*origin[0])/(ellipsoid_radius[0]*ellipsoid_radius[0]))
            + ((origin[1]*origin[1])/(ellipsoid_radius[1]*ellipsoid_radius[1]))
            + ((origin[2]*origin[2])/(ellipsoid_radius[2]*ellipsoid_radius[2]))
            - 1.0f;

    float d = ((b * b) - (4.0f * a * c));
    if (d < 0.0f)
        return (-1.0f);
    else
        d = std::sqrt(d);

    float t1 = (-b + d) / (2.0f * a);
    float t2 = (-b - d) / (2.0f * a);

    // std::cout << "Hit1: " << hit << " Hit2: " << hitsecond << std::endl;
    return (t1 < t2 ? t1 : t2);
}
// void    Renderer::raycast( void ) {
    // mat4    toWorld = mtls::inverse(this->camera.getProjectionMatrix() * this->camera.getViewMatrix());
//     vec2d   m = this->env->getController()->getMousePosition(); // (x, y)
//     /* put mouse position in clip-space (-1, 1) */
//     vec2    mouse = vec2({(float)m[0] / this->env->getWindow().width, (float)m[1] / this->env->getWindow().height});
//     mouse = mouse * 2.0f - 1.0f;
//     mouse(1) = -mouse[1];
//     // std::cout << mouse << std::endl;
//     /* convert that position to world */
//     // vec4    from = toWorld * vec4({mouse[0], mouse[1],-1, 1});
//     // vec4      to = toWorld * vec4({mouse[0], mouse[1], 1, 1});
//     // from /= from[3];
//     // to /= to[3];
//
//     vec4    from = vec4({mouse[0], mouse[1],-1, 1});
//     vec4      to = vec4({mouse[0], mouse[1], 1, 1});
//
//     size_t  i = 0;
//     size_t  clickedObject = -1;
//     float   dist = 99999.0f;
//
//     std::unordered_map<std::string, Bone*>   obj = this->env->getCharacter()->getBones();
//
//     for (auto it = obj.begin(); it != obj.end(); it++) {
//         float t1, t2;
//         vec3    dir = static_cast<vec3>(to - from);
//
//         float res = rayEllipsoidIntersect(static_cast<vec3>(from), dir, obj[it->first]->getModel()->getPosition(), obj[it->first]->getModel()->getScale());
//
//         if (res != -1.0f)
//             std::cout << "intersect: " << it->first << std::endl;
//         // if (intersectSphere(static_cast<vec3>(from), dir, obj[it->first]->getModel()->getPosition(), obj[it->first]->getModel()->getScale(), &t1, &t2)) {
//         // if (rayEllipsoidIntersect(static_cast<vec3>(from), dir, obj[it->first]->getModel()->getPosition(), 1.0f, &t1, &t2) != -1.0f) {
//             // if (t1 < dist) {
//             //     dist = t1;
//             //     clickedObject = i;
//             //     std::cout << "Intersect: " << obj[it->first]->getName() << std::endl;
//             // }
//         // }
//         i++;
//     }
// }

void    Renderer::raycast( void ) {
    /* put mouse position in clip-space (-1, 1) */
    vec2 mouse = mousePosToClipSpace( this->env->getController()->getMousePosition(), this->env->getWindow().width, this->env->getWindow().height );

    /* create the ray and convert it to world-space */
    vec4    rayClip = vec4({mouse[0], mouse[1], -1.0, 1.0});
    vec4    rayEye = mtls::inverse(this->camera.getProjectionMatrix()) * rayClip;
    rayEye = vec4({rayEye[0], rayEye[1], -1.0, 0.0});
    vec3 rayWor = static_cast<vec3>(mtls::inverse(this->camera.getViewMatrix()) * rayEye);
    rayWor = mtls::normalize(rayWor);

    vec4 tmp = static_cast<vec4>(this->camera.getPosition() + this->camera.getTranslation());
    vec3 from = static_cast<vec3>(this->camera.getViewMatrix().transpose() * tmp);

    float       dist = 999999999.0f;
    std::string objectId = "none";

    std::unordered_map<std::string, Bone*>   obj = this->env->getCharacter()->getBones();
    for (auto it = obj.begin(); it != obj.end(); it++) {
        float t = rayEllipsoidIntersect(from, rayWor, obj[it->first]->getModel()->getWorldPosition(), obj[it->first]->getModel()->getScale());
        if (t > 0.0f and t < dist) {
            dist = t;
            objectId = it->first;
        }
    }
    std::cout << "Intersect: " << objectId << std::endl;
}

void	Renderer::loop( void ) {
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(this->env->getWindow().ptr)) {
        glfwPollEvents();
        glClearColor(0.09f, 0.08f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->shader.use();
        this->env->getController()->update();


        this->camera.handleKeys( this->env->getController()->getKeys(), this->env->getCharacter()->getParentBone()->getModel()->getPosition() );
        this->env->getAnimator()->handleKeys( this->env->getController()->getKeys() );
        this->env->getAnimator()->update();

        this->raycast();

        this->updateShaderUniforms();
        glfwSwapBuffers(this->env->getWindow().ptr);
    }
}

void    Renderer::updateShaderUniforms( void ) {
    this->shader.setMat4UniformValue("view", this->camera.getViewMatrix());
    this->shader.setMat4UniformValue("projection", this->camera.getProjectionMatrix());
}
