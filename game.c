#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#define BACKGROUND_COLOR (Color){180, 150, 130}
#define CAR_COLOR BLACK
#define ROTATION_SPEED 20

int main(){

    int width = 1300;
    int height = 1000;
    int world_width = 5000;
    int world_height = 5000;
    InitWindow(width, height, "Racer");
    SetTargetFPS(60);

    Image soil_image = LoadImage("craftpix-889156-free-racing-game-kit/PNG/Background_Tiles/Soil_Tile.png");
    Texture2D soil_text = LoadTextureFromImage(soil_image);

    Image car_image = LoadImage("craftpix-889156-free-racing-game-kit/PNG/Car_1_Main_Positions/Car_1_01.png");
    ImageRotateCW(&car_image);
    Texture2D car_text = LoadTextureFromImage(car_image);
    Rectangle car_text_rec = {
        .x = 0,
        .y = 0,
        .width = car_text.width,
        .height = car_text.height
    };

    int car_width = 75;
    int car_height = 150;
    float car_x = width/2 - car_width/2;
    float car_y = height/2 - car_height/2;
    float car_speed = 0.0;
    float CAR_MAX_SPEED = 10.0;
    int car_direction = -1;
    float car_rotation = -90.0;
    float car_speedUp = 10.0;
    float car_slowDown = 30.0;

    float radians = 0.0;
    float x_move = 0.0;
    float y_move = 0.0;

    Camera2D camera = {
        .offset = (Vector2){0, 0},
        .target = (Vector2){0, 0},
        .rotation = 0.0,
        .zoom = 1.0
    };

    while(!WindowShouldClose()){
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        BeginMode2D(camera);

        int tile_count_col = ceil(world_width / soil_text.width);
        int tile_count_row = ceil(world_height / soil_text.height);

        for(int x = 0; x < tile_count_col; x++){
            for(int y = 0; y < tile_count_row; y++){
                DrawTexture(soil_text, x * soil_text.width, y * soil_text.height, WHITE);    
            }
        }

        if(IsKeyDown(KEY_UP)){
            car_direction = -1;
            car_speed += car_speedUp * dt;
            if(car_speed > CAR_MAX_SPEED){
                car_speed = CAR_MAX_SPEED;
            }
        }
        else if(IsKeyDown(KEY_DOWN)){
            car_direction = 1;
            car_speed -= car_speedUp * dt;
            if(car_speed < (-CAR_MAX_SPEED)){
                car_speed = -CAR_MAX_SPEED;
            }
        }
        else {
            car_speed += car_slowDown * dt * car_direction;
            if(car_direction == -1){
                if(car_speed < 0){
                    car_speed = 0.0;
                }   
            }
            else if (car_direction == 1){
                if(car_speed > 0){
                    car_speed = 0.0;
                }
            }
        }

        if(car_speed > 0 || car_speed < 0){
            if(IsKeyDown(KEY_LEFT)){
                car_rotation -= ROTATION_SPEED * dt * car_speed;
            }
            else if(IsKeyDown(KEY_RIGHT)){
                car_rotation += ROTATION_SPEED * dt * car_speed;
            }
        }

        if(abs(car_rotation) >= 360){
            car_rotation = 0.0;
        }
        // printf("Rotation = %f\n", car_rotation);

        radians = (PI * car_rotation) / 180.0;
        x_move = car_speed * cosf(radians);
        y_move = car_speed * sinf(radians);

        car_x += x_move;
        car_y += y_move;

        Rectangle car_rec = {
            .x = car_x,
            .y = car_y,
            .width = car_height,
            .height = car_width
        };
        Vector2 car_position = {
            .x = car_height/2,
            .y = car_width/2
        };

        DrawTexturePro(car_text, car_text_rec, car_rec, car_position, car_rotation, WHITE);

        camera.target.x = car_rec.x;
        camera.target.y = car_rec.y;
        camera.offset.x = 700;
        camera.offset.y = 500;

        EndDrawing();
    }

    UnloadImage(car_image);
    UnloadTexture(car_text);

    UnloadImage(soil_image);
    UnloadTexture(soil_text);

    CloseWindow();

    return 0;
}