#include <graphic/graphic.hpp>
#include <imgui_impl_sdl.h>
#include <cstring>
#include <nbody/body.hpp>
#include <omp.h>
#include <chrono>

template <typename ...Args>
void UNUSED(Args&&... args [[maybe_unused]]) {}

int main(int argc, char **argv) {
    UNUSED(argc, argv);
    static int num_threads=4;
    static int bodies = 20;//20
    if (argc > 3) {
        std::cerr << "wrong arguments. please input only one argument as the number of threads" << std::endl;
        return 0;
    }else if (argc == 3){
        num_threads = std::stoi(argv[1]);
        bodies = std::stoi(argv[2]);
    }
    omp_set_num_threads(num_threads);

    static float gravity = 100;
    static float space = 800;
    static float radius = 5;
//    static int bodies = 20;//20
    static float elapse = 0.001;
    static ImVec4 color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
    static float max_mass = 50;
    static float current_space = space;
    static float current_max_mass = max_mass;
    static int current_bodies = bodies;
    BodyPool pool(static_cast<size_t>(bodies), space, max_mass);
    graphic::GraphicContext context{"Assignment 2"};
    context.run([&](graphic::GraphicContext *context [[maybe_unused]], SDL_Window *) {
        auto io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::Begin("Assignment 2", nullptr,
                     ImGuiWindowFlags_NoMove
                     | ImGuiWindowFlags_NoCollapse
                     | ImGuiWindowFlags_NoTitleBar
                     | ImGuiWindowFlags_NoResize);
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::DragFloat("Space", &current_space, 10, 200, 1600, "%f");
        ImGui::DragFloat("Gravity", &gravity, 0.5, 0, 1000, "%f");
        ImGui::DragFloat("Radius", &radius, 0.5, 2, 20, "%f");
        ImGui::DragInt("Bodies", &current_bodies, 1, 2, 100, "%d");
        ImGui::DragFloat("Elapse", &elapse, 0.001, 0.001, 10, "%f");
        ImGui::DragFloat("Max Mass", &current_max_mass, 0.5, 5, 100, "%f");
        ImGui::ColorEdit4("Color", &color.x);
        if (current_space != space || current_bodies != bodies || current_max_mass != max_mass) {
            space = current_space;
            bodies = current_bodies;
            max_mass = current_max_mass;
            pool = BodyPool{static_cast<size_t>(bodies), space, max_mass};
        }
        {
            const ImVec2 p = ImGui::GetCursorScreenPos();

//            auto begin = std::chrono::high_resolution_clock::now();
//            pool.update_for_tick(elapse, gravity, space, radius);
//            auto end = std::chrono::high_resolution_clock::now();
//            size_t duration = duration_cast<std::chrono::nanoseconds>(end - begin).count();
//            duration=0;
            //std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;


//            const int sizea = 256;
//            double sinTable[sizea];
//            #pragma omp parallel for
//            for(int n=0; n<sizea; ++n)
//                sinTable[n] = std::sin(2 * M_PI * n / sizea);
//            std::cout<<"----------------------"<<std::endl;
//            #pragma omp parallel
//            {
//                // Code inside this region runs in parallel.
//                printf("Hello!\n");
//            }
//            std::cout<<"**********************"<<std::endl;

//            std::cout<<"----------------------"<<std::endl;
//            auto begin = std::chrono::high_resolution_clock::now();
//            size_t j;
//            #pragma omp parallel for private(j) shared(pool, radius, gravity)
//            {
//                for (size_t i = 0; i < pool.size(); ++i) {
//                    for (size_t j = i + 1; j < pool.size(); ++j) {
//                        std::string str="thread num: "+std::to_string(omp_get_thread_num())+", do "+std::to_string(i)+" "+std::to_string(j)+"\n";
//                        std::cout<<str;
//                        BodyPool::check_and_update(pool.get_body(i), pool.get_body(j), radius, gravity);
//                    }
//                }
//            }
//
//            #pragma omp parallel for shared(pool, elapse, space, radius)
//            {
//                for (size_t i = 0; i < pool.size(); ++i) {
//                    pool.get_body(i).update_for_tick(elapse, space, radius);
//                }
//            }
//            auto end = std::chrono::high_resolution_clock::now();
//            size_t duration = duration_cast<std::chrono::nanoseconds>(end - begin).count();
//            std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;


            //std::cout<<"----------------------"<<std::endl;
//            double x_idx_0=0;
//            double y_idx_0=1;
//            int iter_num_0 = (current_bodies * (current_bodies - 1 )) / 2;
//            auto begin = std::chrono::high_resolution_clock::now();
//
//            #pragma omp parallel for shared(pool, radius, gravity)
//            {
//                for (int iter=0; iter<iter_num_0;iter++){
//
//                    std::string str="thread num: "+std::to_string(omp_get_thread_num())+", do "+std::to_string(x_idx_0)+" "+std::to_string(y_idx_0)+"\n";
//                    std::cout<<str;
//                    BodyPool::check_and_update(pool.get_body(x_idx_0), pool.get_body(y_idx_0), radius, gravity);
//
//                    y_idx_0++;
//                    if (y_idx_0 == pool.size()){
//                        x_idx_0++;
//                        y_idx_0 = x_idx_0+1;
//                        if (x_idx_0==pool.size()) x_idx_0=0;
//                    }
//
//                }
//
//
//            }
//
//            #pragma omp parallel for shared(pool, elapse, space, radius)
//            {
//                for (size_t i = 0; i < pool.size(); ++i) {
//                    pool.get_body(i).update_for_tick(elapse, space, radius);
//                }
//            }
//            auto end = std::chrono::high_resolution_clock::now();
//            size_t duration = duration_cast<std::chrono::nanoseconds>(end - begin).count();
//            std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;

            auto begin = std::chrono::high_resolution_clock::now();
            //size_t j;

            {
                for (size_t i = 0; i < pool.size(); ++i) {
                    #pragma omp parallel for shared(pool, radius, gravity)
                    {
                        for (size_t j = i + 1; j < pool.size(); ++j) {
                            //std::string str="thread num: "+std::to_string(omp_get_thread_num())+", do "+std::to_string(i)+" "+std::to_string(j)+"\n";
                            //std::cout<<str;
                            BodyPool::check_and_update(pool.get_body(i), pool.get_body(j), radius, gravity);
                        }
                    }

                }
            }

            #pragma omp parallel for shared(pool, elapse, space, radius)
            {
                for (size_t i = 0; i < pool.size(); ++i) {
                    pool.get_body(i).update_for_tick(elapse, space, radius);
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            size_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
            std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;

            for (size_t i = 0; i < pool.size(); ++i) {
                auto body = pool.get_body(i);
                auto x = p.x + static_cast<float>(body.get_x());
                auto y = p.y + static_cast<float>(body.get_y());
                draw_list->AddCircleFilled(ImVec2(x, y), radius, ImColor{color});
            }
        }
        ImGui::End();
    });
}
