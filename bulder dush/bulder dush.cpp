#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <cstring>
#include <fstream>
#include <iostream>
using namespace sf;
using namespace std;
class gamesound;
class PLAYER;
struct  Enemy;
struct Flash;
int gravity(string ** ,int,int, int,int,gamesound&,gamesound&,Enemy*enemy,int qantity,int*count_enemy);
int string_to_int(string );
string int_to_string(int );
string** start(int number_level,int *H_,int *W_, int *win_);
float offsetX=0, offsetY=0;
void delete_map(string**TileMap,int*H, Enemy *cube,int qantity);
void show (RenderWindow&,Sprite &,string** TileMap, int *H,int *W,Flash flash);
void set_player(RenderWindow& window,Sprite &sprite, PLAYER& player,string ** TileMap,int *H, int *W,int time, gamesound & sound);
Enemy* create_enemy(string** Tilemap,int H,int W, int *quantity);
int move_enemy(string** TileMap,Enemy* enemy,int quantity,PLAYER player);
//*******************************************************************************************************************************
struct Flash{
	Flash():diamant_flash(0),cube_flash(0),star_flash(0)
	{};
	int diamant_flash;
	int cube_flash;
	int star_flash;
};

//********************************************************************************************************************************
class gamesound{
public:
	SoundBuffer sound_bufer;
	Sound sound;
	string filename;
	gamesound (string filename_){
		filename=filename_;
		sound_bufer.loadFromFile(filename);//��������� � ���� ����
		sound.setBuffer (sound_bufer); //������� ���� � ��������� � ���� ���� �� ������

	}
	void play(){
		sound.play();
	}
};

//******************************************************************************************************************************
struct Enemy{
	Enemy():x(0),y(0),step(1),live(1),symbol('-')
	{};
	int x,y,step,live;
	char symbol;
};
//****************************************************************************************************************************
class PLAYER {

public:

	float dx,dy;    //����������� ����� � ���
	FloatRect rect; //���������� � ������ �������
	Sprite sprite;
	float currentFrame; // ����� �������
	int diamant;         // ���������� ��������� �������
	string**TileMap;     //�������� �����
	int live;
	SoundBuffer sound_bufer;
	Sound almaz;
	PLAYER(Texture &image,string **t)
	{
		sprite.setTexture(image);
		sprite.setTextureRect(IntRect(16,0,16,16));
		rect = FloatRect(1*16,1*16,16,16);
		diamant=0;
		dx=dy=0;
		currentFrame = 0;
		TileMap=t;
		live=1;

		sound_bufer.loadFromFile("sound/almaz.wav");//��������� � ���� ����

		almaz.setBuffer(sound_bufer) ;
	}

	void update(float time)    
	{	

		Collision();   //�������� �� ������������

		rect.left += dx ;	

		rect.top += dy ;

		if (currentFrame > 6) currentFrame -=6 ; //����� ������� ��������

		if (dx>0)sprite.setTextureRect(IntRect(16*int(currentFrame),32,16,16));

		if (dx<0) sprite.setTextureRect(IntRect(16*int(currentFrame)+16,32,-16,16));

		if (dy>0)sprite.setTextureRect(IntRect(16*int(currentFrame),0,16,16));

		if (dy<0) sprite.setTextureRect(IntRect(32,0,16,16));

		for (int i=0;i<10;i++){Sleep(time);}// �������� � ����
		currentFrame++;
		sprite.setPosition(rect.left-offsetX, rect.top-offsetY); // ��������� ������ �� ����

		dx=0;
		dy=0;
	}

	void Collision()
	{   
		int i = (rect.top)/16;
		int j =(rect.left)/16;
		if (dx>0&& (TileMap[i][j+1]=='R'&&TileMap[i][j+2]==' '))  {TileMap[i][j+2]='R';TileMap[i][j]=' ';TileMap[i][j+1]='-';  return;}
		if (dx<0&& (TileMap[i][j-1]=='R'&&TileMap[i][j-2]==' '))  {TileMap[i][j-2]='R';TileMap[i][j]=' ';TileMap[i][j-1]='-';  return;}
		if (dx>0&& (TileMap[i][j+1]=='W'||TileMap[i][j+1]=='H'||TileMap[i][j+1]=='R'))  {dx=0;return;}  //�������� �� ������������
		if (dx<0&& (TileMap[i][j-1]=='W'||TileMap[i][j-1]=='H'||TileMap[i][j-1]=='R')) {dx=0;return;}
		if (dy>0&& (TileMap[i+1][j]=='W'||TileMap[i+1][j]=='H'||TileMap[i+1][j]=='R')) {dy=0;return;} 
		if (dy<0&& (TileMap[i-1][j]=='W'||TileMap[i-1][j]=='H'||TileMap[i-1][j]=='R')) { dy=0;return;}

		if (TileMap[i][j]=='D') 
		{    
			diamant+=1;// �������� �������
			TileMap[i][j]='-';
			almaz.play();
		}

  
		if((dx+dy)!=0)TileMap[i][j]=' ';					
		}
	
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
// 	SetConsoleCP(1251);
// 	SetConsoleOutputCP(1251);
// 	setlocale(LC_ALL, "Russian");


	 gamesound sound_step1("sound/step1.wav");
	 gamesound sound_step2("sound/step2.wav");
	 gamesound sound_stone("sound/stone.wav");
	 gamesound sound_stone_left("sound/stone_left.wav");
     gamesound sound_life("sound/life2.wav");
	 gamesound sound_almaz("sound/almaz.wav");
	 gamesound start_door("sound/start_door.wav");
	 gamesound door("sound/door.wav");
	 
	 int time=10,H,W,i,j,win,max_level; //�������� ����,������ �������� ����, ���������� ������� ��� ���������� ������, ������������ ���������� ����
	                                    // ��������������� � start
	 int number_level=1;
	 bool gameover(false);
	 bool flag_win=false;
     bool rockfall=false; //������� ������ �������� ������
	 int quantity;

	 Flash flash;
	 int pldy=0;// �������� ����� ��� ���
	 bool button_no_press = true;
	 int kill= 0;
	 string **TileMap=start(number_level,&H,&W,&win);
	 int count_enemy=0;
     Enemy *enemy= create_enemy(TileMap,H,W,&quantity);
 //    Enemy *stars= create_enemy(TileMap,H,W,&quantity_stars,'X');
	 RenderWindow window( VideoMode(400, 225), "boulder dash");
	
  	Texture texture;
	 texture.loadFromFile("fang.png");

	Sprite sprite_map(texture);

	float currentFrame=0;
    PLAYER mario(texture,TileMap);
    
 std::ifstream  fil("game.cfg"); 
 if (!fil.is_open()){ 
	 MessageBox(GetActiveWindow(), L"not file map!", L"Boulder dash", MB_ICONERROR);
	 exit(1);
 }
 string temp="";
 getline(fil,temp,'\n');
 time=string_to_int(temp);	
 getline(fil,temp,'\n');
 mario.live=string_to_int(temp);
 getline(fil,temp,'\n');
 max_level=string_to_int(temp);
 fil.close();
//	Clock clock;
 set_player(window,sprite_map,mario,TileMap,&H,&W,time,start_door);
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed){window.close();exit(0);}
		
		}
		
		if (button_no_press && Keyboard::isKeyPressed(Keyboard::Add )) time+=1;

		if (Keyboard::isKeyPressed(Keyboard::Subtract  )) time-=1;
		
		if (time<0)time=0;
	
		button_no_press = true;// ��������� ������� ���� ������ ������������

		if (button_no_press && Keyboard::isKeyPressed(Keyboard::Left)) {mario.dx = -16; sound_step1.play();button_no_press= false;}

		if (button_no_press && Keyboard::isKeyPressed(Keyboard::Right)){ mario.dx = 16;sound_step1.play(); button_no_press= false;}

		if (button_no_press && Keyboard::isKeyPressed(Keyboard::Up)) {mario.dy=-16;sound_step2.play(); button_no_press= false;}
		
		if (button_no_press && Keyboard::isKeyPressed(Keyboard::Down)) {mario.dy=16;sound_step2.play(); button_no_press= false;}
		
		if (mario.dx+mario.dy==0)mario.sprite.setTextureRect(IntRect(16,0,16,16));

		if (mario.rect.left>208) offsetX = mario.rect.left - 208;// ��������� ������ �� ������
		else offsetX=0;
		
		if (mario.rect.top>123)offsetY = mario.rect.top - 123;
        else offsetY=0;
	    
			window.clear(Color(25,29,25));

		  if (pldy-mario.dy==0){rockfall=true; pldy=mario.dy;}
		  else {rockfall=false;pldy=mario.dy;}
			mario.update(time);

		show(window,sprite_map,TileMap,&H,&W,flash);
      kill=move_enemy(TileMap,enemy,quantity,mario);
		
			if (rockfall){        // ������� ������ ����� 
			for ( i=H-1; i>0; i--){
		  	for ( j=W-1; j>0 ; j--) {
					if (TileMap[i][j]=='R'){  // ���� ���������� ������
                     kill=gravity(TileMap,i,j,mario.rect.left/16,mario.rect.top/16,sound_stone_left,sound_stone,enemy,quantity,&count_enemy);
					}
						if(kill==1)	
						{   // ���� ������ ����� �� ������
							kill=0;
							mario.diamant=0;
							int x,y;	
							for(  int c=0;c<3;c++){ 
								for ( int i_=0;i_<3;i_++){
									for (int j_=0;j_<3;j_++){  //������ ����� �����

										y=i_-1+mario.rect.left/16;
										x=j_-1+mario.rect.top/16;

										if (TileMap[x][y]!='H')	TileMap[x][y] = int_to_string(c);
									}
								}
								show(window,sprite_map,TileMap,&H,&W,flash);
								window.display();
								for (int t=0;t<30;t++){Sleep(time);}
							}
							mario.live--;
							if (mario.live==0)gameover= true;
							delete_map(TileMap,&H,enemy,quantity);
							TileMap=start(number_level , &H , &W, &win );
							mario.TileMap=TileMap;
							set_player(window,sprite_map,mario,TileMap,&H,&W,time,start_door);		
							Enemy *enemy= create_enemy(TileMap,H,W,&quantity);
					
						}
               
						if(kill==2)	
						{   // ���� ������ ����� �� ������
							enemy[count_enemy].live=0;
							kill=0;
							int x,y;	
							for(  int c=0;c<2;c++){ 
								for ( int i_=0;i_<3;i_++){
									for (int j_=0;j_<3;j_++){  //������ ����� �����
										y=i_-1+enemy[count_enemy].x;
										x=j_-1+enemy[count_enemy].y;
										if (TileMap[x][y]!='H')	TileMap[x][y] = int_to_string(c);
										if (TileMap[x][y]!='H'&&c>=1)	TileMap[x][y] = ' ';
									}
								}
								show(window,sprite_map,TileMap,&H,&W,flash);
								window.display();
								for (int t=0;t<30;t++){Sleep(time);}
							}
						
						}
						if(kill==3)	
						{   // ���� ������ ����� �� ������
							enemy[count_enemy].live=0;
							kill=0;
							int x,y;	
							for(  int c=0;c<2;c++){ 
								for ( int i_=0;i_<3;i_++){
									for (int j_=0;j_<3;j_++){  //������ ����� �����
										y=i_-1+enemy[count_enemy].x;
										x=j_-1+enemy[count_enemy].y;
										if (TileMap[x][y]!='H')	TileMap[x][y] = int_to_string(c);
										if (TileMap[x][y]!='H'&&c>=1)	TileMap[x][y] = 'D';
									}
								}
								show(window,sprite_map,TileMap,&H,&W,flash);
								window.display();
								for (int t=0;t<30;t++){Sleep(time);}
							}

						}

			
			
			}
			 }            
			}
			
	
			flash.diamant_flash+=1;
			if(flash.diamant_flash>7)flash.diamant_flash=0; //������ ������ ������
			flash.cube_flash+=1;
			if(flash.cube_flash>3)flash.cube_flash=0; //������ ������ ������
			flash.star_flash+=1;
			if(flash.star_flash>3)flash.star_flash=0; //������ ������ ������
			
			
			window.draw(mario.sprite);
       
			std::ostringstream shift;    // �������� ����������
			shift << mario.diamant;
 			Font font;//����� 
			font.loadFromFile("Impact Regular.ttf");//�������� ������ ������ ���� ������
 			
			Text text1("", font,10);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
 			 text1.setColor(Color::White);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
 			// text.setStyle(sf::Text::Bold | sf::Text::Underlined);//������ � ������������ �����. �� ��������� �� "�����":)) � �� ������������
 		
			 text1.setString(L"������� ������:"+shift.str());//������ ������ ������
			 text1.setPosition(0, 0);
			 window.draw(text1);
			 
			 Text text2("", font,10);
             shift.str("");
			 shift<<time;
		   	text2.setPosition(120, 0);//������ ������� ������, ����� ������
			 text2.setString(L"���������� ���� "+shift.str());
 			 window.draw(text2);//����� ���� �����
 				
			 Text text3("", font,10);
			 shift.str("");
			 shift<<mario.live;
			 text3.setPosition(250, 0);//������ ������� ������, ����� ������
			 text3.setString(L"     ����� "+shift.str());
			 window.draw(text3);//����� ���� �����
			 
			 Text text4("", font,10);
			 shift.str("");
			 shift<<number_level;
			 text4.setPosition(320, 0);//������ ������� ������, ����� ������
			 text4.setString(L"������� "+shift.str());
			 window.draw(text4);//����� ���� �����

			 window.display();
			 if (gameover){     // ��������
				delete_map(TileMap,&H,enemy,quantity);
              while(1){
				 window.clear(Color(25,29,25));
				 Text text2("", font,40);
				 text2.setPosition(80, 80);//������ ������� ������, ����� ������
				 text2.setString(" GAME  OVER ");
				 window.draw(text2);//����� ���� �����
		    	window.display();
				 while (window.pollEvent(event)){
					 if (event.type == Event::Closed){window.close();exit(0);}
				   
				   if (Keyboard::isKeyPressed(Keyboard::Escape)||Keyboard::isKeyPressed(Keyboard::Return))exit(0);
			  }
			 }
			 }
			 if(mario.diamant>=win&&flag_win==false){ //��� ������ ������� ���������� ������� ��������� �����
				 flag_win=true;
				 for ( i=0; i<H; i++){
					 for ( j=0; j<W ; j++) 
					 { 
						 if (TileMap[i][j]=='E'){TileMap[i][j]='e';door.play(); }
					 }
				 }			 
			 } 
			 i = (mario.rect.top)/16;
		     j =(mario.rect.left)/16;
		   if (TileMap[i][j]=='e'){  //������� ������ �� ������ �������
			   number_level++;
			   if (number_level>max_level){
				   delete_map(TileMap,&H,enemy,quantity);
				   while(1){
					   window.clear(Color(25,29,25));
					   Text text2("", font,40);
					   text2.setPosition(80, 80);//������ ������� ������, ����� ������
					   text2.setString(" END OF GAME  ");
					   window.draw(text2);//����� ���� �����
					   while (window.pollEvent(event)){
						   if (event.type == Event::Closed){ window.close();exit(0);}
					   window.display();
					   if (Keyboard::isKeyPressed(Keyboard::Escape)||Keyboard::isKeyPressed(Keyboard::Return))exit(0);
				  
					   }
					   }  
			   }
			   mario.diamant=0;
			   flag_win=false;
			   delete_map(TileMap,&H,enemy,quantity);
			   TileMap=start(number_level, &H , &W, &win );
			   mario.TileMap=TileMap;
			   set_player(window,sprite_map,mario,TileMap,&H,&W,time,start_door);
		   }
	}	
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------

int gravity(string **TileMap,int i,int j, int plx,int ply,gamesound &stone_left,gamesound &stone,Enemy *enemy,int qantity, int*count_enemy){
	int count;
	if (TileMap[i+1][j]=='R'||TileMap[i+1][j]=='W'){
		if((TileMap[i+1][j+1]=='X'||TileMap[i+1][j+1]=='C')&&TileMap[i][j+1]==' '){
		    for (int c=0;c<qantity;c++){
				if(enemy[c].x==j+1&&enemy[c].y==i+1)*count_enemy=c;
			}
		   count=*count_enemy;
			if(enemy[count].symbol=='C')return 2;
		return 3;
		}
		if((j+1==plx&&i+1==ply)&&TileMap[i][j+1]==' ')return 1;
		if(TileMap[i+1][j+1]==' '&&TileMap[i][j+1]==' '){
			//if(j+1==plx&&i+2==ply)return 1;
			TileMap[i+1][j+1]=TileMap[i][j];
			TileMap[i][j]=' ';
			stone_left.play();
			return 0;
		}
			if((TileMap[i+1][j-1]=='X'||TileMap[i+1][j-1]=='C')&&TileMap[i][j-1]==' '){
				for (int c=0;c<qantity;c++){
					if(enemy[c].x==j-1&&enemy[c].y==i+1)*count_enemy=c;
				}
				count=*count_enemy;
				if(enemy[count].symbol=='C')return 2;
				return 3;
			}
		if((j-1==plx&&i+2==ply)&&TileMap[i][j+1]==' ')return 1;
		if(TileMap[i+1][j-1]==' '&&TileMap[i][j-1]==' '){
			TileMap[i+1][j-1]=TileMap[i][j];
			TileMap[i][j]=' ';
			stone_left.play();
			return 0;
		}
	}

	if(TileMap[i+1][j]=='X'||TileMap[i+1][j]=='C'){
		for (int c=0;c<qantity;c++){
			if(enemy[c].x==j&&enemy[c].y==i+1)*count_enemy=c;
		}
		count=*count_enemy;
		if(enemy[count].symbol=='C')return 2;
		return 3;
	}	
if (TileMap[i+1][j]!=' ')return 0;
if(j==plx&&i+2==ply)
{return 1;}

	 TileMap[i+1][j]=TileMap[i][j];
	 TileMap[i][j]=' ';
	 stone.play();
	 return 0;
}
//*************************************************************************************************************************************
int string_to_int(string s){
	int n=0;
	stringstream ss;
	ss<<s;
	ss>>n;
	return n;
}
//*************************************************************************************************************************************
string int_to_string(int n){
	string s;
	stringstream ss;
	ss<<n;
	ss >>s;
	return s;
}

//***********************************************************************************************************************************
string** start(int number_level,int *H_,int *W_, int *win_){
	
	int win;
	int H,W;
	string temp="";
	string level="level";
	level+=int_to_string(number_level);
	level+=".map";
	std::ifstream  fil(level); // (����� �� ���������� ��� �����)
	if (!fil.is_open()){ // ���� ���� �� ������
	MessageBox(GetActiveWindow(), L"not file map!", L"Boulder dash", MB_ICONERROR);
		exit(1);
	}
	getline(fil,temp,'\n');
	win=string_to_int(temp);
	getline(fil,temp,'\n');
	H=string_to_int(temp);
	getline(fil,temp,'\n');
	W=string_to_int(temp);

	string  **TileMap = new string  *[H];
	for (int i = 0; i < H; i++) {          // ���������
		TileMap[i] = new string [W]; // ������ 
	}   

	int i=0;
	temp="";
	while(i<H) {// ��������� �����

		getline(fil,temp,'\n');
		if(temp.length()!=W){
		MessageBox(GetActiveWindow(), L"wrong map file!", L"Boulder dash", MB_ICONERROR);
			exit(1);
		}
		for(int j=0;j<W;j++){
			TileMap[i][j]=temp[j];
		}
		temp="";
		i++;
	}
	if (i!=H){
		MessageBox(GetActiveWindow(), L"wrong map file!", L"Boulder dash", MB_ICONERROR);
		exit(1);
	}
	fil.close(); // ��������� ����
	*win_=win;
	*H_=H;
	*W_=W;
	return TileMap;
}
//***************************************************************************************************************************************
void delete_map(string** TileMap,int *H_, Enemy *enemy,int qantity){
	int H=*H_;
	for (int i = 0; i < H; i++) {          // ���������
		delete [] TileMap[i];
	}  
	delete[]TileMap;
	if(qantity!=0)delete[] enemy;
	else enemy=nullptr;
	
}
//*************************************************************************************************************************************************
void show(RenderWindow& window, Sprite & sprite,string** TileMap, int *H,int *W, Flash flash){
	window.clear(Color(25,29,25));
	for (int i=0; i<*H; i++)
		for (int j=0; j<*W ; j++) 
		{ 
			if (TileMap[i][j]=='W')    sprite.setTextureRect(IntRect(32,48,16,16));  

			if (TileMap[i][j]=='D')  sprite.setTextureRect(IntRect(flash.diamant_flash*16,128,16,16));     

			if (TileMap[i][j]=='H')    sprite.setTextureRect(IntRect(0,48,16,16));

			if (TileMap[i][j]=='-')    sprite.setTextureRect(IntRect(64,48,16,16));

			if (TileMap[i][j]=='R')    sprite.setTextureRect(IntRect(80,48,16,16));

		    if (TileMap[i][j]=='0')    sprite.setTextureRect(IntRect(112,96,16,16));

			if (TileMap[i][j]=='1')    sprite.setTextureRect(IntRect(112,80,16,16));
		
			if (TileMap[i][j]=='2')    sprite.setTextureRect(IntRect(112,64,16,16));

			if (TileMap[i][j]=='S')    sprite.setTextureRect(IntRect(0,48,16,16));

			if (TileMap[i][j]=='s')   sprite.setTextureRect(IntRect(16,48,16,16));

			if (TileMap[i][j]=='e')   sprite.setTextureRect(IntRect(16,48,16,16));

			if (TileMap[i][j]=='C')    sprite.setTextureRect(IntRect(80,64+(16*flash.cube_flash),16,16));

			if (TileMap[i][j]=='X')    sprite.setTextureRect(IntRect(96,64+(16*flash.star_flash),16,16));

			if (TileMap[i][j]==' '){continue;}

			sprite.setPosition(j*16-offsetX,i*16-offsetY ) ; 
			window.draw(sprite);
	
		}
		//window.display();

}
//*************************************************************************************************************************************
void set_player(RenderWindow& window,Sprite &sprite, PLAYER& player,string ** TileMap,int *H, int *W,int time, gamesound & sound){
	
	Flash f;
	int x=1,y=1;
	for (int i=0; i<*H; i++){
		for (int j=0; j<*W ; j++) 
		{ 
			if (TileMap[i][j]=='S'){TileMap[i][j]=' ';y=j;x=i;}

		}
	}
		player.rect=FloatRect(y*16,x*16,16,16);
		if (player.rect.left>208) offsetX = player.rect.left - 208;// ��������� ������ �� ������
     else offsetX=0;
		if (player.rect.top>123)offsetY = player.rect.top - 123;
	   else offsetY=0;
		player.update(time);
		for (int c =0;c<2;c++){
		TileMap[x][y]='S';
		show(window,sprite,TileMap,H,W,f);
        window.display();
		 sound.play();
		for (int t=0;t<30;t++){Sleep(time);}
		TileMap[x][y]='s';
		show(window,sprite,TileMap,H,W,f);
		window.display();
        for (int t=0;t<30;t++){Sleep(time);}
		sound.play();
		}
	 
}
//************************************************************************************************************************************
Enemy* create_enemy(string** Tilemap,int H,int W, int *quantity){
	char name_enemy1='C';
	char name_enemy2='X';
	int quantity_=0;
	for (int i=0; i<H; i++){
		for (int j=0; j<W ; j++)
		{
		if (Tilemap[i][j]==name_enemy1||Tilemap[i][j]==name_enemy2)
		{quantity_++;}
		}
	}
		*quantity=quantity_;
	if (quantity_==0){
		  Enemy*temp;

		return temp;
	}
    Enemy*temp= new Enemy[quantity_];
	int c=0;
	for (int i=0; i<H; i++)
		for (int j=0; j<W ; j++){
         if (Tilemap[i][j]==name_enemy1){
			 temp[c].x=j;temp[c].y=i;temp[c].symbol=name_enemy1;c++; }
		 if (Tilemap[i][j]==name_enemy2){
			 temp[c].x=j;temp[c].y=i;temp[c].symbol=name_enemy2;c++; }
		}
		return temp;
}
//************************************************************************************************************************************
int move_enemy(string** TileMap,Enemy* enemy,int quantity,PLAYER player){
	
	if(quantity==0)return 0;
	int player_x=player.rect.left/16;
	int player_y=player.rect.top/16;

		for (int c=0;c<quantity;c++){
	if(enemy[c].live==0)continue;
	 
			if((enemy[c].y==player_y&&enemy[c].x-1==player_x)||(enemy[c].y+1==player_y&&enemy[c].x==player_x)||(enemy[c].y==player_y&&enemy[c].x+1==player_x)||(enemy[c].y-1==player_y&&enemy[c].x==player_x))return 1;
			switch(enemy[c].step){
			
		case 1:   
			
				 if(TileMap[enemy[c].y-1][enemy[c].x]==' '){
			       TileMap[enemy[c].y-1][enemy[c].x]=enemy[c].symbol;
			       TileMap[enemy[c].y][enemy[c].x]=' ';
			        enemy[c].y--;
				   enemy[c].step=4;
				   }
				   else 
					if(TileMap[enemy[c].y][enemy[c].x-1]==' '){
					TileMap[enemy[c].y][enemy[c].x-1]=enemy[c].symbol;
					TileMap[enemy[c].y][enemy[c].x]=' ';
					enemy[c].x--;
				}
	   
				else enemy[c].step=2;
				break;	
		case 2:		
		
			       if(TileMap[enemy[c].y][enemy[c].x-1]==' '){
			       TileMap[enemy[c].y][enemy[c].x-1]=enemy[c].symbol;
			       TileMap[enemy[c].y][enemy[c].x]=' ';
			        enemy[c].x--;
                    enemy[c].step=1;
				   }
			       else if(TileMap[enemy[c].y+1][enemy[c].x]==' '){
					TileMap[enemy[c].y+1][enemy[c].x]=enemy[c].symbol;
					TileMap[enemy[c].y][enemy[c].x]=' ';
					enemy[c].y++;
				}
				else enemy[c].step=3;
					break;	
		case 3:	
	
			     if(TileMap[enemy[c].y+1][enemy[c].x]==' '){
			       TileMap[enemy[c].y+1][enemy[c].x]='C';
			       TileMap[enemy[c].y][enemy[c].x]=' ';
			        enemy[c].y++;
					 enemy[c].step=2;
				 }
				 else  if(TileMap[enemy[c].y][enemy[c].x+1]==' '){
					TileMap[enemy[c].y][enemy[c].x+1]=enemy[c].symbol;
					TileMap[enemy[c].y][enemy[c].x]=' ';
					enemy[c].x++;
			
				}
				else enemy[c].step=4;
					break;
		case 4:
	
			       if(TileMap[enemy[c].y][enemy[c].x+1]==' '){
			       TileMap[enemy[c].y][enemy[c].x+1]=enemy[c].symbol;
			       TileMap[enemy[c].y][enemy[c].x]=' ';
			        enemy[c].x++;
					enemy[c].step=3;
				   }
				 
			else if(TileMap[enemy[c].y-1][enemy[c].x]==' '){
					TileMap[enemy[c].y-1][enemy[c].x]=enemy[c].symbol;
					TileMap[enemy[c].y][enemy[c].x]=' ';
					enemy[c].y--;	
				}
				 else  enemy[c].step=1;
				 	break;

			}
		}
return 0;
}