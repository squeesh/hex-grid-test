#ifndef GAMEBOARD_CHUNK_H
#define GAMEBOARD_CHUNK_H



class GameboardChunk {
    private:
        Hexagon* base_hex;

        UniqueDataVector< GLdouble >* board_terrain_data;
        UniqueDataVector< GLdouble >* board_select_data;
        //UniqueDataVector< GLdouble >* board_object_data;

        void clear_terrain();
        void clear_select();
        //void clear_object();

        void generate_chunk_data();
        void generate_render_data(Hexagon*, GLdouble, GLdouble);

        void write_VBO_data();

    public:
        bool regenerate_terrain;
        bool regenerate_select;
        //bool regenerate_object;

        GameboardChunk(Hexagon*);
        ~GameboardChunk();

        void verify_render_data();

        void render();
};

#endif

