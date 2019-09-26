#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "windows.h"

#define TILESIZE 8
int n = 60;
int N = n*n;
using namespace std;

struct cell{
	cell *up;
	cell *right;
	cell *down;
	cell *left;
	cell *upleft;
	cell *upright;
	cell *downleft;
	cell *downright;
	int color;
	int next_color;
};


class TileMap : public sf::Drawable, public sf::Transformable{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // загрузка текстуры
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // изменить размер массива вершин, чтобы он соответствовал размеру уровня
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // заполнить массив вершин, с одним квадратом на плиту
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // получить текущий номер плитки
                int tileNumber = tiles[i + j * width];

                // найти свое положение в текстуре плитки
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // получить указатель на квадрант текущей плитки
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // определить его 4 угла
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // определить его 4 текстурные координаты
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // применить преобразование
        states.transform *= getTransform();

        // применить текстуру плитки
        states.texture = &m_tileset;

        // нарисуйте массив вершин
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};



int main(){
	
    // создать окно
    sf::RenderWindow window(sf::VideoMode(n * TILESIZE, n * TILESIZE), "Automat");
    // определить уровень с массивом индексов плитки

  
	//скрыть консоль
	HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    
    // run the main loop
	window.setKeyRepeatEnabled(false);
	
	
    TileMap map;
    int level[N];
    
	cell tile[N];
	
	for(int i = 0; i < N; i++){
		tile[i].up = &tile[i-n];
		tile[i].left = &tile[i-1];
		tile[i].down = &tile[i+n];
		tile[i].right = &tile[i+1];
		tile[i].upleft = &tile[i-n-1];
		tile[i].upright = &tile[i-n+1];
		tile[i].downleft = &tile[i+n-1];
		tile[i].downright = &tile[i+n+1];
		tile[i].color = 0;
	}
	
	for(int i = 0; i < n; i++){
		tile[i].up = NULL;	
		tile[i].upleft = NULL;	
		tile[i].upright = NULL;		
	}
	
	for(int i = N - n; i < N; i++){
		tile[i].down = NULL;
		tile[i].downleft = NULL;	
		tile[i].downright = NULL;	
	}
	
	for(int i = 0; i < N; i+=n){
		tile[i].left = NULL;
		tile[i].upleft = NULL;	
		tile[i].downleft = NULL;		
	}
	
	for(int i = n-1; i < N; i+=n){
		tile[i].right = NULL;
		tile[i].upright = NULL;	
		tile[i].downright = NULL;		
	}
		
    srand(time(NULL));
	for(int i = 0; i < N; i++){		 
		tile[i].next_color = 0;
		tile[i].color = level[i] = rand()%8; 
	}
    if (!map.load("resources/tileset1.png", sf::Vector2u(TILESIZE, TILESIZE), level, n, n))
        return -1;
        
	window.clear();  
	window.draw(map); 
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
        	if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
    			if (event.mouseButton.button == sf::Mouse::Left)
    				for(int i = 0; i < N; i++)
						tile[i].color = level[i] = rand()%8; 
		}    
        for(int i = 0; i < N; i++){
			int next = (tile[i].color + 1)%7;
			tile[i].next_color = tile[i].color;
        	if(tile[i].left != NULL )
        		if(tile[i].left->color == next)
        			tile[i].next_color = next;
        	if(tile[i].right != NULL)
        		if(tile[i].right->color == next)
        			tile[i].next_color = next;
        	if(tile[i].up != NULL )
        		if(tile[i].up->color == next)
        			tile[i].next_color = next;
        	if(tile[i].down != NULL)
        		if(tile[i].down->color == next)
        			tile[i].next_color = next;
        	if(tile[i].upleft != NULL )
        /*		if(tile[i].upleft->color == next)
        			tile[i].next_color = next;
        	if(tile[i].upright != NULL)
        		if(tile[i].upright->color == next)
        			tile[i].next_color = next;
        	if(tile[i].downleft != NULL )
        		if(tile[i].downleft->color == next)
        			tile[i].next_color = next;
        	if(tile[i].downright != NULL)
        		if(tile[i].downright->color == next)
        			tile[i].next_color = next;*/
			if(next == 7)
				tile[i].next_color = 7;				
		}  
		for(int i = 0; i < N; i++) {
			level[i] = tile[i].color = tile[i].next_color;
			tile[i].next_color = 0;
		}
    	if (!map.load("resources/tileset1.png", sf::Vector2u(TILESIZE, TILESIZE), level, n, n))
        	return -1;
        Sleep(200);	
		window.clear();  
		window.draw(map); 			
       	window.display();
       	
    } 
    	
    return 0;
}
