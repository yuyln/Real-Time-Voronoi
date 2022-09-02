#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include "vecs.h"
#define OPENCLWRAPPER_IMPLEMENTATION
#include "OpenCLWrapper.h"

#define WIDTH 600
#define HEIGHT 600

static const char *kernels[] = {"voronoi"};

typedef struct 
{
    cl_platform_id *plats; size_t n_plats; int i_plat;
    cl_device_id *devices; size_t n_devs; int i_dev;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    Kernel *kernels;
} OpenCLT;

OpenCLT InitOpenCL(int i_plat, int i_dev)
{
    OpenCLT ret;
    ret.i_dev = i_dev;
    ret.i_plat = i_plat;
    ret.plats = InitPlatforms(&ret.n_plats);

    for (size_t i = 0; i < ret.n_plats; ++i)
        PlatformInfo(stdout, ret.plats[i], i);

    ret.devices = InitDevices(ret.plats[i_plat], &ret.n_devs);
    for (size_t i = 0; i < ret.n_devs; ++i)
        DeviceInfo(stdout, ret.devices[i], i);
    
    ret.context = InitContext(ret.devices, ret.n_devs);
    ret.queue = InitQueue(ret.context, ret.devices[i_dev]);

    char* kernel_file;
    ReadFileOpenCL("./kernel.cl", &kernel_file);

    ret.program = InitProgramSource(ret.context, (const char*)kernel_file);

    size_t n = snprintf(NULL, 0, "-I ./ -DHEIGHT=%d -DWIDTH=%d", HEIGHT, WIDTH) + 1;
    char *opt = (char*)calloc(n, 1);
    snprintf(opt, n, "-I ./ -DHEIGHT=%d -DWIDTH=%d", HEIGHT, WIDTH);
    printf("%s\n", opt);

    cl_int err = BuildProgram(ret.program, ret.n_devs, ret.devices, opt);
    BuildProgramInfo(stdout, ret.program, ret.devices[i_dev], err);
    ret.kernels = InitKernels(ret.program, kernels, sizeof(kernels) / sizeof(char*));
    return ret;
}

int main()
{
    srand(time(NULL));
    OpenCLT opencl = InitOpenCL(0, 0);

    GLFWwindow *window;
    if (!glfwInit())
    {
        fprintf(stderr, "GLFW could not init\n");
        return 1;
    }
    window = glfwCreateWindow(WIDTH, HEIGHT, "voronoi", NULL, NULL);

    if (!window)
        glfwTerminate();

    glfwMakeContextCurrent(window);
    glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Vec3f *pixels = (Vec3f*)calloc(WIDTH * HEIGHT, sizeof(Vec3f));
    cl_mem pixels_buffer = CreateBuffer(sizeof(Vec3f) * WIDTH * HEIGHT, opencl.context, CL_MEM_READ_WRITE);
    Vec3d mouse_pos = Vec3dFromScalar(0.0);

    int n_points = 200;
    Vec3f *points = (Vec3f*)calloc(n_points, sizeof(Vec3f));
    for (int i = 0; i < n_points; ++i)
        points[i] = Vec3fFrom(rand() % WIDTH, rand() % HEIGHT, 0);

    cl_mem points_buffer = CreateBuffer(sizeof(Vec3f) * n_points, opencl.context, CL_MEM_READ_WRITE);
    WriteBuffer(points_buffer, points, sizeof(Vec3f) * n_points, 0, opencl.queue);
    size_t global = WIDTH * HEIGHT;
    size_t local = gcd(global, 512);

    SetKernelArg(opencl.kernels[0], 0, sizeof(cl_mem), &pixels_buffer);
    SetKernelArg(opencl.kernels[0], 1, sizeof(cl_mem), &points_buffer);
    SetKernelArg(opencl.kernels[0], 2, sizeof(int), &n_points);

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
        mouse_pos.y = interp(mouse_pos.y, 0, HEIGHT, HEIGHT, 0);
        SetKernelArg(opencl.kernels[0], 3, sizeof(Vec3d), &mouse_pos);
        EnqueueND(opencl.queue, opencl.kernels[0], 1, NULL, &global, &local);
        Finish(opencl.queue);
        ReadBuffer(pixels_buffer, pixels, sizeof(Vec3f) * global, 0, opencl.queue);
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels);

        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >=1.0)
        {
            double sf = 1000.0/(double)(nbFrames);
            printf("%f fps\n", 1000 / sf);
            nbFrames = 0;
            lastTime += 1.0;
        }

        glfwSwapInterval(0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    free(points);
    free(pixels);

    PrintCLError(stderr, clReleaseMemObject(pixels_buffer), "ERROR ON RELEASING PIXELS BUFFER");
    PrintCLError(stderr, clReleaseMemObject(points_buffer), "ERROR ON RELEASING POINTS BUFFER");
    PrintCLError(stderr, clReleaseCommandQueue(opencl.queue), "ERROR ON RELEASING QUEUE");
    PrintCLError(stderr, clReleaseContext(opencl.context), "ERROR ON RELEASING CONTEXT");
    for (size_t i = 0; i < opencl.n_devs; ++i)
        PrintCLError(stderr, clReleaseDevice(opencl.devices[i]), "ERROR ON RELEASING DEVICE[%zu]", i);
    PrintCLError(stderr, clReleaseKernel(opencl.kernels->kernel), "ERROR ON RELEASING KERNEL");
    PrintCLError(stderr, clReleaseProgram(opencl.program), "ERROR ON RELEASING PROGRAM");

}
