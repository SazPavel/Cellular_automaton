#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "windows.h"

#define TILESIZE 8
int n = 101;
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
		
	for(int i = 0; i < N; i++){
		tile[i].color = level[i] = 0; 
		tile[i].next_color = 0;
	}
	level[N/2] = tile[N/2].color = 1;
    if (!map.load("resources/tileset1.png", sf::Vector2u(TILESIZE, TILESIZE), level, n, n))
        return -1;
        
	window.clear();  
	window.draw(map); 
	int next = 0;
	
    while (window.isOpen()){
    	
        sf::Event event;
        while (window.pollEvent(event)){
        	if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
    			if (event.mouseButton.button == sf::Mouse::Left){
					for(int i = 0; i < N; i++){
						tile[i].color = level[i] = 0; 
					}
					level[N/2] = tile[N/2].color = 1;
				}   
		}
        for(int i = 0; i < N; i++){
        	if(tile[i].left != NULL )
        		next += tile[i].left->color;
        	if(tile[i].right != NULL)
				next +=	tile[i].right->color;
        	if(tile[i].up != NULL )
        		next += tile[i].up->color;
        	if(tile[i].down != NULL)
				next +=	tile[i].down->color;
				
        	if(tile[i].upleft != NULL )
        		next += tile[i].upleft->color;
        	if(tile[i].upright != NULL)
				next +=	tile[i].upright->color;
        	if(tile[i].downleft != NULL )
        		next += tile[i].downleft->color;
        	if(tile[i].downright != NULL)
				next +=	tile[i].downright->color;
	   		if(next == 1 || tile[i].color == 1)
        		tile[i].next_color = 1;
        	next = 0;			
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
