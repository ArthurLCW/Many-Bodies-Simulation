#include <graphic/graphic.hpp>
#include <imgui_impl_sdl.h>
#include <cstring>
#include <nbody/body.hpp>
#include <thread>
#include <chrono>

template <typename ...Args>
void UNUSED(Args&&... args [[maybe_unused]]) {}


void calculate_thread0(BodyPool& pool, int x_idx_0, int y_idx_0, double radius, double gravity){
//    std::string str = "inside thread: "+std::to_string(x_idx_0)+" "+std::to_string(y_idx_0)+"\n";
//    std::cout<<str;
    pool.check_and_update(pool.get_body(x_idx_0), pool.get_body(y_idx_0), radius, gravity);
}

void calculate_thread1(BodyPool& pool, int x_idx_1, double elapse, double space, double radius){
//    std::string str = "inside thread: "+std::to_string(x_idx_1)+"\n";
//    std::cout<<str;
    pool.get_body(x_idx_1).update_for_tick(elapse, space, radius);
}




int main(int argc, char **argv) {
    UNUSED(argc, argv);
    static int num_threads=4;
    static int bodies = 20;
    if (argc > 3) {
        std::cerr << "wrong arguments. please input only one argument as the number of threads" << std::endl;
        return 0;
    }else if (argc == 3){
        num_threads = std::stoi(argv[1]);
        bodies = std::stoi(argv[2]);
    }


    static float gravity = 100;
    static float space = 800;
    static float radius = 5;
    //static int bodies = 20;//20
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
        ImGui::Begin("Assignment 3", nullptr,
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

        // declare threads array
        std::vector<std::thread> thread_arr(num_threads);

        {
            const ImVec2 p = ImGui::GetCursorScreenPos();
            int x_idx_0=0;
            int y_idx_0=1;
            int x_idx_1=0;
            int thread_idx=0;
            int thread_used_0=num_threads;
            int iter_total = (current_bodies * (current_bodies + 1 )) / 2; // deal with 2 bodies (check_and_update) and 1 body (update_for_tick)
            int iter_num_0 = (current_bodies * (current_bodies - 1 )) / 2; // deal with 2 bodies (check_and_update)

            auto begin = std::chrono::high_resolution_clock::now();
            for (int iter=0; iter<iter_total; iter++){
                if (iter<iter_num_0){
//                    thread_arr[thread_idx] = std::thread(pool.check_and_update(get_body(x_idx_0), get_body(y_idx_0), radius, gravity));
                    //thread_arr[thread_idx] = std::thread(pool.check_and_update, pool.get_body(x_idx_0), pool.get_body(y_idx_0), radius, gravity);
                    //std::cout<<"outside:: "<<x_idx_0<<" "<<y_idx_0<<std::endl;

                    thread_arr[thread_idx]=std::thread(calculate_thread0, std::ref(pool), x_idx_0, y_idx_0, radius, gravity);

                    thread_idx++;
                    y_idx_0++;
                    if (y_idx_0 == current_bodies){
                        x_idx_0++;
                        y_idx_0 = x_idx_0+1;
                        if (x_idx_0==current_bodies) x_idx_0=0;
                    }

                    if (thread_idx == thread_used_0){
                        for (int i = 0; i < thread_used_0; i++){
                            thread_arr[i].join();
                        }
                        thread_idx = 0;
                    }

                    if (iter == (iter_num_0/num_threads)*num_threads){ // deal with special case where iter%num_threads!=0
                        thread_used_0 = iter_num_0 % num_threads;
                    }

                }

                else{//deal with the final body
                    //thread_arr[thread_idx] = std::thread(pool.get_body(x_idx_1).update_for_tick(elapse, space, radius));
//                    pool.get_body(x_idx_1).update_for_tick(elapse, space, radius);
//                    x_idx_1++;

//                    BodyPool::Body body_dealt = pool.get_body(x_idx_1);
//                    //thread_arr[thread_idx] = std::thread(BodyPool::Body::update_for_tick, &body_dealt, elapse, space, radius);
//                    thread_arr[thread_idx] = std::thread(&BodyPool::Body::update_for_tick, &body_dealt, elapse, space, radius);
//                    std::cout<<"outside: "<<x_idx_1<<std::endl;
//                    std::cout<<elapse<<std::endl;

                    thread_arr[thread_idx]=std::thread(calculate_thread1, std::ref(pool), x_idx_1, elapse, space, radius);

                    thread_idx++;
                    x_idx_1++;
                    if (thread_idx == num_threads){
                        for (int i = 0; i < num_threads; i++){
                            thread_arr[i].join();
                        }
                        thread_idx = 0;
                    }
                    else if (iter==iter_total-1){ // cannot be zhengchu
                        for (int i = 0; i < thread_idx; i++){
                            thread_arr[i].join();
                        }
                    }

                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            size_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
            //duration=0;
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
