// PPU palette colors
// see http://wiki.nesdev.com/w/index.php/PPU_palettes
#define COLOR_AQUA    0x1c
#define COLOR_BLACK   0x0f
#define COLOR_BLUE    0x12
#define COLOR_LBLUE   0x21
#define COLOR_BROWN   0x17
#define COLOR_DGRAY   0x00
#define COLOR_GREEN   0x1a
#define COLOR_DGREEN  0x0B
#define COLOR_LIME    0x2a
#define COLOR_LGRAY   0x10
#define COLOR_MAGENTA 0x24
#define COLOR_MAROON  0x06
#define COLOR_NAVY    0x02
#define COLOR_OLIVE   0x18
#define COLOR_PURPLE  0x14
#define COLOR_RED     0x16
#define COLOR_TEAL    0x2c
#define COLOR_WHITE   0x30
#define COLOR_YELLOW  0x28
#define COLOR_ORANGE  0x26


//BALL variables
static unsigned char ball_x;
static unsigned char ball_y;
static unsigned char ball_dx;
static unsigned char ball_dy;

//PADDLE CONSTANTS and variables
#define PADDLE_X	    (14*SPRITE_WIDTH)
#define PADDLE_Y		(27*SPRITE_WIDTH)
#define PADDLE_MIN_X	24
#define PADDLE_MAX_X	200
static unsigned char pad;
static int paddle_dx;
static unsigned char paddle_x;
static unsigned char paddle_y;

//BORDER CONSTANTS
#define LEFT_BORDER_X	(3*SPRITE_WIDTH)
#define RIGHT_BORDER_X	(29*SPRITE_WIDTH)
#define TOP_BORDER_Y	(3*SPRITE_WIDTH)
#define BOTTOM_BORDER_Y	(29*SPRITE_WIDTH)


//WALL CONSTANTS AND VARIABLES
#define WALL_MIN_Y	5
#define WALL_MAX_Y	18
const unsigned char collisionPoints[9]={
0,		//No collision
60,		//1 - Orange brick - 60pts
70,		//2 - Light Blue brick- 70 pts
80,		//3 - Green brick - 80 pts
100,	//4 - Dark Blue brick - 100 pts
90,		//5 - Red brick - 90 pts
50, 	//6 - Grey brick - 50 pts 
110, 	//7 - Magenta brick - 110 pts 
120 	//8 - Yellow brick - 120 pts 
};
unsigned char collisionsTableLevel1[13*13]={
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6,
	5,5,5,5,5,5,5,5,5,5,5,5,5,
	8,8,8,8,8,8,8,8,8,8,8,8,8,
	4,4,4,4,4,4,4,4,4,4,4,4,4,
	7,7,7,7,7,7,7,7,7,7,7,7,7,
	3,3,3,3,3,3,3,3,3,3,3,3,3,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0
};
unsigned char collisionsTableLevel2[13*13]={
	1,0,0,0,0,0,0,0,0,0,0,0,0,
	1,2,0,0,0,0,0,0,0,0,0,0,0,
	1,2,3,0,0,0,0,0,0,0,0,0,0,
	1,2,3,4,0,0,0,0,0,0,0,0,0,
	1,2,3,4,5,0,0,0,0,0,0,0,0,
	1,2,3,4,5,1,0,0,0,0,0,0,0,
	1,2,3,4,5,1,2,0,0,0,0,0,0,
	1,2,3,4,5,1,2,3,0,0,0,0,0,
	1,2,3,4,5,1,2,3,4,0,0,0,0,
	1,2,3,4,5,1,2,3,4,5,0,0,0,
	1,2,3,4,5,1,2,3,4,5,1,0,0,
	1,2,3,4,5,1,2,3,4,5,1,2,0,
	6,6,6,6,6,6,6,6,6,6,6,6,6
};
unsigned collisionsTable[13*13]={0};
static int collision_type;
static int collision_case=-1;

//VARIABLES FOR SCREEN UPDATES
static unsigned char digits[5]={0x00,0x00,0x00,0x00,0x00};

static unsigned char updates_init[8+(5*(13*2))+4+1]={

	//Memory address and sprites for scores update
	MSB(NTADR_A(6,1))|NT_UPD_HORZ,LSB(NTADR_A(6,1)),5,0x10,0x10,0x00,0x00,0x00,
	//Memory address and sprites for brick update
	MSB(NTADR_A(0,0))|NT_UPD_HORZ,LSB(NTADR_A(0,0)),2,0x00,0x00,
	//
	NT_UPD_EOF
};

//GAME STATE CONSTANTS AND VARIABLES
#define GAME_NOT_STARTED	0
#define LEVEL1_NOT_STARTED	1
#define LEVEL1_STARTED	2
#define LEVEL2_NOT_STARTED	3
#define LEVEL2_STARTED	4
#define GAME_OVER	5


#define GAME_START_EVENT 1
#define PADDLE_LEFT_EVENT 2
#define PADDLE_RIGHT_EVENT 3
#define START_LEVEL1_EVENT 4
#define END_LEVEL1_EVENT 5 
#define START_LEVEL2_EVENT 6
#define END_LEVEL2_EVENT 7
#define TOP_BORDER_COLLISION_EVENT 8
#define RIGHT_BORDER_COLLISION_EVENT 9
#define LEFT_BORDER_COLLISION_EVENT 10
#define PADDLE_LEFT_PART_COLLISION_EVENT 11
#define PADDLE_RIGHT_PART_COLLISION_EVENT 12
#define PADDLE_CENTER_PART_COLLISION_EVENT 13
#define BRICK_COLLISION_EVENT 14
#define FAILED_PARTY_EVENT 15


static unsigned char game_state; 
static int level_lives;
static int paddles=-1;



//SPRITES constants and variables
#define SPRITE_WIDTH	8
static unsigned char spr;
typedef struct event {
	unsigned char event_type;
} event_t;
const unsigned char sprVaus[]={
	0,0,0xC0,0x04,
	8,0,0xC1,0x05,
	16,0,0xC1,0x05,
	24,0,0xC2,0x04,
	128
};
const unsigned char sprSmallVaus[]={
	0,0,0xE0,0x04,
	8,0,0xE1,0x05,
	16,0,0xE2,0x04,
	128
};
const unsigned char sprBall[]={
	0,0,0xAA,0x04,
	128
};
const unsigned char leftBricksBorder[]={
	16,8*0,0xB2,0x06,
	16,8*1,0xB2,0x06,
	16,8*2,0xD5,0x06,
	16,8*3,0xE5,0x06,
	16,8*4,0xF5,0x06,
	16,8*5,0xB2,0x06,
	16,8*6,0xB2,0x06,
	16,8*7,0xB2,0x06,
	16,8*8,0xD5,0x06,
	16,8*9,0xE5,0x06,
	16,8*10,0xF5,0x06,
	16,8*11,0xB2,0x06,
	16,8*12,0xB2,0x06,
	16,8*13,0xB2,0x06,
	128
};
const unsigned char rightBricksBorder[]={
	232,8*0,0xB3,0x06,
	232,8*1,0xB3,0x06,
	232,8*2,0xD6,0x06,
	232,8*3,0xE6,0x06,
	232,8*4,0xF6,0x06,
	232,8*5,0xB3,0x06,
	232,8*6,0xB3,0x06,
	232,8*7,0xB3,0x06,
	232,8*8,0xD6,0x06,
	232,8*9,0xE6,0x06,
	232,8*10,0xF6,0x06,
	232,8*11,0xB3,0x06,
	232,8*12,0xB3,0x06,
	232,8*13,0xB3,0x06,
	128
};
const unsigned char rightBricksBorderLevel2[]={
	232,8*12,0xB3,0x06,
	232,8*13,0xB3,0x06,
	128
};
const unsigned char row3Level2[]={
	7*8,8*0,0xBA,0x02,
	7*8,8*1,0xBA,0x02,
	7*8,8*2,0xBA,0x02,
	7*8,8*3,0xBA,0x02,
	7*8,8*4,0xBA,0x02,
	7*8,8*5,0xBA,0x02,
	7*8,8*6,0xBA,0x02,
	7*8,8*7,0xBA,0x02,
	7*8,8*8,0xBA,0x02,
	7*8,8*9,0xBA,0x02,
	128
};
const unsigned char row5Level2[]={
	11*8,8*0,0xBA,0x00, 12*8,8*0,0xBB,0x00,   
	11*8,8*1,0xBA,0x00, 12*8,8*1,0xBB,0x00,
	11*8,8*2,0xBA,0x00, 12*8,8*2,0xBB,0x00,
	11*8,8*3,0xBA,0x00, 12*8,8*3,0xBB,0x00,
	11*8,8*4,0xBA,0x00, 12*8,8*4,0xBB,0x00,
	11*8,8*5,0xBA,0x00, 12*8,8*5,0xBB,0x00,
	11*8,8*6,0xBA,0x00, 12*8,8*6,0xBB,0x00,
	11*8,8*7,0xBA,0x00, 12*8,8*7,0xBB,0x00,
	128
};
const unsigned char row7Level2[]={
	15*8,8*0,0xA8,0x00, 
	15*8,8*1,0xA8,0x00, 
	15*8,8*2,0xA8,0x00, 
	15*8,8*3,0xA8,0x00, 
	15*8,8*4,0xA8,0x00, 
	15*8,8*5,0xA8,0x00, 
	128
};
const unsigned char row10Level2[]={
	23*8,8*0,0xB8,0x03, 
	23*8,8*1,0xB8,0x03, 
	128
};


const unsigned char palSprites[16]={
	COLOR_BLACK,
    COLOR_RED, COLOR_BLUE, COLOR_WHITE, 
    COLOR_BLACK,
    COLOR_WHITE, COLOR_LGRAY, COLOR_DGRAY, 
    //                         
    0,                                    
    COLOR_GREEN, COLOR_LGRAY, COLOR_WHITE,                            
    0,                                    
    COLOR_LBLUE, COLOR_GREEN, COLOR_ORANGE, 
};
const unsigned char palBG0 [16]={
	COLOR_BLACK,COLOR_DGREEN,COLOR_RED,COLOR_WHITE,
	COLOR_BLACK,COLOR_BLUE,COLOR_LGRAY,COLOR_BLUE,
	COLOR_BLACK,COLOR_BLUE,COLOR_LGRAY,COLOR_BLUE,
	COLOR_BLACK,COLOR_BLUE,COLOR_LGRAY,COLOR_BLUE,
};
const unsigned char palBG1 [16]={
	COLOR_BLACK,COLOR_BLUE,COLOR_RED,COLOR_WHITE,
	COLOR_BLACK,COLOR_BLUE,COLOR_LGRAY,COLOR_WHITE,
	COLOR_BLACK,COLOR_BLUE,COLOR_RED,COLOR_YELLOW,
	COLOR_BLACK,COLOR_BLUE,COLOR_MAGENTA,COLOR_GREEN,
};

const unsigned char palBG2 [16]={
	COLOR_BLACK,COLOR_DGREEN,COLOR_RED,COLOR_WHITE,
	COLOR_BLACK,COLOR_DGREEN,COLOR_LGRAY,COLOR_WHITE,
	COLOR_BLACK,COLOR_DGREEN,COLOR_ORANGE,COLOR_LBLUE,
	COLOR_BLACK,COLOR_DGREEN,COLOR_GREEN,COLOR_BLUE,
};


