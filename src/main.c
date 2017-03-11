#include "base/neslib.h"
#include "data.h"
#include "welcome.h"
#include "startlevel1.h"
#include "gamelevel1.h"
#include "startlevel2.h"
#include "gamelevel2.h"
#include "gameover.h"
#include "gameov.h"

//PROTOTYPES
void controller(void);
void interceptKeyboardEvents(void);
void interceptGameEvents(void);
//

void paddleLeftPartCollisionEvent(void);
void paddleRightPartCollisionEvent(void);
void paddleCenterPartCollisionEvent(void);
void topBorderCollisionEvent(void);
void rightBorderCollisionEvent(void);
void leftBorderCollisionEvent(void);
void brickCollisionEvent(void);
void updateScoresEvent(void);
void ballFallsOutEvent(void);
void failedPartyEvent(void);
//
void initPaddles(void);
void initBall(void);
void playActivity(void);
void interceptWallCollisions(void);
void interceptCollisions(void);
void countBricksInLevel(void);
void restoreWwallState(void);
//
void movePaddleRightAction(void);
void movePaddleLeftAction(void);
//
void paddleLeftPartCollisionAction(void);
void paddleRightPartCollisionAction(void);
void paddleCenterPartCollisionAction(void);
void topBorderCollisionAction(void);
void rightBorderCollisionAction(void);
void leftBorderCollisionAction(void);
void brickCollisionAction(void);
void failedPartyAction(void);
//
//GAME STATES ACTIONS
void gameNotStartedEntryAction(void);
void gameNotStartedExitAction(void);
//
void level1NotStartedEntryAction(void);
void level1NotStartedExitAction(void);
//
void level1StartedEntryAction(void);
void level1StartedExitAction(void);
//
void level2NotStartedEntryAction(void);
void level2NotStartedExitAction(void);
//
void level2StartedEntryAction(void);
void level2StartedExitAction(void);
//
void gameOverEntryAction(void);
void gameOverExitAction(void);

//system vars used everywhere as well
static unsigned char frame_cnt;
static unsigned char bright;


unsigned char left_bricks;
unsigned char hits;
unsigned char score;
unsigned char points;
#define STRING_OFFSET  0x100

static event_t event;
//variables   
const char *lost= "AAAA";

static unsigned char frame;
static unsigned char i, j,bg_bright,spr_bright;
static unsigned char coord_x, coord_y;


void put_str(unsigned int adr,const char *str)
{
	vram_adr(adr);
	while(1)
	{
		if(!*str) break;
		vram_put((*str++) + STRING_OFFSET);//-0x20 because ASCII code 0x20 is placed in tile 0 of the CHR
	}
}

void fade_screen_in(void)
{
	for(i=0;i<16;i++)
	{
		ppu_wait_nmi();
		if(!(i&3))
		{
			bg_bright++;
			pal_bg_bright(bg_bright);
			spr_bright++;
			pal_spr_bright(spr_bright);
		}
	}
}

void fade_screen_out(void)
{
	for(i=0;i<16;i++)
	{
		ppu_wait_nmi();
		if(!(i&3)) {
			bg_bright--;
			pal_bg_bright(bg_bright);
			spr_bright--;
			pal_spr_bright(spr_bright);
		}
	}
}
/*#### KEYBOARD EVENTS  ################################################################
* PAD_START_EVENT : Player has pressed START key. 
* PAD_LEFT_EVENT : Player has pressed LEFT key
* PAD_RIGHT_EVENT : Player has pressed RIGHT key
#######################################################################################*/
void padStartEvent() {
	if (game_state == GAME_NOT_STARTED) {
		event.event_type = GAME_START_EVENT;
	}
}
void padLeftEvent() {
	if (game_state == LEVEL1_STARTED) {
		event.event_type = PADDLE_LEFT_EVENT;
	}
	else if (game_state == LEVEL2_STARTED) {
		event.event_type = PADDLE_LEFT_EVENT;
	}
}
void padRightEvent() {
	if (game_state == LEVEL1_STARTED) {
		event.event_type = PADDLE_RIGHT_EVENT;
	}
	else if (game_state == LEVEL2_STARTED) {
		event.event_type = PADDLE_RIGHT_EVENT;
	}
}
/*#### GAME EVENTS  ################################################################
* START_GAME_EVENT : Will be ignored if game is already started
* PADDLE_LEFT_EVENT : Will be ignored if not in LEVEL1_STARTED or LEVEL2_STARTED state
* PADDLE_RIGHT_EVENT : Will be ignored if not in LEVEL1_STARTED or LEVEL2_STARTED state
#######################################################################################*/

void paddleLeftPartCollisionEvent() {
	event.event_type = PADDLE_LEFT_PART_COLLISION_EVENT;
}
void paddleRightPartCollisionEvent() {
	event.event_type = PADDLE_RIGHT_PART_COLLISION_EVENT;
}
void paddleCenterPartCollisionEvent() {
	event.event_type = PADDLE_CENTER_PART_COLLISION_EVENT;
}
void topBorderCollisionEvent() {
	event.event_type = TOP_BORDER_COLLISION_EVENT;
}
void leftBorderCollisionEvent() {
	event.event_type = LEFT_BORDER_COLLISION_EVENT;
}
void rightBorderCollisionEvent() {
	event.event_type = RIGHT_BORDER_COLLISION_EVENT;
}
void collisionWithBrickEvent() {
	event.event_type = BRICK_COLLISION_EVENT;
}
void ballFallsOutEvent() {
	if (game_state == LEVEL1_STARTED) {
		event.event_type = END_LEVEL1_EVENT;
	}
	else if (game_state == LEVEL2_STARTED) {
		event.event_type = END_LEVEL2_EVENT;
	}
}
void failedPartyEvent() {
	event.event_type = FAILED_PARTY_EVENT;
}
/*#### GAME ENTRY POINT ################################################################
#######################################################################################*/
void main(void)
{
	controller();
}
/*### GAME CONTROLLER ##################################################################
* Intercepts keyboards events
* Intercepts game events
#######################################################################################*/
void controller() {
	gameNotStartedEntryAction();
	while (1) {
		ppu_wait_nmi();
		interceptKeyboardEvents();
		interceptGameEvents();
	}
}
/*#### KEYBOOARDS EVENTS  ##############################################################
#######################################################################################*/
void interceptKeyboardEvents () {
	pad=pad_poll(0);
	if (pad & PAD_START) {
		padStartEvent();
	}
	if (pad & PAD_LEFT) {
		padLeftEvent();
	}
	if (pad & PAD_RIGHT){
		padRightEvent();
	}
}
/*#### GAME EVENTS  ####################################################################
* GAME_START_EVENT : Transition from GAME_NOT_STARTED to LEVEL1_NOT_STARTED state
* event_type 2 : Transition from LEVEL1_NOT_STARTED to LEVEL1_STARTED state
#######################################################################################*/
void interceptGameEvents () {
	if (event.event_type==GAME_START_EVENT) {
		gameNotStartedExitAction();
	}
	if (event.event_type==START_LEVEL1_EVENT) {
		level1NotStartedExitAction();
	}
	if (event.event_type==START_LEVEL2_EVENT) {
		level2NotStartedExitAction();
	}

	//MOVE PADDLE RIGHT
	if (event.event_type==PADDLE_RIGHT_EVENT) {
		movePaddleRightAction();
	}
	//MOVE PADDLE LEFT
	if (event.event_type==PADDLE_LEFT_EVENT) {
		movePaddleLeftAction();
	}
	//
	if (event.event_type==TOP_BORDER_COLLISION_EVENT) {
		topBorderCollisionAction();
	}
	if (event.event_type==LEFT_BORDER_COLLISION_EVENT) {
		leftBorderCollisionAction();
	}
	if (event.event_type==RIGHT_BORDER_COLLISION_EVENT) {
		rightBorderCollisionAction();
	}
	if (event.event_type==PADDLE_LEFT_PART_COLLISION_EVENT) {
		paddleLeftPartCollisionAction();
	}
	if (event.event_type==PADDLE_RIGHT_PART_COLLISION_EVENT) {
		paddleRightPartCollisionAction();
	}
	if (event.event_type==PADDLE_CENTER_PART_COLLISION_EVENT) {
		paddleCenterPartCollisionAction();
	}
	if (event.event_type==BRICK_COLLISION_EVENT) {
		brickCollisionAction();
	}
	if (event.event_type==END_LEVEL1_EVENT) {
		level1StartedExitAction();
	}
	if (event.event_type==END_LEVEL2_EVENT) {
		level2StartedExitAction();
	}
	if (event.event_type==FAILED_PARTY_EVENT) {
		gameOverEntryAction();
	}
	/////////////////////////////////////////////
	/////////////////////////////////////////////
	//NO EVENT - CONTINUE LEVEL ACTIVITY
	if (game_state == LEVEL1_STARTED) {
		playActivity();
	}
	if (game_state == LEVEL2_STARTED) {
		playActivity();
	}
}
/*#### COMMON METHODS ######################################################
#######################################################################################*/
void initPaddles() {
	paddle_x=PADDLE_X;
	paddle_y=PADDLE_Y;
	paddle_dx=1;
}
void initBall() {
	ball_x = PADDLE_X+ (2*SPRITE_WIDTH) -4;
	ball_y = PADDLE_Y - 4;

	ball_x = (3*SPRITE_WIDTH-2);
	ball_dx=0;
	ball_dy=-1;
}

/*#### PLAYER GENERATED ACTIONS ########################################################
#######################################################################################*/
void movePaddleLeftAction() {
	if (paddle_x > PADDLE_MIN_X) {
		paddle_x -= paddle_dx;
	}
}
void movePaddleRightAction() {
	if (paddle_x<PADDLE_MAX_X) {
		paddle_x += paddle_dx;
	}
}
/*#### GAME LOGIC GENERATED ACTIONS ####################################################
#######################################################################################*/
void paddleLeftPartCollisionAction() {
	ball_dx = -1;
	ball_dy = -1;
}
void paddleRightPartCollisionAction() {
	ball_dx = 1;
	ball_dy = -1;
}
void paddleCenterPartCollisionAction() {
	ball_dx = 0;
	ball_dy = -1;
}
void topBorderCollisionAction() {
	ball_dy = 1;
}
void leftBorderCollisionAction() {
	ball_dx = -ball_dx;
}
void rightBorderCollisionAction() {
	ball_dx = -ball_dx;
}
void updateScoresAndBackground() {
	digits[0]=0;digits[1]=0;digits[2]=0;digits[3]=0;digits[4]=0;
	digits[0]+=points%10;
	digits[1]+=(points - ((points/100)*100))/10;
	digits[2]+=(points/100);
	if (digits[0]>=10) {
		digits[1]+=1;
		digits[0]-=10;
	}
	if (digits[1]>=10) {
		digits[2]+=1;
		digits[1]-=10;
	}
	if (digits[2]>=10) {
		digits[3]+=1;
		digits[2]-=10;
	}
	if (digits[3]>=10) {
		digits[4]+=1;
		digits[3]-=10;
	}
	//Alignement of digits
	if (digits[4]+digits[3]+digits[2]==0) {
		updates_init[3]=0x10+digits[1];
		updates_init[4]=0x10+digits[0];
		updates_init[5]=0x00;
		updates_init[6]=0x00;
		updates_init[7]=0x00;
	}
	else if (digits[4]+digits[3]==0) {
		updates_init[3]=0x10+digits[2];
		updates_init[4]=0x10+digits[1];
		updates_init[5]=0x10+digits[0];
		updates_init[6]=0x00;
		updates_init[7]=0x00;
	}
	else if (digits[4]==0) {
		updates_init[3]=0x10+digits[3];
		updates_init[4]=0x10+digits[2];
		updates_init[5]=0x10+digits[1];
		updates_init[6]=0x10+digits[0];
		updates_init[7]=0x00;
	}
	else {
		updates_init[7]=0x10+digits[0];
		updates_init[6]=0x10+digits[1];
		updates_init[5]=0x10+digits[2];
		updates_init[4]=0x10+digits[3];
		updates_init[3]=0x10+digits[4];
	}
	//Set addreess in VRAM
	updates_init[8]=MSB(NTADR_A((2*coord_x)+3,coord_y))|NT_UPD_HORZ;
	updates_init[9]=LSB(NTADR_A((2*coord_x)+3,coord_y));
	//Choose the right background for current level and row
	if (coord_y%2 ==0) {
		if (game_state == LEVEL1_STARTED) {
			if (ball_dy == 1) {
				updates_init[11]=0xA7;
				updates_init[12]=0xA7;
			}
			else {
				updates_init[11]=0xA6;
				updates_init[12]=0xA6;
			}
		}
		if (game_state == LEVEL2_STARTED) {
			updates_init[11]=0xC7;
			updates_init[12]=0xC7;
		}
	}
	else {
		if (game_state == LEVEL1_STARTED) {
			if (ball_dy ==1) {
				updates_init[11]=0xA6;
				updates_init[12]=0xA6;
			}
			else {
				updates_init[11]=0xA7;
				updates_init[12]=0xA7;
			}
			
		}
		else if (game_state == LEVEL2_STARTED) {
			updates_init[11]=0xC6;
			updates_init[12]=0xC6;
		}
	}
	set_vram_update(updates_init);
}
/*----------------------------------------------------------
* This method :
* 	. Counts the remaining bricks in a level.
*   . Is called by level1StartedStateEntryAction
*   . Is called by level2StartedStateEntryAction
*   . Crawls collision table for the given level
*   . Update left bricks counter
*/
void countBricksInLevel() {
	left_bricks = 0;
	for (i=0; i<13; ++i) {
		for (j=0;j<13;++j) {
			if (game_state == LEVEL1_STARTED) {
				if (collisionsTableLevel1[(13*i)+j]!=0) {
					left_bricks +=1;
				}
			}
			else if (game_state == LEVEL2_STARTED){
				if (collisionsTableLevel2[(13*i)+j]!=0) {
					left_bricks +=1;
				}
			}
		}
	}
}
/*----------------------------------------------------------
* This method :
* 	. Update background in VRAM to restore WALL state
*   . All touched bricks are replaced with the current background
*   . Is called by level1StartedStateEntryAction
*   . Is called by level2StartedStateEntryAction
*   . Crawls collision table for the given level
*   . Touched bricks are identified by collision_type=0
*/
void restoreWwallState() {
	return;
	if (game_state == LEVEL1_STARTED) {
		for (i=0;i<13;++i) {
			for (j=0;j<13;++j) {
				if (collisionsTableLevel1[(13*j)+i] == 0) {
					vram_adr(NTADR_A(3+(2*i),6+j));
					if (j%2 == 0) {
						vram_fill(0xA6,2);
					}
					else {
						vram_fill(0xA7,2);
					}
				}
			}
		}
	}
	else {
		for (i=0;i<13;++i) {
			for (j=0;j<13;++j) {
				if (collisionsTableLevel2[(13*j)+i] == 0) {
					vram_adr(NTADR_A(3+(2*i),6+j));
					if (j%2 == 0) {
						vram_fill(0xC6,2);
					}
					else {
						vram_fill(0xC7,2);
					}
				}
			}
		}
	}
}
/*----------------------------------------------------------*/


void brickCollisionAction() {
	points = collisionPoints[collision_type];
	//points = coord_x;//1
	//points = coord_y;//1
	updateScoresAndBackground();
	ball_dy=-ball_dy;
	if (game_state==LEVEL1_STARTED) {
		collisionsTableLevel1[collision_case]=0;
	}
	
	left_bricks -=1;
}
void interceptWallCollisions() {
	collision_type = -1;
	collision_case = (13*(coord_y-6))+(coord_x);
	if (game_state==LEVEL1_STARTED) {
		collision_type=collisionsTableLevel1[collision_case];
	}
	if (game_state==LEVEL2_STARTED) {
		collision_type=collisionsTableLevel2[collision_case];
	}
	if (coord_y<=WALL_MIN_Y) {
		return;
	}
	if (collision_type !=0) {
		collisionWithBrickEvent();
	}
}
void interceptCollisions() {
	//Column in collision table
	//coord_x = ((ball_x/SPRITE_WIDTH)-3)/2;  
	coord_x = ((ball_x/SPRITE_WIDTH)/2)-1;
	//Row in collision table
	if (ball_dy==-1) {
		coord_y = ball_y/SPRITE_WIDTH;  
	}
	else {
		coord_y=(ball_y+4)/SPRITE_WIDTH;
	}
	//COLLISION WITH WALL?
	if (coord_y<=WALL_MAX_Y) {
		interceptWallCollisions();
	}
	//BALLS FALLS OUT THE PLAYGROUND 
	if (ball_y - 4 > BOTTOM_BORDER_Y) {
		ballFallsOutEvent();
	}
	//COLLISION WITH RIGHT BORDER
	if (ball_x + 6 > RIGHT_BORDER_X) {
		rightBorderCollisionEvent();
	}
	//COLLISION WITH LEFT BORDER?
	if (ball_x + 2 < LEFT_BORDER_X) {
		leftBorderCollisionEvent();
	}
	//COLLISION WITH BORDER TOP?
	if (ball_y < TOP_BORDER_Y) {
		topBorderCollisionEvent();
	}
	//
	//COLLISION WITH PADDLE?
	if (ball_y + 4 > PADDLE_Y) {
		if  (ball_x + 6 < paddle_x){
			//NO COLLISION
		}
		else if (ball_x +2 > paddle_x+ (4*SPRITE_WIDTH)) {
			//NO COLLISION
		}
		else {
			//LEFT PART OF PADDLE
			if (ball_x +6 < paddle_x+ SPRITE_WIDTH) {
				paddleLeftPartCollisionEvent();
			}
			//RIGHT PART OF PADDLE
			else  if (ball_x+2 > paddle_x + (3*SPRITE_WIDTH)) {
				paddleRightPartCollisionEvent();
			}
			//CENTER PART OF PADDLE
			else {
				paddleCenterPartCollisionEvent();
			}
		}
	}
}
/*#### MAIN ACTIVITY for LEVEL1 / LEVEL2  #############################################
#######################################################################################*/
void playActivity() {
	spr=0;
	oam_clear();
	spr=oam_meta_spr(paddle_x,paddle_y,spr,sprVaus);
	if (paddles == 3) {
		spr=oam_meta_spr(20,paddle_y+8,spr,sprSmallVaus);
		spr=oam_meta_spr(44,paddle_y+8,spr,sprSmallVaus);
	}
	else if (paddles == 2) {
		spr=oam_meta_spr(20,paddle_y+8,spr,sprSmallVaus);
	}
	spr=oam_meta_spr(ball_x,ball_y,spr,sprBall);
	//
	if (game_state == LEVEL1_STARTED) {
		spr=oam_meta_spr(0,8*4 -1,spr,leftBricksBorder);
		spr=oam_meta_spr(0,8*4 -1,spr,rightBricksBorder);
	}
	if (game_state == LEVEL2_STARTED) {
		spr=oam_meta_spr(0,8*4 -1,spr,leftBricksBorder);
		spr=oam_meta_spr(0,8*4 -1,spr,rightBricksBorderLevel2);
		spr=oam_meta_spr(0,8*8 -1,spr,row3Level2);
		spr=oam_meta_spr(0,8*10 -1,spr,row5Level2);
		spr=oam_meta_spr(0,8*12 -1,spr,row7Level2);
		spr=oam_meta_spr(0,8*16 -1,spr,row10Level2);
	}
	//
	event.event_type = 0;
	//
	
	ball_x += ball_dx;
	ball_y += ball_dy;
	interceptCollisions();
}
/*#### LEVEL2 STARTED STATE ########################################################
* This state has an entry action, an exit action, and an activity
* Entry action :
* 			. Init Game state number
*           . Display LEVEL2 STARTED SCREEN
*           . Restore wall state
* Activity :
*           . Play activity
* Exit action :
*           . Triggered by END_LEVELZ_EVENT
*           	* No more paddles triggers FAILED_PARTY_EVENT
*               * Paddles available triggers ???????????????
#######################################################################################*/
void level2StartedEntryAction() {
	if (game_state == LEVEL2_STARTED) {
		return;
	}
	//Update game state
	game_state = LEVEL2_STARTED;
	//Display screen and restore wall
	ppu_off();
	pal_bg(palBG2);
	vram_adr(NAMETABLE_A);
	vram_unrle(gamelevel2_rle);
	//restoreWwallState();
	ppu_on_all();
	//Reset sprites
	spr=0;
	oam_clear();
	initPaddles();
	initBall();
}
void level2StartedExitAction() {
	//Clean last brick in VRAM
	updates_init[11]=0x00;
	updates_init[12]=0x00;
	set_vram_update(updates_init);
	//
	paddles -=1;
	if (paddles == 0) {
		failedPartyEvent();
	}
	else {
		game_state-=2;
		level2NotStartedEntryAction();
	}
}
/*#### LEVEL1 STARTED STATE ########################################################
* This state has an entry action, an exit action, and an activity
* Entry action :
* 			. Init Game state number
*           . Display LEVEL1 STARTED SCREEN
*           . Restore wall state
* Activity :
*           . Play activity
* Exit action :
*           . Clear last brick in vram
*           . 
* 
#######################################################################################*/
void level1StartedEntryAction() {
	if (game_state == LEVEL1_STARTED) {
		return;
	}
	//Update game state
	game_state = LEVEL1_STARTED;
	//Display screen and restore wall
	ppu_off();
	pal_bg(palBG1);
	vram_adr(NAMETABLE_A);
	vram_unrle(gamelevel1_rle);
	//restoreWwallState();
	ppu_on_all();
	//Reset sprites
	spr=0;
	oam_clear();
	initPaddles();
	initBall();
}
void level1StartedExitAction() {
	//Clean last brick in VRAM
	updates_init[11]=0x00;
	updates_init[12]=0x00;
	set_vram_update(updates_init);
	//
	paddles -=1;
	if (paddles == 0) {
		failedPartyEvent();
	}
	else {
		game_state-=2;
		level1NotStartedEntryAction();
	}
}
/*#### LEVEL1 NOT STARTED STATE ########################################################
* This state has an entry action, an exit action, no activity
* Entry action :
* 			. Init Game state number
*           . Init number of paddles
*           . Clear sprites
*           . Display LEVEL1 NOT STARTED SCREEN
*           . Wait 60. Throw START LEVEL1 EVENT
* Exit action : 
*           . Triggered by LEVEL1_START_EVENT
*           . Transition to LEVEL1 STARTED STATE  
#######################################################################################*/
void level1NotStartedEntryAction() {
	if (game_state == LEVEL1_NOT_STARTED) {
		return;
	}
	//Update game state
	game_state = LEVEL1_NOT_STARTED;
	if (paddles == -1) {
		paddles = 3;
	}
	//Display LEVEL 1 NOT STARTED screen
	oam_clear();
	ppu_off();
	pal_bg(palBG0);
	vram_adr(NAMETABLE_A);
	vram_unrle(startlevel1_rle);
	ppu_on_all();
	//Wait 60. Throw START_LEVEL1_EVENT
	delay(60);
	event.event_type = START_LEVEL1_EVENT;
}
void level1NotStartedExitAction() {
	level1StartedEntryAction();
}
/*#### LEVEL2 NOT STARTED STATE ############################
* This state has an entry action, an exit action, no activity
* Entry action :
* 			. Init Game state number
*           . Init number of paddles
*           . Clear sprites
*           . Display LEVEL2 NOT STARTED SCREE?
*           . Wait 60. Throw START LEVEL2 EVENT
* Exit action : 
*           . Triggered by LEVEL2_START_EVENT
*           . Transition to LEVELZ STARTED STATE
#######################################################################################*/
void level2NotStartedEntryAction() {
	if (game_state == LEVEL2_NOT_STARTED) {
		return;
	}
	//Update game state
	game_state = LEVEL2_NOT_STARTED;
	if (paddles == -1) {
		paddles = 3;
	}
	//Display LEVEL 1 NOT STARTED screen
	oam_clear();
	ppu_off();
	pal_bg(palBG0);
	vram_adr(NAMETABLE_A);
	vram_unrle(startlevel2_rle);
	ppu_on_all();
	//Wait 60. Throw START_LEVEL1_EVENT
	delay(60);
	event.event_type = START_LEVEL2_EVENT;
}
void level2NotStartedExitAction() {
	level2StartedEntryAction();
}
/*#### GAME NOT STARTED STATE ###########################################################
* Initial state - This state has an entry action, an exit action, no activity
* Entry action :
*   		. Init Game state number
*           . Reset events 
*           . Display GAME NOT STARTED SCREE
* Exit action :
*           . Trigerred by GA%E_START_EVENT
*           . Transition to LEVEL1 NOT STARTED state
#######################################################################################*/
void gameNotStartedEntryAction() {
	game_state = GAME_NOT_STARTED;
	event.event_type = 0; 
	//
	ppu_off();
	pal_bg(palBG0);
	pal_spr(palSprites);
	vram_adr(NAMETABLE_A);
	vram_unrle(welcome_rle);
	ppu_on_all();
	//Reset last brick
}
void gameNotStartedExitAction() {
	level1NotStartedEntryAction();
}
//######################################################################################
/*#### GAME OVER STATE ###########################################################
* End state - This state has an entry action, no exit action, no activity
* Entry action :
*   		. Init Game state number
*           . Reset sprites
*           . Display GAME OVER SCREEN
*           . Clean last brick 
#######################################################################################*/
void gameOverEntryAction() {
	game_state = GAME_OVER;
	event.event_type = 0; 
	//
	oam_clear();
	ppu_off();
	pal_bg(palBG1);
	vram_adr(NAMETABLE_A);
	vram_unrle(gameov_rle);
	ppu_on_all();
}
//#########################################################



