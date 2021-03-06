#include <SDL2/SDL.h>
#include <iostream>
#include <exception>
#include "app.h"
#include "math/vec3_ops.h"
#include "src/fractals/mandelbox.h"
#include "src/fractals/mandelbulb.h"
#include "src/fractals/menger_sponge.h"
#include "src/fractals/sierpinski_triangle.h"

#pragma OPENCL EXTENSION CL_APPLE_gl_sharing : enable
#pragma OPENCL EXTENSION CL_KHR_gl_sharing : enable

App::App(int width, int height, bool centered) {
    this->fullscreen = false;

    win_height = height;
    win_width = width;

    window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        throw * new std::runtime_error("Failed to initialize sdl2");
    }

    window = SDL_CreateWindow("Fractal Explorer", 100, 100, win_width, win_height, window_flags);
    SDL_HideWindow(window);

    if (window == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        throw * new std::runtime_error("Failed to initialize window");
    }

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    context = CLContext(gl_context);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    screen_height = DM.h;
    screen_width = DM.w;

    // Initializing OpenGL textures
    glGenTextures(1, &fullscreen_texture_id);
    glBindTexture(GL_TEXTURE_2D, fullscreen_texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, nullptr);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win_width, win_height, 0, GL_RGBA, GL_FLOAT, nullptr);

    // Initializing OpenCL Buffers
    cl_int texture_error;
    cl::Context & cl_context = context.get_context();
    image = clCreateFromGLTexture(
        cl_context(),
        CL_MEM_WRITE_ONLY,
        GL_TEXTURE_2D,
        0,
        texture_id,
        &texture_error
    );

    if (texture_error)
        throw * new std::runtime_error("Failed to initialize CL Image2D buffer.");

    fullscreen_image = clCreateFromGLTexture(
        cl_context(),
        CL_MEM_WRITE_ONLY,
        GL_TEXTURE_2D,
        0,
        fullscreen_texture_id,
        &texture_error
    );

    if (texture_error)
        throw * new std::runtime_error("Failed to initialize CL Image2D buffer.");

    // Initializing fractal renderers
    cl_float3 sun_direction = {-1, 1, -1};

    renderers.push_back(* new Render(context, sun_direction, * new Mandelbox()));
    renderers.push_back(* new Render(context, sun_direction, * new Mandelbulb()));
    renderers.push_back(* new Render(context, sun_direction, * new MengerSponge()));
    renderers.push_back(* new Render(context, sun_direction, * new SierpinskiTriangle()));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
}

void App::run() {
    SDL_ShowWindow(window);
    SDL_ShowCursor(SDL_DISABLE);

    glViewport(0, 0, win_width, win_height);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    bool running = true;

    uint32_t renderer_index = 0;

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float delta_time;

    const Uint8 * keyboard_state = SDL_GetKeyboardState(NULL);

    while (running) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        cl_float3 movement = {0, 0, 0};

        delta_time = (float)((NOW - LAST)*1000 / (float)SDL_GetPerformanceFrequency() );

        SDL_Event event;
        Render & renderer = renderers[renderer_index];
        Camera & camera = renderer.get_camera();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;

            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_F11:
                        fullscreen = !fullscreen;
                        toggle_fullscreen();
                        glViewport(0, 0,
                            fullscreen ? screen_width : win_width,
                            fullscreen ? screen_height : win_height
                        );
                        break;

                    case SDLK_ESCAPE:
                        running = false;
                        break;

                    case SDLK_UP:
                        camera.set_movement_speed(camera.get_movement_speed() * 1.5f);
                        break;

                    case SDLK_DOWN:
                        camera.set_movement_speed(camera.get_movement_speed() / 1.5f);
                        break;

                    default:
                        break;
                }
            }

            else if (event.type == SDL_MOUSEMOTION) {
                int m = std::min(fullscreen ? screen_width : win_width,
                                 fullscreen ? screen_height : win_height);

                float dx = (float)event.motion.xrel / (float)m;
                float dy = (float)event.motion.yrel / (float)m;

                camera.rotate(dx, -dy);
            }
        }

        SDL_PumpEvents();

        if (keyboard_state[SDL_SCANCODE_W])
            movement += camera.get_direction();

        if (keyboard_state[SDL_SCANCODE_S])
            movement -= camera.get_direction();

        if (keyboard_state[SDL_SCANCODE_A])
            movement -= camera.get_right();

        if (keyboard_state[SDL_SCANCODE_D])
            movement += camera.get_right();

        if (keyboard_state[SDL_SCANCODE_SPACE])
            movement += camera.get_up();

        if (keyboard_state[SDL_SCANCODE_LSHIFT])
            movement -= camera.get_up();

        camera.move(movement, delta_time);

        render(renderer);
    }

    SDL_DestroyWindow(window);
}

void App::toggle_fullscreen() {
    if (fullscreen) {
        if (SDL_SetWindowFullscreen(window, window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP))
            std::cerr << "Fullscreen mode error" << std::endl;
        else
            SDL_SetWindowSize(window, screen_width, screen_height);
    }
    else {
        SDL_SetWindowFullscreen(window, window_flags);
        SDL_SetWindowSize(window, win_width, win_height);
    }
}

void App::render(Render & renderer) {

    int width = fullscreen ? screen_width : win_width;
    int height = fullscreen ? screen_height : win_height;

    cl::Image2D & current_buffer = fullscreen ? fullscreen_image : image;

    glFinish();
    renderer.render(current_buffer, width, height);

    glLoadIdentity();

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fullscreen ? fullscreen_texture_id : texture_id);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0); glVertex2f(-1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(0, 1); glVertex2f(-1, 1);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    glFlush();

    SDL_GL_SwapWindow(window);
}
