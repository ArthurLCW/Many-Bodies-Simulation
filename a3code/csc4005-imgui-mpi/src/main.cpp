#include <graphic/graphic.hpp>
#include <imgui_impl_sdl.h>
#include <cstring>
#include <nbody/body.hpp>
#include <mpi.h>
#include <chrono>

template <typename ...Args>
void UNUSED(Args&&... args [[maybe_unused]]) {}

double* check_and_update_mpi(double *data){
    double* delta_value = new double[14]; //first 6 for ball 1, second 6 for ball 2

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


    //get the data of 2 balls
    double radius = data[2];
    double gravity = data[3];

    double x1 = data[4];
    double y1 = data[5];
    double vx1 = data[6];
    double vy1 = data[7];
//        double ax1 = data[8];
//        double ay1 = data[9];
    double m1 = data[10];

    double x2 = data[11];
    double y2 = data[12];
    double vx2 = data[13];
    double vy2 = data[14];
//        double ax2 = data[15];
//        double ay2 = data[16];
    double m2 = data[17];


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


    delta_value[0] = delta_x1;
    delta_value[1] = delta_y1;
    delta_value[2] = delta_vx1;
    delta_value[3] = delta_vy1;
    delta_value[4] = delta_ax1;
    delta_value[5] = delta_ay1;

    delta_value[6] = delta_x2;
    delta_value[7] = delta_y2;
    delta_value[8] = delta_vx2;
    delta_value[9] = delta_vy2;
    delta_value[10] = delta_ax2;
    delta_value[11] = delta_ay2;

    delta_value[12] = data[0];
    delta_value[13] = data[1];


    return delta_value;
}


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank_num;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &rank_num);
    //std::cout<<"rank-num; "<<rank_num<<std::endl;


    if (rank == 0){
        static float gravity = 100;
        static float space = 800;
        static float radius = 5;
        static int bodies = 20;
        static float elapse = 0.001;
        static ImVec4 color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
        static float max_mass = 50;
        static float current_space = space;
        static float current_max_mass = max_mass;
        static int current_bodies = bodies;
        BodyPool pool(static_cast<size_t>(bodies), space, max_mass);
        graphic::GraphicContext context{"Assignment 3"};

        context.run([&](graphic::GraphicContext *context [[maybe_unused]], SDL_Window *) {
            //
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
            {
                const ImVec2 p = ImGui::GetCursorScreenPos();
                double *send_arr = new double[18];
                double *keep_arr = new double[18];
                double *recv_arr = new double[14];
                int *go_on_flag_recv = new int[1];

                int iter_num_0 = (current_bodies * (current_bodies - 1 )) / 2; // deal with 2 bodies (check_and_update)
                double x_idx_0=0;
                double y_idx_0=1;

                int proc_idx=0;
                int proc_used=rank_num;
                int proc_offset = iter_num_0%rank_num;

                auto begin = std::chrono::high_resolution_clock::now();
                for (int iter=0; iter<iter_num_0; iter++){

                    // assign value to the damn send arr
                    {
                        send_arr[0]=x_idx_0;
                        send_arr[1]=y_idx_0;// first 2 denotes the chosen two ball idx
                        send_arr[2]=radius;
                        send_arr[3]=gravity;// 2,3: denotes two variables set-up in graph
                        send_arr[4]=pool.x[x_idx_0];//4-10: x,y,vx,vy,ax,ay,m for first ball
                        send_arr[5]=pool.y[x_idx_0];
                        send_arr[6]=pool.vx[x_idx_0];
                        send_arr[7]=pool.vy[x_idx_0];
                        send_arr[8]=pool.ax[x_idx_0];
                        send_arr[9]=pool.ay[x_idx_0];
                        send_arr[10]=pool.m[x_idx_0];
                        send_arr[11]=pool.x[y_idx_0];//11-17: x,y,vx,vy,ax,ay,m for second ball
                        send_arr[12]=pool.y[y_idx_0];
                        send_arr[13]=pool.vx[y_idx_0];
                        send_arr[14]=pool.vy[y_idx_0];
                        send_arr[15]=pool.ax[y_idx_0];
                        send_arr[16]=pool.ay[y_idx_0];
                        send_arr[17]=pool.m[y_idx_0];
                    }

                    //modify idx
                    y_idx_0++;
                    if (y_idx_0 == current_bodies){
                        x_idx_0++;
                        y_idx_0 = x_idx_0+1;
                        if (x_idx_0==current_bodies) x_idx_0=0;
                    }

                    //send_data, or calculate if the current proc_idx=0
                    if (proc_idx==0){//if 0, keep the data and calculate after sending data to other proc
                        for (int i=0; i<18; i++){
                            keep_arr[i]=send_arr[i];
                        }
                        proc_idx++;
                    }
                    else{
//                        std::cout << rank << " before send"<<std::endl;
//                        std::cout  << " process id: "<<proc_idx<<std::endl;

                        MPI_Send(send_arr, 18, MPI_DOUBLE, proc_idx, 0, MPI_COMM_WORLD);
//                        std::cout << rank << " after send"<<std::endl;
                        proc_idx++;
                    }


                    if (proc_idx==proc_used){
                        //finish sending data, calculate data which should be taken care of by rank 0
                        {
                            recv_arr = check_and_update_mpi(keep_arr);
                            pool.x[recv_arr[12]] += recv_arr[0];
                            pool.y[recv_arr[12]] += recv_arr[1];
                            pool.vx[recv_arr[12]] += recv_arr[2];
                            pool.vy[recv_arr[12]] += recv_arr[3];
                            pool.ax[recv_arr[12]] += recv_arr[4];
                            pool.ay[recv_arr[12]] += recv_arr[5];

                            pool.x[recv_arr[13]] += recv_arr[6];
                            pool.y[recv_arr[13]] += recv_arr[7];
                            pool.vx[recv_arr[13]] += recv_arr[8];
                            pool.vy[recv_arr[13]] += recv_arr[9];
                            pool.ax[recv_arr[13]] += recv_arr[10];
                            pool.ay[recv_arr[13]] += recv_arr[11];
                        }

                        //Recv calculation result from others!
                        for (int i=1; i<proc_used; i++){

                            MPI_Recv(recv_arr, 14, MPI_DOUBLE, i, 666, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                            //std::cout << rank << " after MPI_Recv"<<std::endl;
                            //then put data into the body-pool
                            {
                                recv_arr = check_and_update_mpi(send_arr);
                                pool.x[recv_arr[12]] += recv_arr[0];
                                pool.y[recv_arr[12]] += recv_arr[1];
                                pool.vx[recv_arr[12]] += recv_arr[2];
                                pool.vy[recv_arr[12]] += recv_arr[3];
                                pool.ax[recv_arr[12]] += recv_arr[4];
                                pool.ay[recv_arr[12]] += recv_arr[5];

                                pool.x[recv_arr[13]] += recv_arr[6];
                                pool.y[recv_arr[13]] += recv_arr[7];
                                pool.vx[recv_arr[13]] += recv_arr[8];
                                pool.vy[recv_arr[13]] += recv_arr[9];
                                pool.ax[recv_arr[13]] += recv_arr[10];
                                pool.ay[recv_arr[13]] += recv_arr[11];
                            }
                        }

                        for (int i=1; i<proc_used; i++){
                            MPI_Send(go_on_flag_recv, 1, MPI_INT, i, 520, MPI_COMM_WORLD);
//                            std::cout << rank << " after send signal"<<std::endl;
                        }

                        if (proc_offset!=0 && iter==iter_num_0-proc_offset){
                            proc_used = proc_offset;
                        }
                        proc_idx=0;

                    }
                }
                auto end = std::chrono::high_resolution_clock::now();
                size_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
                std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;

                for (size_t i = 0; i < pool.size(); ++i) {
                    auto body = pool.get_body(i);
                    body.update_for_tick(elapse, space, radius);
                }


//                auto begin = std::chrono::high_resolution_clock::now();
//                pool.update_for_tick(elapse, gravity, space, radius);
//                auto end = std::chrono::high_resolution_clock::now();
//                size_t duration = duration_cast<std::chrono::nanoseconds>(end - begin).count();
//                std::cout<<"calculation time: "<<duration<<" nanoseconds per iteration."<<std::endl;

                for (size_t i = 0; i < pool.size(); ++i) {
                    auto body = pool.get_body(i);
                    auto x = p.x + static_cast<float>(body.get_x());
                    auto y = p.y + static_cast<float>(body.get_y());
                    draw_list->AddCircleFilled(ImVec2(x, y), radius, ImColor{color});
                }


            }
            ImGui::End();
            //

        });
    }
    else if (rank != 0){
        double *recv_arr_from0 = new double[18];
        double *send_arr_to0 = new double[14];
        int *go_on_flag_recv = new int[1];

        while (0==0){

            // first raw recv data from rank 0
//            std::cout << rank << " before recv"<<std::endl;
            MPI_Recv(recv_arr_from0, 18, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            send_arr_to0 = check_and_update_mpi(recv_arr_from0);
//            std::cout << rank << " after recv"<<std::endl;
            MPI_Send(send_arr_to0, 14, MPI_DOUBLE, 0, 666, MPI_COMM_WORLD);
//            std::cout << rank << " after send"<<std::endl;

            MPI_Recv(go_on_flag_recv, 1, MPI_INT, 0, 520, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            std::cout << rank << " after recv signal"<<std::endl;
        }


    }

    MPI_Finalize();
}
