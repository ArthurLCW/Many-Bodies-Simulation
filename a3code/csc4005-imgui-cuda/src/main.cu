#include <graphic/graphic.hpp>
#include <imgui_impl_sdl.h>
#include <cstring>
#include <nbody/body.hpp>
#include <chrono>


__global__ void cuda_calaulate(int start_arr[], int idx_arr0[], int idx_arr1[], double m[],double x[],double y[],double vx[],double vy[], double conf[], double x_delta[],double y_delta[],double vx_delta[],double vy_delta[],double ax_delta[],double ay_delta[] ){
    if (blockIdx.x+start_arr[0] < start_arr[1]){
        int i = blockIdx.x+start_arr[0];
        int idx0 = idx_arr0[i];
        int idx1 = idx_arr1[i];
        //std::string str = "thread "+ std::to_string(idx0);

        double delta_x1=0;
        double delta_y1=0;
        double delta_vx1=0;
        double delta_vy1=0;
        double delta_ax1=0;
        double delta_ay1=0;

        double delta_x2=0;
        double delta_y2=0;
        double delta_vx2=0;
        double delta_vy2=0;
        double delta_ax2=0;
        double delta_ay2=0;

        double radius=conf[0];
        double gravity=conf[1];

        double m1=m[idx0];
        double x1=x[idx0];
        double y1=y[idx0];
        double vx1=vx[idx0];
        double vy1=vy[idx0];

        double m2=m[idx1];
        double x2=x[idx1];
        double y2=y[idx1];
        double vx2=vx[idx1];
        double vy2=vy[idx1];

        auto delta_x = x1-x2;
        auto delta_y = y1-y2;
        auto distance_square = delta_x * delta_x + delta_y * delta_y;
        auto ratio = 1 + 0.01;

        if (distance_square < radius * radius) {
            distance_square = radius * radius;
        }
        auto distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);
        if (distance < radius) {
            distance = radius;
        }

        if (delta_x * delta_x + delta_y * delta_y<=radius * radius) {
            auto dot_prod = delta_x * (vx1-vx2)
                            + delta_y * (vy1-vy2);
            auto scalar = 2 / (m1 + m2) * dot_prod / distance_square;

            delta_vx1 = -(scalar * delta_x * m2);
            delta_vy1 = -(scalar * delta_y * m2);
            delta_vx2 = +(scalar * delta_x * m1);
            delta_vy2 = +(scalar * delta_y * m1);
            // now relax the distance a bit: after the collision, there must be
            // at least (ratio * radius) between them
            delta_x1 = +(delta_x / distance * ratio * radius / 2.0);
            delta_y1 = +(delta_y / distance * ratio * radius / 2.0);
            delta_x2 = -(delta_x / distance * ratio * radius / 2.0);
            delta_y2 = -(delta_y / distance * ratio * radius / 2.0);
        }else {
            // update acceleration only when no collision
            auto scalar = gravity / distance_square / distance;
            delta_ax1 = -(scalar * delta_x * m2);
            delta_ay1 = -(scalar * delta_y * m2);
            delta_ax2 = +(scalar * delta_x * m1);
            delta_ay2 = +(scalar * delta_y * m1);
        }

        x_delta[idx0] = delta_x1;
        y_delta[idx0] = delta_y1;
        vx_delta[idx0] = delta_vx1;
        vy_delta[idx0] = delta_vy1;
        ax_delta[idx0] = delta_ax1;
        ay_delta[idx0] = delta_ay1;

        x_delta[idx1] = delta_x2;
        y_delta[idx1] = delta_y2;
        vx_delta[idx1] = delta_vx2;
        vy_delta[idx1] = delta_vy2;
        ax_delta[idx1] = delta_ax2;
        ay_delta[idx1] = delta_ay2;
    }

}

template <typename ...Args>
void UNUSED(Args&&... args [[maybe_unused]]) {}

int main(int argc, char **argv) {
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
//    static int bodies = 20;
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
            //pool.update_for_tick(elapse, gravity, space, radius);


            //transform m,x,y,vx,vy,ax,ay
            double *m = (double*)malloc(sizeof(double)*bodies);
            double *x = (double*)malloc(sizeof(double)*bodies);
            double *y = (double*)malloc(sizeof(double)*bodies);
            double *vx = (double*)malloc(sizeof(double)*bodies);
            double *vy = (double*)malloc(sizeof(double)*bodies);
            double *ax = (double*)malloc(sizeof(double)*bodies);
            double *ay = (double*)malloc(sizeof(double)*bodies);

            double *m_delta = (double*)malloc(sizeof(double)*bodies);
            double *x_delta = (double*)malloc(sizeof(double)*bodies);
            double *y_delta = (double*)malloc(sizeof(double)*bodies);
            double *vx_delta = (double*)malloc(sizeof(double)*bodies);
            double *vy_delta = (double*)malloc(sizeof(double)*bodies);
            double *ax_delta = (double*)malloc(sizeof(double)*bodies);
            double *ay_delta = (double*)malloc(sizeof(double)*bodies);

            double *conf = (double*)malloc(sizeof(double)*2);
            for (int i=0; i<bodies;i++){
                m[i]=pool.m[i];
                x[i]=pool.x[i];
                y[i]=pool.y[i];
                vx[i]=pool.vx[i];
                vy[i]=pool.vy[i];
                ax[i]=pool.ax[i];
                ay[i]=pool.ay[i];
            }
            conf[0]=radius;
            conf[1]=gravity;

            //calculate idx arr
            int *idx_arr0;
            int *idx_arr1;
            int iter_num_0 = ((bodies-1)*bodies)/2;
            idx_arr0=(int*)malloc(sizeof(int)*iter_num_0);
            idx_arr1=(int*)malloc(sizeof(int)*iter_num_0);
            int x_idx_0=0;
            int y_idx_0=1;
            for (int i=0; i<iter_num_0;i++){
                idx_arr0[i]=x_idx_0;
                idx_arr1[i]=y_idx_0;
                y_idx_0++;
                if (y_idx_0 == current_bodies){
                    x_idx_0++;
                    y_idx_0 = x_idx_0+1;
                    if (x_idx_0==current_bodies) x_idx_0=0;
                }
            }

            //transfer all arr into cuda_arr
            int *idx_arr0_cu;
            int *idx_arr1_cu;

            double *m_cu;
            double *x_cu;
            double *y_cu;
            double *vx_cu;
            double *vy_cu;
            double *ax_cu;
            double *ay_cu;

            double *m_delta_cu;
            double *x_delta_cu;
            double *y_delta_cu;
            double *vx_delta_cu;
            double *vy_delta_cu;
            double *ax_delta_cu;
            double *ay_delta_cu;

            double *conf_cu;

            //    cudaMalloc((void**)&pool_arr_cu, sizeof(pool));
            cudaMalloc((void**)&idx_arr0_cu, sizeof(double)*iter_num_0);
            cudaMalloc((void**)&idx_arr1_cu, sizeof(double)*iter_num_0);
            //    cudaMemcpy(pool_arr_cu, pool_arr, sizeof(pool), cudaMemcpyHostToDevice);
            cudaMemcpy(idx_arr0_cu, idx_arr0, sizeof(double)*iter_num_0, cudaMemcpyHostToDevice);
            cudaMemcpy(idx_arr1_cu, idx_arr1, sizeof(double)*iter_num_0, cudaMemcpyHostToDevice);

            cudaMalloc((void**)&m_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&x_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&y_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&vx_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&vy_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&ax_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&ay_cu,sizeof(double)*bodies);

            cudaMalloc((void**)&m_delta_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&x_delta_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&y_delta_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&vx_delta_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&vy_delta_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&ax_delta_cu,sizeof(double)*bodies);
            cudaMalloc((void**)&ay_delta_cu,sizeof(double)*bodies);

            cudaMemcpy(m_cu, m, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(x_cu, x, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(y_cu, y, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(vx_cu, vx, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(vy_cu, vy, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(ax_cu, ax, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(ay_cu, ay, sizeof(double)*bodies, cudaMemcpyHostToDevice);

            cudaMemcpy(m_delta_cu, m_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(x_delta_cu, x_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(y_delta_cu, y_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(vx_delta_cu, vx_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(vy_delta_cu, vy_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(ax_delta_cu, ax_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);
            cudaMemcpy(ay_delta_cu, ay_delta, sizeof(double)*bodies, cudaMemcpyHostToDevice);

            cudaMalloc((void**)&conf_cu,sizeof(double)*2);
            cudaMemcpy(conf_cu, conf, sizeof(double)*2, cudaMemcpyHostToDevice);

            //kernel calculation
            //dim3 dimGrid(iter_num_0);
            int* start_idx_arr_cu;
            int *start_idx_arr = (int*)malloc(sizeof(int)*2);
            cudaMalloc((void**)&start_idx_arr_cu,sizeof(int)*2);

//            int num_threads=3;
            dim3 dimGrid(num_threads);
            dim3 dimBlock(1);

            int small_iter_num = iter_num_0/num_threads;
            if (iter_num_0%num_threads!=0) small_iter_num=small_iter_num+1;

            auto begin = std::chrono::high_resolution_clock::now();
            for (int iter=0; iter<small_iter_num; iter++){
                start_idx_arr[0] = iter*num_threads;
                start_idx_arr[1] = iter_num_0;
                cudaMemcpy(start_idx_arr_cu, start_idx_arr, sizeof(int)*2, cudaMemcpyHostToDevice);
                //cout<<iter*num_threads<<endl;
                cuda_calaulate<<<dimGrid, dimBlock>>>(start_idx_arr_cu, idx_arr0_cu, idx_arr1_cu, m_cu, x_cu, y_cu, vx_cu, vy_cu, conf_cu, x_delta_cu,y_delta_cu,vx_delta_cu, vy_delta_cu,ax_delta_cu,ay_delta_cu);
            }


            cudaMemcpy(m_delta, m_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);
            cudaMemcpy(x_delta, x_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);
            cudaMemcpy(y_delta, y_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);
            cudaMemcpy(vx_delta, vx_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);
            cudaMemcpy(vy_delta, vy_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);
            cudaMemcpy(ax_delta, ax_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);
            cudaMemcpy(ay_delta, ay_delta_cu, sizeof(double)*bodies, cudaMemcpyDeviceToHost);

            for (int i=0; i<bodies;i++){
                pool.m[i]+=m_delta[i];
                pool.x[i]+=x_delta[i];
                pool.y[i]+=y_delta[i];
                pool.vx[i]+=vx_delta[i];
                pool.vy[i]+=vy_delta[i];
                pool.ax[i]+=ax_delta[i];
                pool.ay[i]+=ay_delta[i];
            }
            auto end = std::chrono::high_resolution_clock::now();
            size_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
            std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;

            for (size_t i = 0; i < pool.size(); ++i) {
                auto body = pool.get_body(i);
                body.update_for_tick(elapse, space, radius);
            }



            for (size_t i = 0; i < pool.size(); ++i) {
                auto body = pool.get_body(i);
                std::cout<<i<<": "<<" "<<body.get_x()<<" "<<body.get_y()<<" "<<body.get_vx()<<" "<<body.get_vy()<<" "<<body.get_ax()<<" "<<body.get_ay()<<std::endl;
                auto x = p.x + static_cast<float>(body.get_x());
                auto y = p.y + static_cast<float>(body.get_y());
                draw_list->AddCircleFilled(ImVec2(x, y), radius, ImColor{color});
            }
        }
        ImGui::End();
    });
}

