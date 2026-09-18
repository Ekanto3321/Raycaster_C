
const int screenWidth = 1000;
const int screenHeight = 800;
const int fps = 60;

bool mode = 0;
bool arcade = 0;
int enemy_cnt = 30;
const int gridSize = 25;

int jump = 0;

int rayPerDeg = 4;
float angleincrement;

int mapx = screenWidth/gridSize;
int mapy = screenHeight/gridSize; 

int map[200][200];

void generateMap();
int rectSize;

void controls();
void drawGrid();
void drawMap(); 
void drawPlayer();
float checkCol(int a, float x_dst, float y_dst, int taregt);
void editBlock(bool keystate);
void render2D();
void render3D();
void spawnBlockArcade(bool spawn);
void arcadeKill();



Vector2 playerPos = {screenWidth/2, screenHeight/2};
float rot = 45.0f; 
float rot_rad;
float sens = 0.24f;
float speed = 2.0f;
float rot_speed = 4.0f;

int heightscalar = 50;
float fov = 80;


